# PIO Input test for reading data from a 6 bit rotary encoder
from picozero import  pico_led
from machine import Pin
from rp2 import PIO, StateMachine, asm_pio
import time

#make the led of pico as our board status check
pico_led.on()

#Input pin define
Pin (13,Pin.IN,Pin.PULL_DOWN)

#@asm_pio(out_init=(rp2.PIO.OUT_LOW ,) * 9, out_shiftdir=PIO.SHIFT_RIGHT)
@asm_pio()
#define the IQ data function for PIO
def IQdata():
    wrap_target() #this is the start of the wrapped 
    in_(pins,10) #input 10 bit
    push() #push it to FIFO
    wrap() # this is the end of wrapped 



# start IQ data reading
sm = rp2.StateMachine(0, IQdata, freq=2000, in_base=Pin(13))
sm.active(1) # to start our PIO application
Value = sm.get()
print(Value)
while True:
    sm.active(1)
    Value = sm.get()
    print(Value)
    time.sleep(0.1)
