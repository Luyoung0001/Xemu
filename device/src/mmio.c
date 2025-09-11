#include "mmio.h"
#include <stdint.h>

// 定义设备地址
#define UART_BASE 0x10000000
#define VGA_BASE  0x10001000
#define SOUND_BASE 0x10002000

// 模拟MMIO读取
uint32_t mmio_read(uint32_t address) {
    if (address == UART_BASE) {
        // 从UART读取数据
        return read_uart();
    } else if (address == VGA_BASE) {
        // 从VGA读取数据
        return read_vga();
    } else if (address == SOUND_BASE) {
        // 从声卡读取数据
        return read_sound();
    }
    return 0; // 如果没有匹配的设备，返回0
}

// 模拟MMIO写入
void mmio_write(uint32_t address, uint32_t value) {
    if (address == UART_BASE) {
        // 向UART写数据
        write_uart(value);
    } else if (address == VGA_BASE) {
        // 向VGA写数据
        write_vga(value);
    } else if (address == SOUND_BASE) {
        // 向声卡写数据
        write_sound(value);
    }
}
