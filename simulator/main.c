#include <core.h>
#include <difftest.h>
#include <memory.h>
#include <stdio.h>

void main_init() {
    change_type.change_type = 1;  // 正常跳转
    pc = 0x1bfffffc;
    difftest_init();
}

void exu(uint32_t n) {
    while (n--) {
        next_pc();
        inst = inst_fetch();
        if(inst.word == 0){
            break;
        }
        decode_result = inst_decode();

        if (decode_result != 0) {
            // 未知指令
            printf("Unkown instruction: %08x\n", decode_result);
            break;
        }
        // 如果顺序执行
        if (change_type.change_type == 1) {
            inst_execute();
            memo_access();
            write_back();
            if (!difftest()) {
                break;
            };
        }
    }
}

int main() {
    init_memory();  // 实例化内存
    init_gpr();     // 初始化 CPU 状态: 归零
    main_init();
    exu(-1);
    return 0;
}
