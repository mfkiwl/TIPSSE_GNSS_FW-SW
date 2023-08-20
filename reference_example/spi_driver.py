import rp2
from machine import Pin
import time

@rp2.asm_pio(out_shiftdir=0, autopull=True, pull_thresh=8, autopush=True, push_thresh=8, sideset_init=(rp2.PIO.OUT_LOW, rp2.PIO.OUT_HIGH), out_init=rp2.PIO.OUT_LOW)
def spi_cpha0():
    set(x, 6)
    wrap_target()
    pull(ifempty).side(0x2)   [1]
    label("bitloop")
    out(pins, 1).side(0x0)    [1]
    in_(pins, 1).side(0x1)
    jmp(x_dec, "bitloop").side(0x1)

    out(pins, 1).side(0x0)
    set(x, 6).side(0x0)
    in_(pins, 1).side(0x1)
    jmp(not_osre, "bitloop").side(0x1)

    nop().side(0x0)           [1] # CSn back porch
    wrap()

class PIOSPI:
    def __init__(self, sm_id, pin_clock, pin_enable, pin_data, cpha=False, cpol=False, freq=1000000):
        assert(not (cpol or cpha))
        self._sm = rp2.StateMachine(sm_id, spi_cpha0, freq=4 * freq, sideset_base=Pin(pin_enable), out_base=Pin(pin_data), in_base=Pin(pin_data))
        self._sm.active(1)

    def write_blocking(self, wdata):
        for b in wdata:
            self._sm.put(b << 24)

    def read_blocking(self, n):
        data = []
        for i in range(n):
            data.append(self._sm.get() & 0xff)
        return data

    def write_read_blocking(self, wdata):
        rdata = []
        for b in wdata:
            self._sm.put(b << 24)
            rdata.append(self._sm.get() & 0xff)
        return rdata

# Define your pin numbers here
PIN_CLOCK = 0
PIN_ENABLE = 1
PIN_DATA = 2


# Create an instance of PIOSPI
spi = PIOSPI(sm_id=0, pin_clock=PIN_CLOCK, pin_enable=PIN_ENABLE, pin_data=PIN_DATA)

#Prepare data
# Data to be written
write_data = [0x55, 0xAA, 0x33, 0x66, 0xFF]
# Read data
#spi.read_blocking(write_data)
while True:
    # Write data
    spi.write_blocking(write_data)
    time.sleep(0.1)