from picozero import pico_led
from machine import Pin, SPI
from time import sleep

pico_led.on() # check pico status

# Pin define
# chip open closed pin define
shdn = Pin(4, Pin.OUT)
# SPI pin define
csn = Pin(1, Pin.OUT)
mosi = Pin(3)
spi = SPI(0, baudrate=1_000_000, polarity=0, phase=0, bits=8, sck=Pin(2), mosi=mosi, miso=Pin(0))



#init chip
shdn.high() #Open the chip

# default config 3 value 0x0EAFA1DC
# first time write STRMEN
csn.low()
spi.write(b'\x00\x20') #write value of config 3 (0x02)
spi.write(b'\x0e\xaf\xa9\xdc')  #write STRMEN
csn.high()

sleep(0.1)

# second time write STRMSTART
csn.low()
spi.write(b'\x00\x20') #write value of config 3 (0x02)
spi.write(b'\x0e\xaf\xad\xdc') #write STRMSTART
csn.high()

sleep(0.1)

csn.low()
spi.write(b'\x00\x28') #read value of config 3 (0x02)
mosi.init(mode=Pin.IN)
result = spi.read(4)
spi = SPI(0, baudrate=1_000_000, polarity=0, phase=0, bits=8, sck=Pin(2), mosi=mosi, miso=Pin(0))
csn.high()

print(result)

while True:
    sleep(1)