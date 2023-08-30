#pragma once

// Windows only supports RNDIS, and apparently doesn't like composite devices with two configurations.
// Mac only supports CDC-ECM, which we present as the second config in a dual configuration device
// Linux supports either RNDIS, CDC-ECM or CDC-NCM
#define CFG_TUD_ECM_RNDIS         1
#define CFG_TUD_MSC               1     // This enables mass storage
#define CFG_TUD_MSC_EP_BUFSIZE    512   // Also Add msc_disk.c and descritors
#define CFG_TUD_NCM               0
#define BOARD_DEVICE_RHPORT_NUM   0
#define BOARD_DEVICE_RHPORT_SPEED OPT_MODE_FULL_SPEED
#define CFG_TUSB_RHPORT0_MODE     (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)

// VENDOR sub-class for the reset interface
#define RESET_INTERFACE_SUBCLASS 0x00
// VENDOR protocol for the reset interface
#define RESET_INTERFACE_PROTOCOL 0x01

#define USBD_MAX_POWER_MA (250)

#define CFG_TUD_CDC             (1)
#define CFG_TUD_CDC_RX_BUFSIZE  (256)
#define CFG_TUD_CDC_TX_BUFSIZE  (256)

// We use a vendor specific interface but with our own driver
#define CFG_TUD_VENDOR            (0)