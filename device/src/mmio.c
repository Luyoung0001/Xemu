#include <memory.h>
#include <mmio.h>




// 模拟 MMIO 读操作
uint32_t mmio_read(uint32_t address) {
    // 目前只有数据或者地址
    return read_memory(address);
}

// 重定向到不同的地址
void mmio_write(uint32_t address, uint32_t value) {
    if (address == UART_BASE) {
        write_uart(value);
    } else {
        write_memory(address, value);
    }
}
