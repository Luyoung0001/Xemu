#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#define NUM_REGISTERS 32  // 寄存器数量

// 寄存器结构体
typedef struct {
    uint32_t regs[NUM_REGISTERS];  // 寄存器数组
} riscv_cpu_gpr;

// pc 跳转信号
// jump: change_type = 1
// ... change_type = ...
typedef struct {
    uint8_t change_type;
} pc_change_type;

// 译码后的数据结构体
typedef struct {
    uint32_t inst_op_type;  // 操作类型
    uint8_t reg1_addr;
    uint8_t reg2_addr;
    uint8_t wr_reg_addr;  // 寄存器写回地址
    uint8_t wr_reg;       // 是否回写
} decode_info;

// 执行指令以后的结果
typedef struct {
    uint8_t exu_type;
    uint8_t wr_reg_addr;
    uint32_t alu_result;
    uint32_t mult_dev_result;
    uint32_t load_or_store_addr;
    uint8_t wr_reg;  // 是否回写

} execute_result;

// 访存的结果
typedef struct {
    uint8_t wr_reg_addr;
    uint8_t wr_reg;  // 是否回写
    uint32_t alu_result;
    uint32_t mult_dev_result;
    uint32_t load_or_store_addr;
} mem_result;

// 指令结构体
typedef union {
    uint32_t word;
    struct {
        uint32_t imm : 12;
        uint32_t rs1 : 5;
        uint32_t rs2 : 5;
        uint32_t funct3 : 3;
        uint32_t rd : 5;
        uint32_t opcode : 7;
    } decode;  // 只用于举例，具体会根据指令格式扩展
} instruction_t;

// 函数声明
void init_gpr(riscv_cpu_gpr* cpu);
uint32_t next_pc(uint32_t pc, pc_change_type* change_type);
instruction_t inst_fetch(uint32_t pc);
void inst_decode(instruction_t instr, decode_info *dc_info);
void inst_execute(decode_info* dc_info, execute_result* exe_re);
void memo_access(execute_result * exe_result, mem_result *mem_re);
void write_back(riscv_cpu_gpr *cpu, mem_result *mem_re);

#endif
