#include "max2771_spi.h"
#include "max2771_spi.pio.h"

const uint CSN_PIN = 0;
const uint SCLK_PIN = 1;
const uint SDATA_PIN = 2;

static PIO spi_pio;
static uint spi_sm;

void max2771_spi_program_init(PIO pio)
{
    spi_pio = pio;

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    spi_sm = pio_claim_unused_sm(spi_pio, true);

    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(spi_pio, &max2771_spi_program);

    pio_sm_config cfg = max2771_spi_program_get_default_config(offset);

    sm_config_set_clkdiv(&cfg, 20);

    sm_config_set_set_pins(&cfg, CSN_PIN, 3);
    sm_config_set_sideset_pins(&cfg, SCLK_PIN);
    sm_config_set_out_pins(&cfg, SDATA_PIN, 1);
    sm_config_set_in_pins(&cfg, SDATA_PIN);
    sm_config_set_jmp_pin(&cfg, SDATA_PIN);

    sm_config_set_out_shift(&cfg, false, false, 32);
    sm_config_set_in_shift(&cfg, false, false, 32);

    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(spi_pio, CSN_PIN);
    pio_gpio_init(spi_pio, SCLK_PIN);
    pio_gpio_init(spi_pio, SDATA_PIN);

    // Set the pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(spi_pio, spi_sm, CSN_PIN, 3, true);

    // SPI is synchronous, so bypass input synchroniser to reduce input delay.
    hw_set_bits(&spi_pio->input_sync_bypass, 1u << SDATA_PIN);

    // Load our configuration, and jump to the start of the program
    pio_sm_init(spi_pio, spi_sm, offset, &cfg);
    // Set the state machine running
    pio_sm_set_enabled(spi_pio, spi_sm, true);
}

uint32_t max2771_read(uint32_t addr)
{
    pio_sm_put_blocking(spi_pio, spi_sm, 0x000FFFFF | addr << 20);
    return pio_sm_get_blocking(spi_pio, spi_sm);
}

void max2771_write(uint32_t addr, uint32_t val)
{
    pio_sm_put_blocking(spi_pio, spi_sm, addr << 20);
    pio_sm_put_blocking(spi_pio, spi_sm, val);
}
