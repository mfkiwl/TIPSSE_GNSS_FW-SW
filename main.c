#include <stdio.h>
#include <stdint.h>
#include <pico/stdlib.h>
#include <pico/bit_ops.h>
#include <hardware/pio.h>
#include <mongoose.h>
#include <tusb.h>
#include "max2771.h"
#include "max2771_spi.h"
#include "server.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

int main()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    max2771_spi_program_init(pio0);

    struct mg_mgr mgr;  // Initialise Mongoose event manager
    mg_mgr_init(&mgr);  // and attach it to the interface
    mg_timer_add(&mgr, 500, MG_TIMER_REPEAT, blink_cb, &mgr);

    struct mg_tcpip_driver driver = {.tx = usb_tx, .up = usb_up};
    struct mg_tcpip_if mif = {
        .mac = {2, 0, 1, 2, 3, 0x77},
        .ip = mg_htonl(MG_U32(192, 168, 3, 1)),
        .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
        .enable_dhcp_server = true,
        .driver = &driver,
        .recv_queue.size = 4096
    };
    s_ifp = &mif;
    mg_tcpip_init(&mgr, &mif);
    tusb_init();

    stdio_init_all();

    MG_INFO(("Initialising application..."));
    web_init(&mgr);

    MG_INFO(("Starting event loop"));
    for (;;) {
        mg_mgr_poll(&mgr, 0);
        tud_task();
    }

    while (true) {
        printf("=========================\n");
        printf("MAX2771 registers:\n");

        printf("=========================\n");
        uint32_t cfg1_val = max2771_read(0x00);
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
        uint32_t cfg2_val = max2771_read(0x01);
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
        uint32_t cfg3_val = max2771_read(0x02);
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
        uint32_t pllcfg_val = max2771_read(0x03);
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
        uint32_t pllintdiv_val = max2771_read(0x04);
        printf("PLLINTDIV: 0x%08x\n", pllintdiv_val);
        MAX2771_PLLINTDIV pllintdiv = *(MAX2771_PLLINTDIV*)&pllintdiv_val;
        printf("RDIV:   0x%x\n", pllintdiv.RDIV);
        printf("NDIV:   0x%x\n", pllintdiv.NDIV);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t pllfracdiv_val = max2771_read(0x05);
        printf("PLLFRACDIV: 0x%08x\n", pllfracdiv_val);
        MAX2771_PLLFRACDIV pllfracdiv = *(MAX2771_PLLFRACDIV*)&pllfracdiv_val;
        printf("FDIV:   0x%x\n", pllfracdiv.FDIV);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t reserved_val = max2771_read(0x06);
        printf("RESERVED: 0x%08x\n", reserved_val);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t clkcfg1_val = max2771_read(0x07);
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
        uint32_t test1_val = max2771_read(0x08);
        printf("TEST1: 0x%08x\n", test1_val);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t test2_val = max2771_read(0x09);
        printf("TEST2: 0x%08x\n", test2_val);
        sleep_ms(10);

        printf("=========================\n");
        uint32_t clkcfg2_val = max2771_read(0x0A);
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