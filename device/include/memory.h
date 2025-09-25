#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <commen.h>


// 内存管理函数声明
extern uint8_t *memory;
void load_inst_file(const char *filename);
void init_memory(void);
uint8_t* mmio_map(uint32_t addr);
void write_memory(uint32_t addr, uint32_t value);
uint32_t read_memory(uint32_t addr);

#endif
