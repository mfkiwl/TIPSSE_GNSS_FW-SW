#include "server.h"
#include <tusb.h>
#include <pico/stdlib.h>
#include "max2771_spi.h"

struct mg_tcpip_if *s_ifp;

const uint8_t tud_network_mac_address[6] = {2, 2, 0x84, 0x6A, 0x96, 0};

void blink_cb(void *arg)
{   // Blink periodically
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

static const char *s_json_header =
    "Content-Type: application/json\r\n"
    "Cache-Control: no-cache\r\n";
static uint64_t s_boot_timestamp = 0;  // Updated by SNTP

// This is for newlib and TLS (mbedTLS)
uint64_t mg_now(void) {
    return mg_millis() + s_boot_timestamp;
}

// SNTP connection event handler. When we get a response from an SNTP server,
// adjust s_boot_timestamp. We'll get a valid time from that point on
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    uint64_t *expiration_time = (uint64_t *) c->data;
    if (ev == MG_EV_OPEN) {
        *expiration_time = mg_millis() + 3000;  // Store expiration time in 3s
    } else if (ev == MG_EV_SNTP_TIME) {
        uint64_t t = *(uint64_t *) ev_data;
        s_boot_timestamp = t - mg_millis();
        c->is_closing = 1;
    } else if (ev == MG_EV_POLL) {
        if (mg_millis() > *expiration_time) c->is_closing = 1;
    }
    (void) fn_data;
}

static void timer_sntp_fn(void *param)
{  // SNTP timer function. Sync up time
    mg_sntp_connect(param, "udp://time.google.com:123", sfn, NULL);
}

// HTTP request handler function
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_http_match_uri(hm, "/read/*")) {
            const char* addr_ptr = mg_strstr(mg_str(hm->uri.ptr+1), mg_str("/"))+1;
            struct mg_str addr_str = mg_str_n(addr_ptr, hm->uri.ptr+hm->uri.len-addr_ptr+1);
            ((char*)addr_str.ptr)[addr_str.len] = '\0';
            uint8_t addr = strtoul(addr_str.ptr, NULL, 16);
            uint32_t value = max2771_read(addr);
            printf("read 0x%02x -> 0x%08x\n", addr, value);
            mg_http_reply(c, 200, "", "%08x", value);
        }
        if (mg_http_match_uri(hm, "/write/*/*")) {
            const char* addr_ptr = mg_strstr(mg_str(hm->uri.ptr+1), mg_str("/"))+1;
            const char* value_ptr = mg_strstr(mg_str(addr_ptr), mg_str("/"))+1;
            struct mg_str addr_str = mg_str_n(addr_ptr, hm->uri.ptr+hm->uri.len-addr_ptr+1);
            struct mg_str value_str = mg_str_n(value_ptr, hm->uri.ptr+hm->uri.len-value_ptr+1);
            ((char*)addr_str.ptr)[addr_str.len] = '\0';
            ((char*)value_str.ptr)[value_str.len] = '\0';
            uint8_t addr = strtoul(addr_str.ptr, NULL, 16);
            uint32_t value = strtoul(value_str.ptr, NULL, 16);
            max2771_write(addr, value);
            uint32_t readback = max2771_read(addr);
            printf("write 0x%02x 0x%08x -> 0x%08x\n", addr, value, readback);
            mg_http_reply(c, 200, "", "%08x", readback);
        }
        else {
            struct mg_http_serve_opts opts;
            memset(&opts, 0, sizeof(opts));
        #if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
            opts.root_dir = "dist";  // On workstations, use filesystem
        #else
            opts.root_dir = "/dist";  // On embedded, use packed files
            opts.fs = &mg_fs_packed;
        #endif
            mg_http_serve_dir(c, ev_data, &opts);
        }
        MG_DEBUG(("%lu %.*s %.*s -> %.*s", c->id, (int) hm->method.len,
            hm->method.ptr, (int) hm->uri.len, hm->uri.ptr, (int) 3,
            &c->send.buf[9]));
    }
    (void) fn_data;
}

void web_init(struct mg_mgr *mgr)
{
    mg_http_listen(mgr, HTTP_URL, fn, NULL);

    // mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_mqtt_fn, c->mgr);
    mg_timer_add(mgr, 3600 * 1000, MG_TIMER_RUN_NOW | MG_TIMER_REPEAT,
        timer_sntp_fn, mgr);
}