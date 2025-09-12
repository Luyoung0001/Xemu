#include <lib.h>
#include <commen.h>
#include <stdint.h>

void serial_putc(char value) {
    volatile uint8_t *uart_reg = (volatile uint8_t *)(UART_BASE + UART_REG_OFFSET);
    *uart_reg = value;  // 这里的赋值会被编译器优化为 st.b 指令
}