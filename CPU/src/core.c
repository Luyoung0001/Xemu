#include <commen.h>
#include <core.h>
#include <difftest.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>

pc_change_type change_type;
riscv_cpu_gpr gpr;  // 实例化通用寄存器
uint32_t pc;        // 当前 pc
uint32_t pc_next;
instruction_t inst;  // 指令
decode_info de_info;
uint32_t decode_result;
execute_result exe_resul;

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

// 译码结果

uint8_t alu_op_add;
uint8_t alu_op_sub;
uint8_t alu_op_slt;
uint8_t alu_op_sltu;
uint8_t alu_op_and;
uint8_t alu_op_nor;
uint8_t alu_op_or;
uint8_t alu_op_xor;
uint8_t alu_op_sll;
uint8_t alu_op_srl;
uint8_t alu_op_sra;
uint8_t alu_op_lu12i;

uint8_t rf_raddr1;
uint8_t rf_raddr2;
uint32_t rf_rdata1;
uint32_t rf_rdata2;

uint32_t rj_value;
uint32_t rkd_value;

uint8_t need_ui5;
uint8_t need_si12;
uint8_t need_si14;
uint8_t need_si16;
uint8_t need_si20;
uint8_t need_si26;
uint8_t need_ui12;
uint8_t src2_is_4;

uint8_t src_reg_is_rd;
uint8_t src1_is_pc;
uint8_t src2_is_imm;

uint8_t res_from_mem;
uint8_t res_from_csr;
uint8_t rd_csr_addr;
uint8_t csr_we;
uint8_t csr_mask;

uint32_t imm;         // 立即数
uint8_t mem_we;       // 是否写内存

uint32_t br_offs;
uint32_t jirl_offs;

uint8_t rj_eq_rd;
uint8_t rj_lt_rd;
uint8_t rj_ltu_rd;
uint8_t rj_gt_rd;
uint8_t rj_gtu_rd;

uint8_t br_taken;     // 是否跳转
uint32_t br_target;   // 跳转地址
uint8_t inst_valid;   // 当前指令是否有效，如果无效可能会引发异常
uint8_t kernel_inst;  // 是否是核心态指令

uint32_t alu_src1;  // 操作数
uint32_t alu_src2;
uint32_t alu_result;  // alu 计算结果

uint32_t mul_div_src1;    // 操作数
uint32_t mul_div_src2;    // 操作数
uint32_t mul_div_result;  // 乘除法 计算结果

uint8_t dst_is_r1;
uint8_t dst_is_rj;

uint8_t gr_we;  // 寄存器写
uint8_t dest;   // 寄存写地址

uint32_t rdata_mem;  // 从内存取出的值
uint32_t wdata_mem;  // 存储的值
uint32_t final_rdata_result;
uint32_t final_wdata_result;

// 初始化 CPU 寄存器和程序计数器
void init_gpr() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        gpr.regs[i] = 0;  // 初始化寄存器为0
    }
}

// pre_IFU 对指令的 PC 进行一个处理
void next_pc() {
    // 根据情况来判断 next_pc 的值
    switch (change_type.change_type) {
        case 1:
            pc += 4;
            break;
        case 2:
            pc = br_target;
            break;
        default:
            pc += 4;
    }
}

// 取指
instruction_t inst_fetch() {
    uint32_t read_result = read_memory(pc);
    return (instruction_t)read_result;
}

// IDU 访问寄存器
void rf_result() {
    rf_rdata1 = gpr.regs[rf_raddr1];
    rf_rdata2 = gpr.regs[rf_raddr2];
}

// 解析译码结果
void decoder_more() {
    alu_op_add = inst_add_w | inst_addi_w | inst_ld_w | inst_st_w | inst_jirl |
                 inst_bl | inst_pcaddu12i | inst_ld_b | inst_ld_bu | inst_ld_h |
                 inst_ld_hu | inst_st_b | inst_st_h | inst_ll_w | inst_sc_w |
                 inst_valid_cacop;
    alu_op_sub = inst_sub_w;
    alu_op_slt = inst_slt | inst_slti;
    alu_op_sltu = inst_sltu | inst_sltui;
    alu_op_and = inst_and | inst_andi;
    alu_op_nor = inst_nor;
    alu_op_or = inst_or | inst_ori;
    alu_op_xor = inst_xor | inst_xori;
    alu_op_sll = inst_slli_w | inst_sll;
    alu_op_srl = inst_srli_w | inst_srl;
    alu_op_sra = inst_srai_w | inst_sra;
    alu_op_lu12i = inst_lu12i_w;
}

// alu 计算单元
void alu_caculate() {
    uint32_t _add = alu_src1 + alu_src2;
    uint32_t _sub = alu_src1 - alu_src2;
    uint32_t _sll = alu_src1
                    << (alu_src2 & 0x1F);  // 32位架构通常使用低5位作为移位量
    uint32_t _slt = ((int32_t)alu_src1 < (int32_t)alu_src2) ? 1 : 0;
    uint32_t _sltu = (alu_src1 < alu_src2) ? 1 : 0;
    uint32_t _and = alu_src1 & alu_src2;
    uint32_t _nor = ~(alu_src1 | alu_src2);
    uint32_t _or = alu_src1 | alu_src2;
    uint32_t _xor = alu_src1 ^ alu_src2;
    uint32_t _srl = alu_src1 >> (alu_src2 & 0x1F);
    uint32_t _sra = (int32_t)alu_src1 >> (alu_src2 & 0x1F);
    uint32_t _lu12i = alu_src2;

    alu_result = alu_op_add     ? _add
                 : alu_op_sub   ? _sub
                 : alu_op_slt   ? _slt
                 : alu_op_sltu  ? _sltu
                 : alu_op_and   ? _and
                 : alu_op_nor   ? _nor
                 : alu_op_or    ? _or
                 : alu_op_xor   ? _xor
                 : alu_op_sll   ? _sll
                 : alu_op_srl   ? _srl
                 : alu_op_sra   ? _sra
                 : alu_op_lu12i ? _lu12i
                                : 0;
}
// mult_dev 计算单元

void mult_div() {
    uint64_t mulh_w = mul_div_src1 * mul_div_src1;
    uint64_t mulhu_w = mul_div_src1 * mul_div_src1;
    uint32_t div_w = mul_div_src1 / mul_div_src2;
    uint32_t divu_w = mul_div_src1 / mul_div_src2;
    uint32_t mod_w = mul_div_src1 % mul_div_src2;
    uint32_t modu_w = mul_div_src1 % mul_div_src2;

    mul_div_result = inst_mul_w     ? mulh_w
                     : inst_mulh_wu ? mulhu_w
                     : inst_div_w   ? div_w
                     : inst_div_wu  ? divu_w
                     : inst_mod_w   ? mod_w
                     : inst_mod_wu  ? modu_w
                                    : 0;
}

uint32_t extend(uint8_t s, uint8_t length_of_num, uint32_t num) {
    uint32_t mask = (1U << length_of_num) - 1;
    num &= mask;
    if (s) {
        if (num & (1U << (length_of_num - 1))) {
            return num | (~mask);
        }
    } else {
        return num;
    }
    return num;
}

// 译码
uint32_t inst_decode() {
    // 解析指令类型
    instruction_t instr = inst;
    uint32_t MASK_24 = 0b00000001000000000000000000000000;
    uint32_t MASK_25 = 0b00000010000000000000000000000000;
    uint8_t rd = instr.decode.rd_4_0;
    uint8_t rj = instr.decode.rj_9_5;
    uint8_t rk = instr.decode.rk_14_10;

    uint32_t i12 = (instr.word >> 10) & 0xfff;
    uint32_t i14 = (instr.word >> 10) & 0x3fff;
    uint32_t i20 = (instr.word >> 5) & 0xfffff;
    uint32_t i16 = (instr.word >> 10) & 0xffff;
    uint32_t i26 = (instr.word & 0x3ff) | i16;
    uint8_t inst_25 = (inst.word & MASK_25) == MASK_25;  // 指令的第 25 位
    uint8_t inst_24 = (inst.word & MASK_24) == MASK_24;  // 指令的第 24 位

    inst_add_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                 inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x00;
    inst_sub_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                 inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x02;
    inst_slt = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x04;
    inst_slti = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x08;
    inst_sltu = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x05;
    inst_sltui = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x09;
    inst_nor = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x08;
    inst_and = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x09;
    inst_andi = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0xd;
    inst_or = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
              inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x0a;
    inst_ori = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0xe;
    inst_xor = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x0b;
    inst_xori = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0xf;
    inst_sll = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0xe;
    inst_slli_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x1 &&
                  inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x01;
    inst_srl = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x0f;
    inst_srli_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x1 &&
                  inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x09;
    inst_sra = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
               inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x10;
    inst_srai_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x1 &&
                  inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x11;
    inst_addi_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0xa;

    inst_ld_w = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x2;
    inst_ld_b = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x0;
    inst_ld_bu = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x08;
    inst_ld_h = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x1;
    inst_ld_hu = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x09;
    inst_st_w = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x6;
    inst_st_b = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x4;
    inst_st_h = inst.decode.op_31_26 == 0x0a && inst.decode.op_25_22 == 0x5;

    inst_jirl = inst.decode.op_31_26 == 0x13;
    inst_b = inst.decode.op_31_26 == 0x14;
    inst_bl = inst.decode.op_31_26 == 0x15;
    inst_blt = inst.decode.op_31_26 == 0x18;
    inst_bltu = inst.decode.op_31_26 == 0x1a;
    inst_beq = inst.decode.op_31_26 == 0x16;
    inst_bne = inst.decode.op_31_26 == 0x17;
    inst_bge = inst.decode.op_31_26 == 0x19;
    inst_bgeu = inst.decode.op_31_26 == 0x1b;

    inst_lu12i_w = inst.decode.op_31_26 == 0x05 && ~inst_25;
    inst_pcaddu12i = inst.decode.op_31_26 == 0x07 && ~inst_25;

    inst_mul_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                 inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x18;
    inst_mulh_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                  inst.decode.op_21_20 == 0x1 && inst.decode.op_19_15 == 0x19;
    inst_mulh_wu = inst.decode.op_31_26 == 0x00 &&
                   inst.decode.op_25_22 == 0x0 && inst.decode.op_21_20 == 0x1 &&
                   inst.decode.op_19_15 == 0x1a;
    inst_div_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                 inst.decode.op_21_20 == 0x2 && inst.decode.op_19_15 == 0x0;
    inst_div_wu = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                  inst.decode.op_21_20 == 0x2 && inst.decode.op_19_15 == 0x2;
    inst_mod_w = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                 inst.decode.op_21_20 == 0x2 && inst.decode.op_19_15 == 0x1;
    inst_mod_wu = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                  inst.decode.op_21_20 == 0x2 && inst.decode.op_19_15 == 0x3;

    inst_csrrd =
        inst.decode.op_31_26 == 0x01 && ~inst_25 && ~inst_24 && rj == 0x00;
    inst_csrwr =
        inst.decode.op_31_26 == 0x01 && ~inst_25 && ~inst_24 && rj == 0x01;
    inst_csrxchg = inst.decode.op_31_26 == 0x01 && ~inst_25 && ~inst_24 &&
                   ~rj == 0x00 && ~rj == 0x01;
    inst_ertn = inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x09 &&
                inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x10 &&
                rk == 0x0e && rj == 0x00 && rd == 0x00;
    inst_syscall = inst.decode.op_31_26 == 0x00 &&
                   inst.decode.op_25_22 == 0x0 && inst.decode.op_21_20 == 0x2 &&
                   inst.decode.op_19_15 == 0x16;
    inst_break = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                 inst.decode.op_21_20 == 0x2 && inst.decode.op_19_15 == 0x14;

    inst_rdcntid_w = inst.decode.op_31_26 == 0x00 &&
                     inst.decode.op_25_22 == 0x0 &&
                     inst.decode.op_21_20 == 0x0 &&
                     inst.decode.op_19_15 == 0x00 && rk == 0x18 && rd == 0x00;
    inst_rdcntvl_w =
        inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
        inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x00 &&
        rk == 0x18 && rj == 0x00 && ~rd == 0x00;
    inst_rdcntvh_w = inst.decode.op_31_26 == 0x00 &&
                     inst.decode.op_25_22 == 0x0 &&
                     inst.decode.op_21_20 == 0x0 &&
                     inst.decode.op_19_15 == 0x00 && rk == 0x19 && rj == 0x00;
    inst_zero = inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x0 &&
                inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x00 &&
                rk == 0x00 && rj == 0x00 && rd == 0x00;

    inst_invtlb = inst.decode.op_31_26 == 0x01 &&
                  inst.decode.op_25_22 == 0x09 && inst.decode.op_21_20 == 0x0 &&
                  inst.decode.op_19_15 == 0x13;
    inst_tlbsrch =
        inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x09 &&
        inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x10 &&
        rk == 0x0a && rj == 0x00 && rd == 0x00;
    inst_tlbrd = inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x09 &&
                 inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x10 &&
                 rk == 0x0b && rj == 0x00 && rd == 0x00;
    inst_tlbwr = inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x09 &&
                 inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x10 &&
                 rk == 0x0c && rj == 0x00 && rd == 0x00;
    inst_tlbfill =
        inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x09 &&
        inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x10 &&
        rk == 0x0d && rj == 0x00 && rd == 0x00;

    inst_ll_w = inst.decode.op_31_26 == 0x08 && ~inst_25 && ~inst_24;
    inst_sc_w = inst.decode.op_31_26 == 0x08 && ~inst_25 && inst_24;

    inst_dbar = inst.decode.op_31_26 == 0x0e && inst.decode.op_25_22 == 0x1 &&
                inst.decode.op_21_20 == 0x3 && inst.decode.op_19_15 == 0x04;
    inst_ibar = inst.decode.op_31_26 == 0x0e && inst.decode.op_25_22 == 0x1 &&
                inst.decode.op_21_20 == 0x3 && inst.decode.op_19_15 == 0x05;
    inst_idle = inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x09 &&
                inst.decode.op_21_20 == 0x0 && inst.decode.op_19_15 == 0x11;

    inst_cacop = inst.decode.op_31_26 == 0x01 && inst.decode.op_25_22 == 0x08;

    need_ui5 = inst_slli_w | inst_srli_w | inst_srai_w;
    need_si12 = inst_addi_w | inst_ld_w | inst_st_w | inst_slti | inst_sltui |
                inst_ld_b | inst_ld_bu | inst_ld_h | inst_ld_hu | inst_st_b |
                inst_st_h | inst_valid_cacop;

    need_si14 = inst_ll_w | inst_sc_w;
    need_si16 = inst_jirl | inst_beq | inst_bne;

    need_si20 = inst_lu12i_w | inst_pcaddu12i;
    need_si26 = inst_b | inst_bl;

    src2_is_4 = inst_jirl | inst_bl;

    need_ui12 = inst_andi | inst_ori | inst_xori;

    imm = src2_is_4   ? 4
          : need_ui12 ? i12
          : need_si14 ? extend(1, 14, i14) << 2
          : need_si20 ? i20 << 12
                      : extend(1, 12, i12);

    br_offs = need_si26 ? extend(1, 26, i26) << 2 : extend(1, 16, i16) << 2;

    jirl_offs = extend(1, 16, i16) << 2;

    src_reg_is_rd = inst_beq | inst_bne | inst_st_w | inst_st_h | inst_st_b |
                    inst_sc_w | inst_blt | inst_bltu | inst_bge | inst_bgeu |
                    inst_csrwr | inst_csrxchg;

    src1_is_pc = inst_jirl | inst_bl | inst_pcaddu12i;

    src2_is_imm = inst_slli_w | inst_srli_w | inst_srai_w | inst_addi_w |
                  inst_lu12i_w | inst_jirl | inst_bl | inst_slti | inst_sltui |
                  inst_andi | inst_ori | inst_xori | inst_pcaddu12i |
                  inst_st_b | inst_st_h | inst_st_w | inst_ld_b | inst_ld_bu |
                  inst_ld_h | inst_ld_hu | inst_ld_w | inst_ll_w | inst_sc_w |
                  inst_valid_cacop;

    res_from_mem =
        inst_ld_w | inst_ld_b | inst_ld_bu | inst_ld_h | inst_ld_hu | inst_ll_w;

    res_from_csr = inst_csrrd | inst_csrwr | inst_csrxchg | inst_rdcntid_w |
                   inst_rdcntvl_w | inst_rdcntvh_w | inst_cpucfg;
    mem_we = inst_st_w || inst_st_b || inst_st_h;

    dst_is_r1 = inst_bl;
    dst_is_rj = inst_rdcntid_w;

    gr_we = inst_jirl | inst_bl | inst_add_w | inst_sub_w | inst_slt |
            inst_sltu | inst_nor | inst_and | inst_or | inst_xor | inst_slli_w |
            inst_srli_w | inst_srai_w | inst_addi_w | inst_ld_w | inst_ld_b |
            inst_ld_bu | inst_ld_h | inst_ld_hu | inst_lu12i_w | inst_slti |
            inst_sltui | inst_andi | inst_ori | inst_xori | inst_sll |
            inst_srl | inst_sra | inst_pcaddu12i | inst_mul_w | inst_mulh_w |
            inst_mulh_wu | inst_div_w | inst_div_wu | inst_mod_w | inst_mod_wu |
            inst_csrrd | inst_csrxchg | inst_csrwr | inst_rdcntid_w |
            inst_rdcntvl_w | inst_rdcntvh_w | inst_ll_w | inst_sc_w |
            inst_cpucfg;

    dest = dst_is_r1 ? 1 : dst_is_rj ? rj : rd;

    rf_raddr1 = rj;
    rf_raddr2 = src_reg_is_rd ? rd : rk;

    // 访问 gpr
    rf_result();

    rj_value = rf_rdata1;
    rkd_value = rf_rdata2;
    decoder_more();

    rj_eq_rd = rj_value == rkd_value;
    rj_lt_rd = (int)rj_value < (int)rkd_value;
    rj_ltu_rd = rj_value < rkd_value;
    rj_gt_rd = (int)rj_value >= (int)rkd_value;
    rj_gtu_rd = rj_value >= rkd_value;

    // 是否跳转
    br_taken = (inst_beq & rj_eq_rd) | (inst_bne & !rj_eq_rd) | inst_jirl |
               inst_bl | inst_b | (inst_blt & rj_lt_rd) |
               (inst_bltu & rj_ltu_rd) | (inst_bge & rj_gt_rd) |
               (inst_bgeu & rj_gtu_rd);
    // 跳转目标
    br_target = (inst_beq | inst_bne | inst_bl | inst_b | inst_blt | inst_bltu |
                 inst_bge | inst_bgeu)
                    ? (pc + br_offs)
                    : (rj_value + jirl_offs);

    // 指示指令是否有效，引发某些异常
    inst_valid = inst_add_w | inst_sub_w | inst_slt | inst_slti | inst_sltu |
                 inst_sltui | inst_nor | inst_and | inst_andi | inst_or |
                 inst_ori | inst_xor | inst_xori | inst_sll | inst_slli_w |
                 inst_srl | inst_srli_w | inst_sra | inst_srai_w | inst_addi_w |
                 inst_ld_w | inst_ld_b | inst_ld_bu | inst_ld_h | inst_ld_hu |
                 inst_st_w | inst_st_b | inst_st_h | inst_jirl | inst_b |
                 inst_bl | inst_blt | inst_bltu | inst_beq | inst_bne |
                 inst_bge | inst_bgeu | inst_lu12i_w | inst_pcaddu12i |
                 inst_mul_w | inst_mulh_w | inst_mulh_wu | inst_div_w |
                 inst_div_wu | inst_mod_w | inst_mod_wu | inst_csrrd |
                 inst_csrwr | inst_csrxchg | inst_ertn | inst_syscall |
                 inst_break | inst_rdcntid_w | inst_rdcntvl_w | inst_rdcntvh_w |
                 inst_tlbsrch | inst_tlbrd | inst_tlbwr | inst_tlbfill |
                 inst_ll_w | inst_sc_w | inst_dbar | inst_ibar | inst_idle |
                 inst_valid_cacop | inst_nop | inst_preld | inst_cpucfg |
                 (inst_invtlb & (rd == 0 || rd == 1 || rd == 2 || rd == 3 ||
                                 rd == 4 || rd == 5 || rd == 6));

    // 输送到 debug 系统，如果指令执行到了不存在的指令处，那么继续添加
    if (!inst_valid) {
        return inst.word;
    }

    kernel_inst = inst_csrrd | inst_csrwr | inst_csrxchg | inst_tlbsrch |
                  inst_tlbrd | inst_tlbwr | inst_tlbfill | inst_invtlb |
                  inst_idle | inst_ertn;

    alu_src1 = src1_is_pc ? pc : rj_value;
    alu_src2 = src2_is_imm ? imm : rkd_value;

    // printf("alu_src2: %08x\n", alu_src2);
    // printf("imm: %08x\n", imm);
    // printf("i20: %08x\n", i20);
    // printf("inst_lu12i_w: %08x\n", inst_lu12i_w);
    // printf("(instr.word >> 5): %08x\n", (instr.word >> 5));
    // printf("(instr.word >> 5) & 0xfffff: %08x\n", (instr.word >> 5) &
    // 0xfffff); 计算类型： alu mult_div
    de_info.inst_op_type = (alu_op_add | alu_op_sub | alu_op_slt | alu_op_sltu |
                            alu_op_and | alu_op_nor | alu_op_or | alu_op_xor |
                            alu_op_sll | alu_op_srl | alu_op_sra | alu_op_lu12i)
                               ? ALU_TYPE
                           : (inst_mul_w | inst_mulh_wu | inst_div_w |
                              inst_div_wu | inst_mod_w | inst_mod_wu)
                               ? MULT_DIV_TYPE
                               : UNKNOWN_TYPE;

    change_type.change_type = br_taken ? 2 : 1;

    return 0;
}
// 执行
void inst_execute() {
    if (de_info.inst_op_type == ALU_TYPE) {
        alu_caculate();
    } else if (de_info.inst_op_type == MULT_DIV_TYPE) {
        mult_div();
    }
}

// 访存
void memo_access() {
    // 访存
    // 这里访问内存的地址永远要保持 4 字节对齐
    // 取出来的值要根据 ld 的类型进行截取或者处理

    uint32_t final_addr = alu_result & ~(0x3);
    if (res_from_mem) {
        // load data
        rdata_mem = read_memory(final_addr);
    }

    uint8_t offset = alu_result & 0x3;  // 取最低两位
    // load
    if (inst_ld_b) {
        final_rdata_result =
            offset == 0   ? extend(1, 8, rdata_mem & 0xf)
            : offset == 1 ? extend(1, 8, (rdata_mem >> 8) & 0xf)
            : offset == 2 ? extend(1, 8, (rdata_mem >> 16) & 0xf)
                          : extend(1, 8, (rdata_mem >> 24) & 0xf);
    } else if (inst_ld_bu) {
        final_rdata_result =
            offset == 0   ? extend(0, 8, rdata_mem & 0xf)
            : offset == 1 ? extend(0, 8, (rdata_mem >> 8) & 0xf)
            : offset == 2 ? extend(0, 8, (rdata_mem >> 16) & 0xf)
                          : extend(0, 8, (rdata_mem >> 24) & 0xf);

    } else if (inst_ld_h) {
        final_rdata_result = offset == 0 ? extend(1, 16, rdata_mem & 0xff)
                             : offset == 2
                                 ? extend(1, 16, (rdata_mem >> 16) & 0xff)
                                 : 0;  // 其他情况异常
    } else if (inst_ld_hu) {
        final_rdata_result = offset == 0 ? extend(0, 16, rdata_mem & 0xff)
                             : offset == 2
                                 ? extend(0, 16, (rdata_mem >> 16) & 0xff)
                                 : 0;  // 其他情况异常
    } else if (inst_ld_w) {
        final_rdata_result = rdata_mem;
    }

    // store
    // rkd_value
    if (inst_st_b) {
        uint8_t lowest_8_bit = rkd_value & 0xff;
        final_wdata_result = lowest_8_bit | (lowest_8_bit << 8) |
                             (lowest_8_bit << 16) | (lowest_8_bit << 24);
    } else if (inst_st_h) {
        uint8_t lowest_16_bit = rkd_value & 0xffff;
        final_wdata_result = lowest_16_bit | (lowest_16_bit << 16);
    } else if (inst_st_w) {
        final_wdata_result = rkd_value;
    }

    if (mem_we) {
        // store data
        write_memory(final_addr, final_wdata_result);
    }

}

// 回写
void write_back() {
    mycpu_trace_info->we = gr_we;
    mycpu_trace_info->pc = pc;
    mycpu_trace_info->wnum = dest;
    mycpu_trace_info->value = res_from_mem ? final_rdata_result : alu_result;

    if (gr_we && dest != 0) {
        gpr.regs[dest] = res_from_mem ? final_rdata_result : alu_result;
    }
}
