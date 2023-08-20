import rp2
from machine import Pin
from rp2 import PIO, StateMachine, asm_pio
import time

@rp2.asm_pio(out_shiftdir=0, autopull=True, pull_thresh=8, autopush=False, push_thresh=8, sideset_init=(rp2.PIO.OUT_LOW, rp2.PIO.OUT_HIGH), out_init=rp2.PIO.OUT_LOW)
def spi_cpha0():
    wrap_target()
    label("stall")
    pull()
    label("loop1")
    nop().side(0x0)
    in_(pins, 1).side(0x0)
    out(pins, 1).side(0x0)
    nop().side(0x1)
    jmp(not_osre, "loop1").side(0x1)
    push().side(0x0)
    wrap()
    

class PIOSPI:
    """
    Raspberry Pi Pico-specific drop-in replacement for Micropython's stock
    :class:`machine.SPI` class, implemented as a Programmable Input/Output
    (PIO)-optimized class.
    
    Attributes
    --------------
    _sm : StateMachine
        Finite state machine (FSM) underlying the PIO performed by this class.
    """
    
    #FIXME: Generalize to support enabling the "cpha" and "cpol" parameters.
    def __init__(
        self,
        pio_statemachine_id,
        mosi,
        miso,
        sck,
        baudrate,
        cpha=False,
        cpol=False,
    ):
        """
        Create this PIO-optimized SPI interface.

        Parameters
        --------------
        pio_statemachine_id : int
            0-based index of the PIO-based finite state machine (FSM) to run
            this SPI implementation on. Since the Pico supports 8 PIO FSMs,
            this *must* be an integer in ``[0, 7]``.
        """
        
        # If this SPI baudrate exceeds the Pico's system clock frequency,
        # raise an exception.
        if baudrate > 125000000:
            raise ValueError(
                f'SPI baudrate {baudrate} > system clock frequency {BAUDRATE_MAX}.')
        
        # If the caller attempted to enable either CPHA or CPOL, raise an
        # exception. This run_spi_cpol_0_cpha_0() assembly deferred to below
        # currently assumes this to be the case.
        if cpha:
            raise ValueError('Enabling CPHA currently unsupported.')
        if cpol:
            raise ValueError('Enabling CPOL currently unsupported.')
        
        # Create, classify, and enable a new FSM running this PIO assembly.
        self._sm = StateMachine(
            # These first two parameters *MUST* be passed positionally.
            pio_statemachine_id,
            spi_cpha0,
            # All remaining parameters *MAY* be passed by keyword.
            #FIXME: Why the "4" multiplier magic number here? *sigh*
            freq=4 * baudrate,
            sideset_base=sck,
            out_base=mosi,
            in_base=miso,
        )
        self._sm.active(1)


    def read(self, buffer_out):
        for byte_index in range(len(buffer_out)):
            buffer_out[byte_index] = self._sm.get() & 0xff


    def write(self, buffer_in):        
        for byte_in in buffer_in:
            self._sm.put(byte_in << 24)
            
            # Drain the RX FIFO to avoid catastrophic memory leaks.
            self._sm.get() & 0xff


    def write_readinto(self, buffer_in, buffer_out):        
        for byte_index in range(len(buffer_out)):
            self._sm.put(buffer_in[byte_index] << 24)
            buffer_out[byte_index] = self._sm.get() & 0xff

# Define your pin numbers here
PIN_CLOCK = 0
PIN_ENABLE = 1
PIN_DATA = 2


# Create an instance of PIOSPI
spi = PIOSPI(0, sck=Pin(PIN_CLOCK), mosi=Pin(PIN_DATA), miso=Pin(PIN_ENABLE), baudrate=1000000)

#Prepare data
# Data to be written
write_data = [0x55, 0xAA, 0x33, 0x66, 0xFF]
# Read data
#spi.read_blocking(write_data)
while True:
    # Write data
    spi.write([0x06])
    time.sleep(0.1)