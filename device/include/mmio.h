#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>
#include <serial.h>
#include <stdio.h>

void write_uart(char value);
uint32_t mmio_read(uint32_t address);
void mmio_write(uint32_t address, uint32_t value);

#endif