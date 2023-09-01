#include "server.h"
#include <tusb.h>
#include <pico/stdlib.h>

struct mg_tcpip_if *s_ifp;

const uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

void blink_cb(void *arg)
{   // Blink periodically
    // uint32_t cfg1_val = max2771_read(pio, sm, 0x00);
    // printf("CFG1: 0x%08x\n", cfg1_val);
    // max2771_write(pio, sm, 0x00, 0xBEA41603);

    gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get_out_level(PICO_DEFAULT_LED_PIN));
    (void) arg;
}

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len)
{
    mg_tcpip_qwrite((void *) buf, len, s_ifp);
    // MG_INFO(("RECV %hu", len));
    // mg_hexdump(buf, len);
    tud_network_recv_renew();
    return true;
}

void tud_network_init_cb(void) {}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg)
{
    // MG_INFO(("SEND %hu", arg));
    memcpy(dst, ref, arg);
    return arg;
}

size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp)
{
    if (!tud_ready()) return 0;
    while (!tud_network_can_xmit(len)) tud_task();
    tud_network_xmit((void *) buf, len);
    (void) ifp;
    return len;
}

bool usb_up(struct mg_tcpip_if *ifp)
{
    (void) ifp;
    return tud_inited() && tud_ready() && tud_connected();
}