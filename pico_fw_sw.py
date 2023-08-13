from picozero import  pico_led
import rp2
from machine import Pin

#make the led of pico as our board status check
pico_led.on()

#define the PIO with asm
@rp2.asm_pio(set_init=rp2.PIO.IN_LOW) #initialize the PIO
#define the IQ data function for PIO
def IQdata():
    wrap_target()
    