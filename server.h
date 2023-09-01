#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdbool.h>
#include <mongoose.h>

extern struct mg_tcpip_if *s_ifp;

void blink_cb(void *arg);
size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp);
bool usb_up(struct mg_tcpip_if *ifp);

#endif