#ifndef MAX2771_H
#define MAX2771_H

typedef struct
{
    unsigned FGAIN      : 1;
    unsigned FCENX      : 1;
    unsigned F3OR5      : 1;
    unsigned FBW        : 3;
    unsigned FCEN       : 7;
    unsigned MIXERMODE  : 2;
    unsigned LNAMODE    : 2;
    unsigned MIXPOLE    : 1;
    unsigned RESERVED1  : 2;
    unsigned RESERVED2  : 2;
    unsigned RESERVED3  : 4;
    unsigned RESERVED4  : 4;
    unsigned IDLE       : 1;
    unsigned CHIPEN     : 1;
} MAX2771_CFG1;

typedef struct
{
    unsigned DIEID           : 2;
    unsigned RESERVED1       : 1;
    unsigned RESERVED2       : 1;
    unsigned DRVCFG          : 2;
    unsigned BITS            : 3;
    unsigned FORMAT          : 2;
    unsigned AGCMODE         : 2;
    unsigned SPI_SDIO_CONFIG : 2;
    unsigned GAINREF         : 4;
    unsigned IQEN            : 1;
    unsigned ANAIMON         : 1;
    unsigned RESERVED3       : 2;
    unsigned RESERVED4       : 1;
} MAX2771_CFG2;

typedef struct
{
    unsigned STRMRST    : 1;
    unsigned DATASYNCEN : 1;
    unsigned TIMESYNCEN : 1;
    unsigned STAMPEN    : 1;
    unsigned STRMBITS   : 2;
    unsigned RESERVED1  : 3;
    unsigned STRMSTOP   : 1;
    unsigned STRMSTART  : 1;
    unsigned STRMEN     : 1;
    unsigned PGAQEN     : 1;
    unsigned PGAIEN     : 1;
    unsigned RESERVED2  : 1;
    unsigned FHIPEN     : 1;
    unsigned RESERVED3  : 1;
    unsigned RESERVED4  : 1;
    unsigned RESERVED5  : 1;
    unsigned RESERVED6  : 1;
    unsigned HILOADEN   : 1;
    unsigned RESERVED7  : 1;
    unsigned GAININ     : 6;
    unsigned RESERVED8  : 4;
} MAX2771_CFG3;

#endif