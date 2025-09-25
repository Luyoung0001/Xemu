#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define RESET_VECTOR 0x1c000000

uint8_t* memory = NULL;
char* bin_file = "/home/luyoung/Xemu/ref/func/obj/main.bin";  // 文件名


// 地址转换
uint8_t* padd2host(uint32_t paddr) {
    return memory + paddr;
}

uint32_t pmem_read(uint8_t* paddr) {
    uintptr_t addr_val = (uintptr_t)paddr;  // 将指针转换为整数类型
    uintptr_t aligned_addr_val =
        addr_val & ~3;  // 确保地址是 4 字节对齐的起始地址
    uint8_t* aligned_addr =
        (uint8_t*)aligned_addr_val;  // 将整数类型转换回指针类型

    return *((uint32_t*)aligned_addr);
}

void pmem_write(uint8_t* paddr, uint32_t data) {
    uintptr_t addr_val = (uintptr_t)paddr;  // 将指针转换为整数类型
    uintptr_t aligned_addr_val =
        addr_val & ~3;  // 确保地址是 4 字节对齐的起始地址
    uint8_t* aligned_addr =
        (uint8_t*)aligned_addr_val;  // 将整数类型转换回指针类型
    *((uint32_t*)aligned_addr) = data;
}

uint32_t paddr_read(uint32_t paddr) {
    return pmem_read(padd2host(paddr));
}

void paddr_write(uint32_t paddr, uint32_t data) {
    pmem_write(padd2host(paddr), data);
}

// 读取 inst.bin 文件到内存
void load_inst_file(const char* filename) {
    FILE* fp = fopen(bin_file, "rb");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    printf("The image is %s, size = %ld\n", bin_file, size);

    fseek(fp, 0, SEEK_SET);

    // 装载 bin 到仿真环境，可以是 sram、mrom、flash
    fread(padd2host(RESET_VECTOR), size, 1, fp);

    fclose(fp);
}

// 内存初始化
void init_memory(void) {
    memory = (uint8_t*)malloc(MEMORY_SIZE);

    printf("PMEM_ADDR:%ld\n", (uint64_t)memory);
    printf("PMEM_END:%ld\n", (uint64_t)memory + MEMORY_SIZE);
    if (memory == NULL) {
        fprintf(stderr, "内存分配失败！\n");
        exit(1);
    }

    load_inst_file(bin_file);
}

// 实现 mmio_map 函数
uint8_t* mmio_map(uint32_t addr) {
    // 检查地址是否在模拟内存区域内
    if ((uint64_t)memory + (uint64_t)addr <=
        (uint64_t)memory + (uint64_t)MEMORY_SIZE) {
        return (uint8_t*)((uint64_t)memory + (uint64_t)addr);
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
        addr == NUM_ADDR || addr == SW_INTER_ADDR) {
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
    if (addr == SW_INTER_ADDR) {
        // 忽略
        return 0x0000aaaa;
    }
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
