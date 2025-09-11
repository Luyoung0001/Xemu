#include <serial.h>
#include <stdio.h>

// 模拟的 serial_putc() 系统调用，模拟结束，这里使用系统调用来借助 host 进行输出
void serial_putc(char c) {
    putchar(c);
}
