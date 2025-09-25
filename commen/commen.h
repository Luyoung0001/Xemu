#ifndef COMMEN_H
#define COMMEN_H

#define MEMORY_SIZE 0x100000000
#define MEM_BASE_ADDR 0x1c000000       // 模拟的内存映射起始地址

#define UART_BASE (MEM_BASE_ADDR + 0x01000000)
#define UART_REG_OFFSET 0x00
#endif