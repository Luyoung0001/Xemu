#include <serial.h>
#include <mmio.h>
#include <stdint.h>


// 串口输出
void write_uart(char value) {
    printf("%c", value); // 调用系统调用模拟串口输出
}