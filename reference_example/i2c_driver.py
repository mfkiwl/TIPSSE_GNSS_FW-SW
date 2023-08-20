import time
from machine import Pin
from rp2 import PIO, StateMachine, asm_pio

# Define PIO program instructions for START, STOP, and ACK
I2C_SC0_SD0 = 0
I2C_SC0_SD1 = 1
I2C_SC1_SD0 = 2
I2C_SC1_SD1 = 3

# Define PIO program for starting I2C communication
'''
program=[
    PIO.OUT_LOW,    # Set SCL = 0, SDA = 0
    PIO.OUT_LOW,    # Set SCL = 0, SDA = 1
    PIO.OUT_HIGH,   # Set SCL = 1, SDA = 0
    PIO.OUT_HIGH    # Set SCL = 1, SDA = 1
]
'''

@asm_pio()
def i2c_program():
    wrap_target()
    set(pindirs, 0) [7]
    set(pindirs, 1) [7]
    set(pindirs, 0) [7]
    set(pindirs, 1) [7]
    wrap()

# Initialize PIO program
sm = StateMachine(0, i2c_program, freq=32_000_000, set_base=Pin(0))  # Adjust Pin(0) to your SDA pin
sm.active(1)
print('finish')
# Define Python function to send a start condition
def i2c_start():
    sm.put(I2C_SC0_SD0)

# Define Python function to send a stop condition
def i2c_stop():
    sm.put(I2C_SC1_SD1)

# Define Python function to send a byte and receive ACK/NACK
def i2c_send_byte(byte, nack=False):
    for i in range(8):
        bit = (byte >> (7 - i)) & 1
        sm.put(I2C_SC1_SD0 if bit else I2C_SC0_SD0)
        sm.put(I2C_SC1_SD1 if bit else I2C_SC0_SD1)

    sm.put(I2C_SC0_SD0)  # Set SCL = 0, SDA = 0 (for ACK/NACK)
    sm.put(I2C_SC1_SD1 if nack else I2C_SC0_SD1)  # Send ACK/NACK
    sm.put(I2C_SC1_SD0)  # Set SCL = 1, SDA = 0 (for ACK/NACK)
    sm.put(I2C_SC1_SD1)  # Set SCL = 1, SDA = 1 (stop)

# Example usage
print('finish')
i2c_start()
print('finish')
i2c_send_byte(0x55)
i2c_send_byte(0xAA)
i2c_stop()
print('finish')
