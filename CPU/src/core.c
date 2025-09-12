#include <commen.h>
#include <core.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
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
uint8_t gpr_we;       // 是否写寄存器
uint8_t mem_we;       // 是否写内存
uint8_t gpr_wr_addr;  // 写寄存器的地址

uint32_t br_offs;
uint32_t jirl_offs;

uint8_t rj_eq_rd;
uint8_t rj_lt_rd;
uint8_t rj_ltu_rd;
uint8_t rj_gt_rd;
uint8_t rj_gtu_rd;

uint8_t br_taken;     // 是否跳转
uint8_t br_target;    // 跳转地址
uint8_t inst_valid;   // 当前指令是否有效，如果无效可能会引发异常
uint8_t kernel_inst;  // 是否是核心态指令

uint32_t alu_src1;  // 操作数
uint32_t alu_src2;
uint32_t alu_result;  // alu 计算结果

uint32_t mul_div_src1;    // 操作数
uint32_t mul_div_src2;    // 操作数
uint32_t mul_div_result;  // 乘除法 计算结果

// 初始化 CPU 寄存器和程序计数器
void init_gpr(riscv_cpu_gpr* cpu) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu->regs[i] = 0;  // 初始化寄存器为0
    }
}

// pre_IFU 对指令的 PC 进行一个处理

uint32_t next_pc(uint32_t pc, pc_change_type* change_type) {
    // 根据情况来判断 next_pc 的值
    switch (change_type->change_type) {
        case 1:
            return pc + 4;
            break;
        default:
            return pc + 4;
    }
    return pc + 4;
}

// 取指
instruction_t inst_fetch(uint32_t pc) {
    uint32_t read_result = read_memory(pc);
    return (instruction_t)read_result;
}

// IDU 访问寄存器
void rf_result(riscv_cpu_gpr* gpr) {
    rf_rdata1 = gpr->regs[rf_raddr1];
    rf_rdata2 = gpr->regs[rf_raddr2];
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
}

// alu 计算单元
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

    alu_result = alu_op_add    ? _add
                 : alu_op_sub  ? _sub
                 : alu_op_slt  ? _slt
                 : alu_op_sltu ? _sltu
                 : alu_op_and  ? _and
                 : alu_op_nor  ? _nor
                 : alu_op_or   ? _or
                 : alu_op_xor  ? _xor
                 : alu_op_sll  ? _sll
                 : alu_op_srl  ? _srl
                 : alu_op_sra  ? _sra
                               : 0;  // 默认值，当所有标志都为0时，返回0
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
                     : modu_w       ? modu_w
                                    : 0;
}

// 译码
uint32_t inst_decode(instruction_t inst,
                     decode_info* dc_info,
                     riscv_cpu_gpr* gpr) {
    // 这里进行译码
    // 得到以下信息:
    // 1、指令类型
    // 2、两个寄存器访问地址
    // 3、操作类型：alu or what
    // 4、如果是访存：访存类型
    // 等等信息保存在全局变量中，以供后续单元使用？
    // 还是创建一个结构体，直接传送到inst_exexute?
    // 这里为了反应真实 CPU情况，应该创建一个结构体进行传送
    // 尽量避免全局变量

    inst_add_w =
        (inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x00 &&
         inst.decode.op_21_20 == 0x01 && inst.decode.op_19_15 == 0x00);
    inst_sub_w =
        (inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x00 &&
         inst.decode.op_21_20 == 0x01 && inst.decode.op_19_15 == 0x02);
    inst_slt = (inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x00 &&
                inst.decode.op_21_20 == 0x01 && inst.decode.op_19_15 == 0x04);
    inst_slti = (inst.decode.op_31_26 == 0x00 && inst.decode.op_25_22 == 0x08);

    // 输送到 debug 系统，如果指令执行到了不存在的指令处，那么继续添加
    inst_exist_debug = inst_add_w | inst_sub_w | inst_slt | inst_slti;
    if (inst_exist_debug) {
        return inst.word;
    }

    // 访问 gpr
    rf_result(gpr);

    return 0;
}
// 执行
void inst_execute(decode_info* dc_info, execute_result* exe_re) {}

// 访存
void memo_access(execute_result* exe_result, mem_result* mem_re) {}

// 回写
void write_back(riscv_cpu_gpr* cpu, mem_result* mem_re) {}
