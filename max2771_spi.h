#ifndef MAX2771_SPI_H
#define MAX2771_SPI_H

#include <pico/stdlib.h>
#include <hardware/pio.h>

void max2771_spi_program_init(PIO pio, uint sm);
uint32_t max2771_read(PIO pio, uint sm, uint32_t addr);
void max2771_write(PIO pio, uint sm, uint32_t addr, uint32_t val);

#endif