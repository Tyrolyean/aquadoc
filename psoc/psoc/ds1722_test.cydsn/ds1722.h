
#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
#include <stdbool.h>

uint8_t get_ds1722_address(bool read, uint8_t address);
static inline void spi_fifo_empty_interrupt();

#endif /* DS1722_H */