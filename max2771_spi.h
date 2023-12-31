#ifndef MAX2771_SPI_H
#define MAX2771_SPI_H

#include <pico/stdlib.h>
#include <hardware/pio.h>

void max2771_spi_program_init(PIO pio);
uint32_t max2771_read(uint8_t addr);
void max2771_write(uint8_t addr, uint32_t val);

#endif