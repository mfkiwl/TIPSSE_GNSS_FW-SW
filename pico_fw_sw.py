from picozero import  pico_led
import rp2
import time
from machine import Pin
import machine
import utime
import ustruct
import sys


#make the led of pico as our board status check
pico_led.on()

## Define SPI

# Define the pins
SCLK = Pin(14, Pin.OUT)
SDATA = Pin(15, Pin.OUT)
CS = Pin(16, Pin.OUT)

SCLK_CYC = 1

# Default MAX2771 register settings
reg_default = [
    [0xA2241797, 0x20550288, 0x0E9F21DC, 0x69888000, 0x00082008, 0x0647AE70,
     0x08000000, 0x00000000, 0x01E0F281, 0x00000002, 0x00000004],
    [0xA224A019, 0x28550288, 0x0E9F31DC, 0x78888000, 0x00062008, 0x004CCD70,
     0x08000000, 0x10000000, 0x01E0F281, 0x00000002, 0x00000004]
]

# Delay function
def delay(cyc):
    for _ in range(cyc):
        utime.sleep_us(1)

# Digital read function
def digital_read(port):
    return SDATA.value()

# Digital write function
def digital_write(port, dat):
    SDATA.value(dat)

# Write SCLK function
def write_sclk():
    SCLK.value(1)
    delay(SCLK_CYC)
    SCLK.value(0)
    delay(SCLK_CYC)

# Write SDATA function
def write_sdata(dat):
    digital_write(SDATA, dat)
    write_sclk()

# Read SDATA function
def read_sdata():
    dat = digital_read(SDATA)
    write_sclk()
    return dat

# Write MAX2771 SPI frame header
def write_head(addr, mode):
    for i in range(11, -1, -1):
        write_sdata((addr >> i) & 1)
    write_sdata(mode)

    for _ in range(3):
        write_sdata(0)
    delay(SCLK_CYC)

# Write MAX2771 register
def write_reg(cs, addr, val):
    CS.value(0)
    write_head(addr, 0)

    for i in range(31, -1, -1):
        write_sdata((val >> i) & 1)
    CS.value(1)

# Read MAX2771 register
def read_reg(cs, addr):
    val = 0
    CS.value(0)
    write_head(addr, 1)

    for i in range(31, -1, -1):
        val <<= 1
        val |= read_sdata()
    CS.value(1)
    return val


#define the PIO with asm
@rp2.asm_pio(set_init=rp2.PIO.IN_LOW) #initialize the PIO
#define the IQ data function for PIO
def IQdata():
    wrap_target()
    in_(pins,1)
    in_(pins,1)
    wrap()
    

# Assign chip select (CS) pin (and start it high)
cs = machine.Pin(17, machine.Pin.OUT)
# Initialize SPI



# start IQ data reading
sm = rp2.StateMachine(0, IQdata, freq=2000, set_base=Pin(25))
sm.active(1)
time.sleep(3)
sm.active(0)




'''
@rp2.asm_pio(set_init=rp2.PIO.OUT_LOW)
def blink():
    wrap_target()
    set(pins, 1)   [31]
    nop()          [31]
    nop()          [31]
    nop()          [31]
    nop()          [31]
    set(pins, 0)   [31]
    nop()          [31]
    nop()          [31]
    nop()          [31]
    nop()          [31]
    wrap()

# Instantiate a state machine with the blink program, at 2000Hz, with set bound to Pin(25) (LED on the Pico board)
sm = rp2.StateMachine(0, blink, freq=2000, set_base=Pin(25))

# Run the state machine for 3 seconds.  The LED should blink.
sm.active(1)
time.sleep(3)
sm.active(0)
'''