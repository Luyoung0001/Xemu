#include <core.h>
#include <memory.h>
#include <stdio.h>

pc_change_type change_type;
riscv_cpu_gpr rv_cpu_gpr;      // 实例化通用寄存器
uint32_t pc;  // 当前 pc
uint32_t pc_next;
instruction_t inst;  // 指令
decode_info de_info;
uint32_t decode_result;
execute_result exe_resul;
mem_result mem_resul;

void main_init() {
    change_type.change_type = 1;  // 正常跳转
    pc = 0x30000000 - 4;
}

void exu(uint32_t n) {
    while (n--) {
        pc_next = next_pc(pc, &change_type);
        inst = inst_fetch(pc_next);
        decode_result = inst_decode(inst, &de_info,&rv_cpu_gpr);
        if (decode_result != 0){
            // 未知指令
            printf("Unkown instruction: %08x\n", decode_result);
            break;
        }
        // 如果顺序执行
        if (change_type.change_type == 1) {
            inst_execute(&de_info, &exe_resul);
            memo_access(&exe_resul, &mem_resul);
            write_back(&rv_cpu_gpr, &mem_resul);
        }
    }
}

int main() {
    init_memory();          // 实例化内存
    init_gpr(&rv_cpu_gpr);  // 初始化 CPU 状态: 归零
    main_init();
    exu(-1);
    return 0;
}
