#include <difftest.h>
#include <stdio.h>
#include <stdlib.h>

// golden_ref_path
char* golden_ref_path = "/home/luyoung/Xemu/ref/gettrace/golden_trace.txt";

FILE* fp;

diff* mycpu_trace_info = NULL;
diff* ref_struct = NULL;

void op_file() {
    fp = fopen(golden_ref_path, "r");
}

void read_ref() {
    if (ref_struct == NULL) {
        printf("Error: ref_struct is NULL\n");
        return;
    }
    if (fp == NULL) {
        printf("Error: File pointer is NULL\n");
        return;
    }

    // 使用 fscanf 读取文件内容
    if (fscanf(fp, "%x%x%x%x", &(ref_struct->we), &(ref_struct->pc),
           &(ref_struct->wnum), &(ref_struct->value)) != 4) {
        printf("Error reading from file\n");
        return;
    }
}

void print_info() {
    printf("-->CPU %d %08x %02x %08x\n", mycpu_trace_info->we,
           mycpu_trace_info->pc, mycpu_trace_info->wnum,
           mycpu_trace_info->value);
    printf("-->REF %d %08x %02x %08x\n\n\n", ref_struct->we, ref_struct->pc,
           ref_struct->wnum, ref_struct->value);
}

void difftest_init() {
    // 绑定对象
    mycpu_trace_info = (diff*)malloc(sizeof(diff));
    ref_struct = (diff*)malloc(sizeof(diff));
    op_file();
}

int difftest() {
    // 每次调用，就读取一行
    read_ref();
    int good = (ref_struct->we == mycpu_trace_info->we &&
                ref_struct->wnum == mycpu_trace_info->wnum &&
                ref_struct->pc == mycpu_trace_info->pc &&
                ref_struct->value == mycpu_trace_info->value);

    if (!good) {
        print_info();
    }
    return good;
}
