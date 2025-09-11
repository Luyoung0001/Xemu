#include <core.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>

// 初始化 CPU 寄存器和程序计数器
void init_gpr(riscv_cpu_gpr* cpu) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu->regs[i] = 0;  // 初始化寄存器为0
    }
}

// pre_IFU 对指令的 PC 进行一个处理

uint32_t next_pc(uint32_t pc, pc_change_type* change_type) {
    // 根据情况来判断 next_pc 的值
    switch (change_type->change_type) {
        case 1:
            return pc + 4;
            break;
        default:
            return pc + 4;
    }
    return pc + 4;
}

// 取指
instruction_t inst_fetch(uint32_t pc) {
    // 返回指令字
    uint32_t read_result = read_memory(pc);
    return (instruction_t)read_result;
}

// 译码
void inst_decode(instruction_t instr, decode_info* dc_info) {
    // 这里进行译码
    // 得到以下信息:
    // 1、指令类型
    // 2、两个寄存器访问地址
    // 3、操作类型：alu or what
    // 4、如果是访存：访存类型
    // 等等信息保存在全局变量中，以供后续单元使用？
    // 还是创建一个结构体，直接传送到inst_exexute?
    // 这里为了反应真实 CPU情况，应该创建一个结构体进行传送
    // 尽量避免全局变量
}
// 执行
void inst_execute(decode_info* dc_info, execute_result* exe_re) {}

// 访存
void memo_access(execute_result* exe_result, mem_result* mem_re) {}

// 回写
void write_back(riscv_cpu_gpr* cpu, mem_result* mem_re) {}
