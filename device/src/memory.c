#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t *memory = NULL;  // 模拟的内存区域
#define BIN_FILE ""    //文件名

// 读取 inst.bin 文件到内存
void load_inst_file(const char *filename) {
    FILE *file = fopen(filename, "rb");  // 打开文件，二进制模式
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
    memory = (uint8_t *)malloc(MEMORY_SIZE);
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
    // 将地址 0x30000000 映射到我们分配的内存
    // 地址 0x30000000 应该映射到 memory 的第一个单元
    printf("内存初始化完成，内存大小: %d 字节，起始地址: 0x%p\n", MEMORY_SIZE, memory);

    load_inst_file(BIN_FILE);
}

// MMIO 地址映射，返回内存中的对应地址
void* mmio_map(uint32_t addr) {
    // 如果地址不在映射范围内，返回 NULL
    if (addr < MEM_BASE_ADDR || addr >= MEM_BASE_ADDR + MEMORY_SIZE) {
        return NULL;
    }

    // 返回地址对应的内存指针
    return (void *)(memory + (addr - MEM_BASE_ADDR));
}

// 向内存写入数据
void write_memory(uint32_t addr, uint32_t value) {
    void* mem_ptr = mmio_map(addr);
    if (mem_ptr != NULL) {
        *(uint32_t*)mem_ptr = value;
    } else {
        printf("内存写入失败：地址 0x%x 超出范围\n", addr);
    }
}

// 从内存读取数据
uint32_t read_memory(uint32_t addr) {
    void* mem_ptr = mmio_map(addr);
    if (mem_ptr != NULL) {
        return *(uint32_t*)mem_ptr;
    } else {
        printf("内存读取失败：地址 0x%x 超出范围\n", addr);
        return 0;
    }
}
