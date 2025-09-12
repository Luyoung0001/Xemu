#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#define NUM_REGISTERS 32  // 寄存器数量

uint8_t inst_add_w;
uint8_t inst_sub_w;
uint8_t inst_slt;
uint8_t inst_slti;
uint8_t inst_sltu;
uint8_t inst_sltui;
uint8_t inst_nor;
uint8_t inst_and;
uint8_t inst_andi;
uint8_t inst_or;
uint8_t inst_ori;
uint8_t inst_xor;
uint8_t inst_xori;
uint8_t inst_sll;
uint8_t inst_slli_w;
uint8_t inst_srl;
uint8_t inst_srli_w;
uint8_t inst_sra;
uint8_t inst_srai_w;
uint8_t inst_addi_w;
uint8_t inst_ld_w;
uint8_t inst_ld_b;
uint8_t inst_ld_bu;
uint8_t inst_ld_h;
uint8_t inst_ld_hu;
uint8_t inst_st_w;
uint8_t inst_st_b;
uint8_t inst_st_h;
uint8_t inst_jirl;
uint8_t inst_b;
uint8_t inst_bl;
uint8_t inst_blt;
uint8_t inst_bltu;
uint8_t inst_beq;
uint8_t inst_bne;
uint8_t inst_bge;
uint8_t inst_bgeu;
uint8_t inst_lu12i_w;
uint8_t inst_pcaddu12i;

uint8_t inst_mul_w;
uint8_t inst_mulh_w;
uint8_t inst_mulh_wu;
uint8_t inst_div_w;
uint8_t inst_div_wu;
uint8_t inst_mod_w;
uint8_t inst_mod_wu;

uint8_t inst_csrrd;
uint8_t inst_csrwr;
uint8_t inst_csrxchg;
uint8_t inst_ertn;

uint8_t inst_syscall;
uint8_t inst_break;

uint8_t inst_rdcntid_w;
uint8_t inst_rdcntvl_w;
uint8_t inst_rdcntvh_w;
uint8_t inst_zero;

uint8_t inst_tlbsrch;
uint8_t inst_tlbrd;
uint8_t inst_tlbwr;
uint8_t inst_tlbfill;
uint8_t inst_invtlb;

uint8_t need_ui5;
uint8_t need_si12;
uint8_t need_si14;
uint8_t need_si16;
uint8_t need_si20;
uint8_t need_si26;
uint8_t src2_is_4;
uint8_t need_ui12;

uint8_t inst_ll_w;
uint8_t inst_sc_w;

uint8_t inst_dbar;
uint8_t inst_ibar;

uint8_t inst_idle;

uint8_t inst_cacop;
uint8_t inst_valid_cacop;

// 指令合法，但是暂时没有执行意义
uint8_t inst_nop;
uint8_t inst_preld;
uint8_t inst_cpucfg;

uint8_t inst_exist_debug;









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

typedef union {
    uint32_t word;
    struct {
        uint32_t rd_4_0 : 5;  // 小端方式，最低位
        uint32_t rj_9_5 : 5;
        uint32_t rk_14_10 : 5;
        uint32_t op_19_15 : 5;
        uint32_t op_21_20 : 2;
        uint32_t op_25_22 : 4;
        uint32_t op_31_26 : 6;
    } decode;
} instruction_t;

// 函数声明
void init_gpr(riscv_cpu_gpr* cpu);
uint32_t next_pc(uint32_t pc, pc_change_type* change_type);
instruction_t inst_fetch(uint32_t pc);
uint32_t inst_decode(instruction_t instr,
                     decode_info* dc_info,
                     riscv_cpu_gpr* gpr);
void inst_execute(decode_info* dc_info, execute_result* exe_re);
void memo_access(execute_result* exe_result, mem_result* mem_re);
void write_back(riscv_cpu_gpr* cpu, mem_result* mem_re);

#endif
