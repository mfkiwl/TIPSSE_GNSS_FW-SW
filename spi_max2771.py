# Define SPI
# Settings
import utime
from machine import Pin

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
