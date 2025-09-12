#include <core.h>
#include <memory.h>

int main() {
    riscv_cpu_gpr rv_cpu_gpr;  // 实例化通用寄存器
    init_memory();             // 实例化内存
    init_gpr(&rv_cpu_gpr);     // 初始化 CPU 状态: 归零

    pc_change_type change_type;
    change_type.change_type = 1; // 正常跳转


    uint32_t pc = 0x30000000 - 4;  // 当前 pc
    uint32_t pc_next;
    instruction_t inst;  // 指令
    decode_info decode_info;
    execute_result execute_result;
    mem_result mem_result;

    while (1) {
        pc_next = next_pc(pc, &change_type);
        inst = inst_fetch(pc_next);
        inst_decode(inst, &decode_info);
        // 如果顺序执行
        if (change_type.change_type == 1) {
            inst_execute(&decode_info, &execute_result);
            memo_access(&execute_result, &mem_result);
            write_back(&rv_cpu_gpr, &mem_result);
        }
    }

    return 0;
}
