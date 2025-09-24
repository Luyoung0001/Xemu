# difftest 说明

这里进行 difftest，参考的是 https://gitee.com/loongson-edu/cdp_ede_local/tree/master/mycpu_env/gettrace 中的 gettrace。

首先在目录 ref/func 中生成好测试用例，比如 EXP=10 对应的 bin 文件：

```bash
make EXP=10
```
以上命令可以生成这些文件：

```bash
ls
convert          n26_xori.o     n46_st_h.s          n67_invtlb_0x5.o
data_ram.coe     n26_xori.s     n47_syscall_ex.o    n67_invtlb_0x5.s
data_ram.mif     n27_sll_w.o    n47_syscall_ex.s    n68_invtlb_0x6.o
init.o           n27_sll_w.s    n48_brk_ex.o        n68_invtlb_0x6.s
init.s           n28_sra_w.o    n48_brk_ex.s        n69_invtlb_inv_op.o
inst_ram.coe     n28_sra_w.s    n49_ti_ex.o         n69_invtlb_inv_op.s
inst_ram.mif     n29_srl_w.o    n49_ti_ex.s         n6_sltu.o
libinst.a        n29_srl_w.s    n4_sub_w.o          n6_sltu.s
main.bin         n2_add_w.o     n4_sub_w.s          n70_tlb_4MB.o
main.data        n2_add_w.s     n50_ine_ex.o        n70_tlb_4MB.s
main.elf         n30_div_w.o    n50_ine_ex.s        n71_tlb_ex.o
n10_nor.o        n30_div_w.s    n51_soft_int_ex.o   n71_tlb_ex.s
n10_nor.s        n31_div_wu.o   n51_soft_int_ex.s   n72_dmw_test.o
n11_slli_w.o     n31_div_wu.s   n52_adef_ex.o       n72_dmw_test.s
n11_slli_w.s     n32_mul_w.o    n52_adef_ex.s       n73_icacop_op0.o
n12_srli_w.o     n32_mul_w.s    n53_ale_ld_w_ex.o   n73_icacop_op0.s
n12_srli_w.s     n33_mulh_w.o   n53_ale_ld_w_ex.s   n74_dcacop_op0.o
n13_srai_w.o     n33_mulh_w.s   n54_ale_ld_h_ex.o   n74_dcacop_op0.s
n13_srai_w.s     n34_mulh_wu.o  n54_ale_ld_h_ex.s   n75_icacop_op1.o
n14_ld_w.o       n34_mulh_wu.s  n55_ale_ld_hu_ex.o  n75_icacop_op1.s
n14_ld_w.s       n35_mod_w.o    n55_ale_ld_hu_ex.s  n76_dcacop_op1.o
n15_st_w.o       n35_mod_w.s    n56_ale_st_h_ex.o   n76_dcacop_op1.s
n15_st_w.s       n36_mod_wu.o   n56_ale_st_h_ex.s   n77_icacop_op2.o
n16_beq.o        n36_mod_wu.s   n57_ale_st_w_ex.o   n77_icacop_op2.s
n16_beq.s        n37_blt.o      n57_ale_st_w_ex.s   n78_dcacop_op2.o
n17_bne.o        n37_blt.s      n58_rdcnt.o         n78_dcacop_op2.s
n17_bne.s        n38_bge.o      n58_rdcnt.s         n79_cache_writeback.o
n18_bl.o         n38_bge.s      n59_tlbrd_tlbwr.o   n79_cache_writeback.s
n18_bl.s         n39_bltu.o     n59_tlbrd_tlbwr.s   n7_and.o
n19_jirl.o       n39_bltu.s     n5_slt.o            n7_and.s
n19_jirl.s       n3_addi_w.o    n5_slt.s            n80_ti_ex_wait.o
n1_lu12i_w.o     n3_addi_w.s    n60_tlbfill.o       n80_ti_ex_wait.s
n1_lu12i_w.s     n40_bgeu.o     n60_tlbfill.s       n81_atomic_ins.o
n20_b.o          n40_bgeu.s     n61_tlbsrch.o       n81_atomic_ins.s
n20_b.s          n41_ld_b.o     n61_tlbsrch.s       n8_or.o
n21_pcaddu12i.o  n41_ld_b.s     n62_invtlb_0x0.o    n8_or.s
n21_pcaddu12i.s  n42_ld_h.o     n62_invtlb_0x0.s    n9_xor.o
n22_slti.o       n42_ld_h.s     n63_invtlb_0x1.o    n9_xor.s
n22_slti.s       n43_ld_bu.o    n63_invtlb_0x1.s    rom.vlog
n23_sltui.o      n43_ld_bu.s    n64_invtlb_0x2.o    start.o
n23_sltui.s      n44_ld_hu.o    n64_invtlb_0x2.s    start.s
n24_andi.o       n44_ld_hu.s    n65_invtlb_0x3.o    test.s
n24_andi.s       n45_st_b.o     n65_invtlb_0x3.s
n25_ori.o        n45_st_b.s     n66_invtlb_0x4.o
n25_ori.s        n46_st_h.o     n66_invtlb_0x4.s
```

其中，inst_ram.mif 是 gettrace 的默认文件，gettrace 会使用这个文件进行仿真，然后生成参考文件 golden_trace.txt。了解能多信息，可以阅读 gettrace 下的 README。

得到了 golden_trace.txt 之后，我们就可以设计 difftest 了。一个思路就是模拟器执行一步，读取一行 golden_trace.txt，对比一下CPU写回寄存器的四值（是否写，哪一条指令写，写哪里，写的值是什么）。

