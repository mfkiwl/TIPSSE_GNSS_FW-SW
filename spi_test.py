from picozero import pico_led
from machine import Pin, SPI
from time import sleep

pico_led.on()

shdn = Pin(4, Pin.OUT)

csn = Pin(1, Pin.OUT)
spi = SPI(0, baudrate=1_000_000, polarity=0, phase=0, bits=8, sck=Pin(2), mosi=Pin(3), miso=Pin(0))

shdn.high()

while True:

    csn.low()

    spi.write(b'\x00\x28')
    spi.read(4)

    csn.high()

    sleep(0.1)