#ifndef CORE_H
#define CORE_H

#include <stdint.h>



#define NUM_REGISTERS 32  // 寄存器数量

extern uint8_t inst_add_w;
extern uint8_t inst_sub_w;
extern uint8_t inst_slt;
extern uint8_t inst_slti;
extern uint8_t inst_sltu;
extern uint8_t inst_sltui;
extern uint8_t inst_nor;
extern uint8_t inst_and;
extern uint8_t inst_andi;
extern uint8_t inst_or;
extern uint8_t inst_ori;
extern uint8_t inst_xor;
extern uint8_t inst_xori;
extern uint8_t inst_sll;
extern uint8_t inst_slli_w;
extern uint8_t inst_srl;
extern uint8_t inst_srli_w;
extern uint8_t inst_sra;
extern uint8_t inst_srai_w;
extern uint8_t inst_addi_w;
extern uint8_t inst_ld_w;
extern uint8_t inst_ld_b;
extern uint8_t inst_ld_bu;
extern uint8_t inst_ld_h;
extern uint8_t inst_ld_hu;
extern uint8_t inst_st_w;
extern uint8_t inst_st_b;
extern uint8_t inst_st_h;
extern uint8_t inst_jirl;
extern uint8_t inst_b;
extern uint8_t inst_bl;
extern uint8_t inst_blt;
extern uint8_t inst_bltu;
extern uint8_t inst_beq;
extern uint8_t inst_bne;
extern uint8_t inst_bge;
extern uint8_t inst_bgeu;
extern uint8_t inst_lu12i_w;
extern uint8_t inst_pcaddu12i;

extern uint8_t inst_mul_w;
extern uint8_t inst_mulh_w;
extern uint8_t inst_mulh_wu;
extern uint8_t inst_div_w;
extern uint8_t inst_div_wu;
extern uint8_t inst_mod_w;
extern uint8_t inst_mod_wu;

extern uint8_t inst_csrrd;
extern uint8_t inst_csrwr;
extern uint8_t inst_csrxchg;
extern uint8_t inst_ertn;

extern uint8_t inst_syscall;
extern uint8_t inst_break;

extern uint8_t inst_rdcntid_w;
extern uint8_t inst_rdcntvl_w;
extern uint8_t inst_rdcntvh_w;
extern uint8_t inst_zero;

extern uint8_t inst_tlbsrch;
extern uint8_t inst_tlbrd;
extern uint8_t inst_tlbwr;
extern uint8_t inst_tlbfill;
extern uint8_t inst_invtlb;

extern uint8_t need_ui5;
extern uint8_t need_si12;
extern uint8_t need_si14;
extern uint8_t need_si16;
extern uint8_t need_si20;
extern uint8_t need_si26;
extern uint8_t src2_is_4;
extern uint8_t need_ui12;

extern uint8_t inst_ll_w;
extern uint8_t inst_sc_w;

extern uint8_t inst_dbar;
extern uint8_t inst_ibar;

extern uint8_t inst_idle;

extern uint8_t inst_cacop;
extern uint8_t inst_valid_cacop;

// 指令合法，但是暂时没有执行意义
extern uint8_t inst_nop;
extern uint8_t inst_preld;
extern uint8_t inst_cpucfg;

extern uint8_t inst_exist_debug;

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

#define ALU_TYPE 1
#define MULT_DIV_TYPE 2
#define UNKNOWN_TYPE 3

// 译码后的数据结构体
typedef struct {
    uint8_t inst_op_type;  // 操作类型
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
void init_gpr();
void next_pc();
instruction_t inst_fetch();
uint32_t inst_decode();
void inst_execute();
void memo_access();
void write_back();

// 使用全局变量
extern pc_change_type change_type;
extern riscv_cpu_gpr gpr;      // 实例化通用寄存器
extern uint32_t pc;  // 当前 pc
extern uint32_t pc_next;
extern instruction_t inst;  // 指令
extern decode_info de_info;
extern uint32_t decode_result;
extern execute_result exe_resul;

#endif
