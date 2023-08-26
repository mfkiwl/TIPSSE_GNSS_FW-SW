#include <stdio.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <pico/bit_ops.h>
#include <hardware/pio.h>
#include "max2771_spi.pio.h"
#include "max2771.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint CSN_PIN = 0;
const uint SCLK_PIN = 1;
const uint SDATA_PIN = 2;

static inline void max2771_spi_program_init(PIO pio, uint sm)
{
    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(pio, &max2771_spi_program);

    pio_sm_config cfg = max2771_spi_program_get_default_config(offset);

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

    max2771_spi_program_init(pio, sm);

    stdio_init_all();
    while (true) {
        printf("=========================\n");
        printf("MAX2771 registers:\n");

        printf("=========================\n");
        uint32_t cfg1_val = max2771_read(pio, sm, 0x00);
        printf("CFG1: 0x%08x\n", cfg1_val);
        MAX2771_CFG1 cfg1 = *(MAX2771_CFG1*)&cfg1_val;
        printf("FGAIN:     0x%x\n", cfg1.FGAIN);
        printf("FCENX:     0x%x\n", cfg1.FCENX);
        printf("F3OR5:     0x%x\n", cfg1.F3OR5);
        printf("FBW:       0x%x\n", cfg1.FBW);
        printf("FCEN:      0x%x\n", cfg1.FCEN);
        printf("MIXERMODE: 0x%x\n", cfg1.MIXERMODE);
        printf("LNAMODE:   0x%x\n", cfg1.LNAMODE);
        printf("MIXPOLE:   0x%x\n", cfg1.MIXPOLE);
        printf("IDLE:      0x%x\n", cfg1.IDLE);
        printf("CHIPEN:    0x%x\n", cfg1.CHIPEN);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t cfg2_val = max2771_read(pio, sm, 0x01);
        printf("CFG2: 0x%08x\n", cfg2_val);
        MAX2771_CFG2 cfg2 = *(MAX2771_CFG2*)&cfg2_val;
        printf("DIEID:           0x%x\n", cfg2.DIEID);
        printf("DRVCFG:          0x%x\n", cfg2.DRVCFG);
        printf("BITS:            0x%x\n", cfg2.BITS);
        printf("FORMAT:          0x%x\n", cfg2.FORMAT);
        printf("AGCMODE:         0x%x\n", cfg2.AGCMODE);
        printf("SPI_SDIO_CONFIG: 0x%x\n", cfg2.SPI_SDIO_CONFIG);
        printf("GAINREF:         0x%x\n", cfg2.GAINREF);
        printf("IQEN:            0x%x\n", cfg2.IQEN);
        printf("ANAIMON:         0x%x\n", cfg2.ANAIMON);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t cfg3_val = max2771_read(pio, sm, 0x02);
        printf("CFG3: 0x%08x\n", cfg3_val);
        MAX2771_CFG3 cfg3 = *(MAX2771_CFG3*)&cfg3_val;
        printf("STRMRST:       0x%x\n", cfg3.STRMRST);
        printf("DATASYNCEN:    0x%x\n", cfg3.DATASYNCEN);
        printf("TIMESYNCEN:    0x%x\n", cfg3.TIMESYNCEN);
        printf("STAMPEN:       0x%x\n", cfg3.STAMPEN);
        printf("STRMBITS:      0x%x\n", cfg3.STRMBITS);
        printf("STRMSTOP:      0x%x\n", cfg3.STRMSTOP);
        printf("STRMSTART:     0x%x\n", cfg3.STRMSTART);
        printf("STRMEN:        0x%x\n", cfg3.STRMEN);
        printf("PGAQEN:        0x%x\n", cfg3.PGAQEN);
        printf("PGAIEN:        0x%x\n", cfg3.PGAIEN);
        printf("FHIPEN:        0x%x\n", cfg3.FHIPEN);
        printf("HILOADEN:      0x%x\n", cfg3.HILOADEN);
        printf("GAININ:        0x%x\n", cfg3.GAININ);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t pllcfg_val = max2771_read(pio, sm, 0x03);
        printf("PLLCFG: 0x%08x\n", pllcfg_val);
        MAX2771_PLLCFG pllcfg = *(MAX2771_PLLCFG*)&pllcfg_val;
        printf("PWRSAV:     0x%x\n", pllcfg.PWRSAV);
        printf("INT_PLL:    0x%x\n", pllcfg.INT_PLL);
        printf("ICP:        0x%x\n", pllcfg.ICP);
        printf("IXTAL:      0x%x\n", pllcfg.IXTAL);
        printf("REFOUTEN:   0x%x\n", pllcfg.REFOUTEN);
        printf("LOBAND:     0x%x\n", pllcfg.LOBAND);
        printf("REFDIV:     0x%x\n", pllcfg.REFDIV);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t pllintdiv_val = max2771_read(pio, sm, 0x04);
        printf("PLLINTDIV: 0x%08x\n", pllintdiv_val);
        MAX2771_PLLINTDIV pllintdiv = *(MAX2771_PLLINTDIV*)&pllintdiv_val;
        printf("RDIV:   0x%x\n", pllintdiv.RDIV);
        printf("NDIV:   0x%x\n", pllintdiv.NDIV);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t pllfracdiv_val = max2771_read(pio, sm, 0x05);
        printf("PLLFRACDIV: 0x%08x\n", pllfracdiv_val);
        MAX2771_PLLFRACDIV pllfracdiv = *(MAX2771_PLLFRACDIV*)&pllfracdiv_val;
        printf("FDIV:   0x%x\n", pllfracdiv.FDIV);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t reserved_val = max2771_read(pio, sm, 0x06);
        printf("RESERVED: 0x%08x\n", reserved_val);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t clkcfg1_val = max2771_read(pio, sm, 0x07);
        printf("CLKCFG1: 0x%08x\n", clkcfg1_val);
        MAX2771_CLKCFG1 clkcfg1 = *(MAX2771_CLKCFG1*)&clkcfg1_val;
        printf("MODE:           0x%x\n", clkcfg1.MODE);
        printf("ADCCLK:         0x%x\n", clkcfg1.ADCCLK);
        printf("FCLKIN:         0x%x\n", clkcfg1.FCLKIN);
        printf("REFCLK_M_CNT:   0x%x\n", clkcfg1.REFCLK_M_CNT);
        printf("REFCLK_L_CNT:   0x%x\n", clkcfg1.REFCLK_L_CNT);
        printf("EXTADCCLK:      0x%x\n", clkcfg1.EXTADCCLK);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t test1_val = max2771_read(pio, sm, 0x08);
        printf("TEST1: 0x%08x\n", test1_val);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t test2_val = max2771_read(pio, sm, 0x09);
        printf("TEST2: 0x%08x\n", test2_val);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t clkcfg2_val = max2771_read(pio, sm, 0x0A);
        printf("CLKCFG1: 0x%08x\n", clkcfg2_val);
        MAX2771_CLKCFG2 clkcfg2 = *(MAX2771_CLKCFG2*)&clkcfg2_val;
        printf("CLKOUT_SEL:     0x%x\n", clkcfg2.CLKOUT_SEL);
        printf("PREFRACDIV_SEL: 0x%x\n", clkcfg2.PREFRACDIV_SEL);
        printf("ADCCLK_M_CNT:   0x%x\n", clkcfg2.ADCCLK_M_CNT);
        printf("ADCCLK_L_CNT:   0x%x\n", clkcfg2.ADCCLK_L_CNT);
        sleep_ms(10);

        printf("=========================\n\n");
        getchar();
    }
}