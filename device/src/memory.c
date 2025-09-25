#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t* memory = NULL;                                       // 模拟的内存区域
char* bin_file = "/home/luyoung/Xemu/ref/func/obj/main.bin";  // 文件名

// 读取 inst.bin 文件到内存
void load_inst_file(const char* filename) {
    FILE* file = fopen(filename, "rb");  // 打开文件，二进制模式
    if (file == NULL) {
        printf("无法打开文件: %s\n", filename);
        exit(1);
    }
    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);  // 重置文件指针到文件开头

    printf("文件大小: %ld 字节\n", file_size);

    if (file_size > MEMORY_SIZE) {
        printf("文件大小超过内存限制，无法加载！\n");
        fclose(file);
        exit(1);
    }

    // 读取文件内容到内存
    size_t bytes_read = fread(memory, 1, file_size, file);
    if (bytes_read != file_size) {
        printf("文件读取错误！读取的字节数: %zu\n", bytes_read);
        fclose(file);
        exit(1);
    }
    printf("文件内容已成功加载到内存。\n");
    fclose(file);
}

// 内存初始化
void init_memory(void) {
    memory = (uint8_t*)malloc(MEMORY_SIZE);
    if (memory == NULL) {
        printf("内存分配失败！\n");
        exit(1);  // 如果内存分配失败，退出
    }

    // 确保内存的起始地址是 4 字节对齐
    if ((uintptr_t)memory % 4 != 0) {
        printf("内存地址未对齐，分配失败！\n");
        exit(1);
    }

    // 初始化内存为 0
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
    // 将地址 0x1c000000 映射到我们分配的内存
    // 地址 0x1c000000 应该映射到 memory 的第一个单元
    printf("内存初始化完成，内存大小: %d 字节，起始地址: 0x%p\n", MEMORY_SIZE,
           memory);

    load_inst_file(bin_file);
}

// 实现 mmio_map 函数
void* mmio_map(uint32_t addr) {
    // 检查地址是否在模拟内存区域内
    if (addr >= 0x1c000000 && addr < (0x1c000000 + MEMORY_SIZE)) {
        // 如果是内存区域，则返回对应的内存地址
        return &memory[addr - 0x1c000000];  // 计算偏移量并返回地址
    }
    return NULL;
}

#define CR0_ADDR 0xbfaf8000  // 32'hbfaf_8000
#define CR1_ADDR 0xbfaf8010  // 32'hbfaf_8010
#define CR2_ADDR 0xbfaf8020  // 32'hbfaf_8020
#define CR3_ADDR 0xbfaf8030  // 32'hbfaf_8030
#define CR4_ADDR 0xbfaf8040  // 32'hbfaf_8040
#define CR5_ADDR 0xbfaf8050  // 32'hbfaf_8050
#define CR6_ADDR 0xbfaf8060  // 32'hbfaf_8060
#define CR7_ADDR 0xbfaf8070  // 32'hbfaf_8070

#define LED_ADDR 0xbfaff020       // 32'hbfaf_f020
#define LED_RG0_ADDR 0xbfaff030   // 32'hbfaf_f030
#define LED_RG1_ADDR 0xbfaff040   // 32'hbfaf_f040
#define NUM_ADDR 0xbfaff050       // 32'hbfaf_f050
#define SWITCH_ADDR 0xbfaff060    // 32'hbfaf_f060
#define BTN_KEY_ADDR 0xbfaff070   // 32'hbfaf_f070
#define BTN_STEP_ADDR 0xbfaff080  // 32'hbfaf_f080
#define SW_INTER_ADDR 0xbfaff090  // 32'hbfaf_f090
#define TIMER_ADDR 0xbfafe000     // 32'hbfaf_e000

#define IO_SIMU_ADDR 0xbfafff00       // 32'hbfaf_ff00
#define VIRTUAL_UART_ADDR 0xbfafff10  // 32'hbfaf_ff10
#define SIMU_FLAG_ADDR 0xbfafff20     // 32'hbfaf_ff20
#define OPEN_TRACE_ADDR 0xbfafff30    // 32'hbfaf_ff30
#define NUM_MONITOR_ADDR 0xbfafff40   // 32'hbfaf_ff40

// real memo_access
void write_memory(uint32_t addr, uint32_t value) {
    if (addr == LED_RG1_ADDR || addr == LED_RG0_ADDR || addr == LED_ADDR ||
        addr == NUM_ADDR) {
        // 忽略
        return;
    }
    void* mem_ptr = mmio_map(addr);
    if (mem_ptr != NULL) {
        *(uint32_t*)mem_ptr = value;
    } else {
        printf("内存写入失败：地址 0x%x 超出范围\n", addr);
    }
}

uint32_t read_memory(uint32_t addr) {
    void* mem_ptr = mmio_map(addr);
    if (mem_ptr != NULL) {
        printf("inst_data: %08x--->", *(uint32_t*)mem_ptr);
        printf("pc: %08x\n", addr);
        return *(uint32_t*)mem_ptr;
    } else {
        printf("内存读取失败：地址 0x%x 超出范围\n", addr);
        return 0;  // 如果读取失败，返回0
    }
}
