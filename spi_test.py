from picozero import pico_led
from machine import Pin, SPI
from time import sleep

pico_led.on()

shdn = Pin(4, Pin.OUT)

csn = Pin(1, Pin.OUT)
mosi = Pin(3)
spi = SPI(0, baudrate=1_000_000, polarity=0, phase=0, bits=8, sck=Pin(2), mosi=mosi, miso=Pin(0))

shdn.high()

while True:

    csn.low()

    spi.write(b'\x00\x28')
    mosi.init(mode=Pin.IN)
    spi.read(4)
    spi = SPI(0, baudrate=1_000_000, polarity=0, phase=0, bits=8, sck=Pin(2), mosi=mosi, miso=Pin(0))

    csn.high()

    sleep(0.1)