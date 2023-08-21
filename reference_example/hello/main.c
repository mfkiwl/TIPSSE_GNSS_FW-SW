#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "hello.pio.h"

static inline void hello_program_init(PIO pio, uint sm, uint offset, uint pin)
{
    pio_sm_config c = hello_program_get_default_config(offset);

    // Map the state machine's OUT pin group to one pin, namely the `pin`
    // parameter to this function.
    // sm_config_set_out_pins(&c, pin, 1);

    sm_config_set_set_pins(&c, pin, 3);

    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, pin+1);
    pio_gpio_init(pio, pin+2);
    // Set the pin direction to output at the PIO
    //pio_sm_set_consecutive_pindirs(pio, sm, pin, 3, true);

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

int main()
{
    const uint IO_PIN = 0;
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    PIO pio = pio0;
    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(pio, &hello_program);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    uint sm = pio_claim_unused_sm(pio, true);
    hello_program_init(pio, sm, offset, IO_PIN);

    stdio_init_all();
    while (true) {
        printf("Hello, world!\n");
        // gpio_put(LED_PIN, 1);
        pio_sm_put_blocking(pio, sm, 1);
        sleep_ms(500);
        // gpio_put(LED_PIN, 0);
        pio_sm_put_blocking(pio, sm, 0);
        sleep_ms(500);
    }
}