#ifndef DIFFTEST_H
#define DIFFTEST_H
#include <stdint.h>

typedef struct diff {
    uint32_t we;
    uint32_t pc;
    uint32_t wnum;
    uint32_t value;
} diff;

extern diff* mycpu_trace_info;
extern diff* ref_struct;

void read_ref();
void difftest_init();
int difftest();

#endif