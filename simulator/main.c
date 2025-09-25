#include <core.h>
#include <difftest.h>
#include <memory.h>
#include <stdio.h>

void main_init() {
    change_type.change_type = 1;  // 正常跳转
    pc = 0x1bfffffc;
    difftest_init();
}
uint64_t inst_count = 0;
void exu(uint32_t n) {
    while (n--) {
        inst_count++;
        printf("%luth\n", inst_count);
        next_pc();
        inst = inst_fetch();
        if (inst.word == 0) {
            break;
        }
        decode_result = inst_decode();

        if (decode_result != 0) {
            // 未知指令
            printf("Unkown instruction: %08x\n", decode_result);
            break;
        }
        // 如果顺序执行
        inst_execute();
        memo_access();
        write_back();
        if (!difftest()) {
            printf("Oops, wrong!!!\n");
            break;
        };
        if (pc == 0x1c000100) {
            printf("\033[32mPassed all the tests!\033[0m\n");
            break;
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
