#include <stdio.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "hello.pio.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CSN_PIN = 0;
const uint SCLK_PIN = 1;
const uint SDATA_PIN = 2;

static inline void hello_program_init(PIO pio, uint sm)
{
    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(pio, &hello_program);

    pio_sm_config cfg = hello_program_get_default_config(offset);

    sm_config_set_clkdiv(&cfg, 20);

    sm_config_set_set_pins(&cfg, CSN_PIN, 3);
    sm_config_set_sideset_pins(&cfg, SCLK_PIN);
    sm_config_set_out_pins(&cfg, SDATA_PIN, 1);
    sm_config_set_in_pins(&cfg, SDATA_PIN);
    sm_config_set_jmp_pin(&cfg, SDATA_PIN);

    sm_config_set_out_shift(&cfg, true, false, 32);
    sm_config_set_in_shift(&cfg, false, false, 32);

    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, CSN_PIN);
    pio_gpio_init(pio, SCLK_PIN);
    pio_gpio_init(pio, SDATA_PIN);

    // Set the pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, CSN_PIN, 3, true);

    // SPI is synchronous, so bypass input synchroniser to reduce input delay.
    hw_set_bits(&pio->input_sync_bypass, 1u << SDATA_PIN);

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &cfg);
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

uint32_t max2771_read(PIO pio, uint sm, uint32_t addr)
{
    pio_sm_put_blocking(pio, sm, 0xF000 | __rev(addr) >> 20);
    return pio_sm_get_blocking(pio, sm);
}

int main()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    PIO pio = pio0;

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    uint sm = pio_claim_unused_sm(pio, true);

    hello_program_init(pio, sm);

    stdio_init_all();
    while (true) {
        printf("MAX2771 registers:\n");
        // gpio_put(LED_PIN, 1);
        printf("%08x\n", max2771_read(pio, sm, 0x00));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x01));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x02));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x03));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x04));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x05));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x06));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x07));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x08));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x09));
        sleep_ms(10);
        printf("%08x\n", max2771_read(pio, sm, 0x0A));
        sleep_ms(500);
        // gpio_put(LED_PIN, 0);
        // pio_sm_put_blocking(pio, sm, 0);
        // pio_sm_put_blocking(pio, sm, 0);
        // sleep_ms(500);
    }
}