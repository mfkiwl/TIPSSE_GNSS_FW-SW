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

typedef struct
{
    unsigned RESERVED1  : 1;
    unsigned RESERVED2  : 1;
    unsigned PWRSAV     : 1;
    unsigned INT_PLL    : 1;
    unsigned RESERVED3  : 3;
    unsigned RESERVED4  : 1;
    unsigned RESERVED5  : 1;
    unsigned ICP        : 1;
    unsigned RESERVED6  : 3;
    unsigned RESERVED7  : 1;
    unsigned RESERVED8  : 5;
    unsigned IXTAL      : 2;
    unsigned RESERVED9  : 2;
    unsigned RESERVED10 : 1;
    unsigned REFOUTEN   : 1;
    unsigned RESERVED11 : 1;
    unsigned RESERVED12 : 1;
    unsigned RESERVED13 : 1;
    unsigned LOBAND     : 1;
    unsigned REFDIV     : 3;
} MAX2771_PLLCFG;

typedef struct
{
    unsigned RESERVED1  : 3;
    unsigned RDIV       : 10;
    unsigned NDIV       : 15;
    unsigned RESERVED2  : 4;
} MAX2771_PLLINTDIV;

typedef struct
{
    unsigned RESERVED1  : 1;
    unsigned RESERVED2  : 1;
    unsigned RESERVED3  : 1;
    unsigned RESERVED4  : 1;
    unsigned RESERVED5  : 4;
    unsigned FDIV       : 20;
    unsigned RESERVED6  : 4;
} MAX2771_PLLFRACDIV;

typedef struct
{
    unsigned RESERVED1  : 28;
    unsigned RESERVED2  : 4;
} MAX2771_RESERVED;

typedef struct
{
    unsigned RESERVED1  : 1;
    unsigned RESERVED2  : 1;
    unsigned RESERVED3  : 1;
    unsigned RESERVED4  : 1;
    unsigned RESERVED5  : 1;
    unsigned RESERVED6  : 1;
    unsigned RESERVED7  : 5;
    unsigned RESERVED8  : 1;
    unsigned RESERVED9  : 4;
    unsigned RESERVED10 : 4;
    unsigned RESERVED11 : 4;
    unsigned RESERVED12 : 4;
    unsigned RESERVED13 : 2;
    unsigned RESERVED14 : 1;
    unsigned RESERVED15 : 1;
} MAX2771_TEST1;

typedef struct
{
    unsigned RESERVED1  : 1;
    unsigned RESERVED2  : 1;
    unsigned RESERVED3  : 1;
    unsigned RESERVED4  : 5;
    unsigned RESERVED5  : 5;
    unsigned RESERVED6  : 1;
    unsigned RESERVED7  : 1;
    unsigned RESERVED8  : 1;
    unsigned RESERVED9  : 1;
    unsigned RESERVED10 : 1;
    unsigned RESERVED11 : 2;
    unsigned RESERVED12 : 1;
    unsigned RESERVED13 : 1;
    unsigned RESERVED14 : 3;
    unsigned RESERVED15 : 4;
    unsigned RD_CALC : 1;
    unsigned RESERVED16 : 1;
    unsigned RESERVED17 : 1;
} MAX2771_TEST2;

typedef struct
{
    unsigned MODE           : 1;
    unsigned RESERVED1      : 1;
    unsigned ADCCLK         : 1;
    unsigned FCLKIN         : 1;
    unsigned REFCLK_M_CNT   : 12;
    unsigned REFCLK_L_CNT   : 12;
    unsigned EXTADCCLK      : 1;
    unsigned RESERVED2      : 3;
} MAX2771_CLKCFG1;

typedef struct
{
    unsigned RESERVED1      : 2;
    unsigned CLKOUT_SEL     : 1;
    unsigned PREFRACDIV_SEL : 1;
    unsigned ADCCLK_M_CNT   : 12;
    unsigned ADCCLK_L_CNT   : 12;
    unsigned RESERVED2      : 1;
    unsigned RESERVED3      : 3;
} MAX2771_CLKCFG2;

#endif