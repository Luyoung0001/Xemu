#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 1024 * 1024 * 128  // 128MB内存
#define MEM_BASE_ADDR 0x30000000       // 模拟的内存映射起始地址




// 内存管理函数声明

extern uint8_t *memory;
void load_inst_file(const char *filename);
void init_memory(void);
void* mmio_map(uint32_t addr);
void write_memory(uint32_t addr, uint32_t value);
uint32_t read_memory(uint32_t addr);

#endif
