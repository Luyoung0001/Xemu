# 1 "init.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "init.S"
# 1 "include/asm.h" 1



# 1 "include/regdef.h" 1
# 5 "include/asm.h" 2
# 1 "include/sysdep.h" 1
# 6 "include/asm.h" 2
# 2 "init.S" 2
# 1 "include/regdef.h" 1
# 3 "init.S" 2
# 1 "include/inst_test.h" 1
# 1 "include/cpu_cde.h" 1
# 1 "include/test_config.h" 1
# 2 "include/cpu_cde.h" 2
# 2 "include/inst_test.h" 2
# 1 "include/test_define.h" 1
# 3 "include/inst_test.h" 2
# 4 "init.S" 2

.text; .globl test4_init; .align 3; .cfi_startproc ; .type test4_init, @function; test4_init:
  csrwr $r0, 0x1
  csrwr $r0, 0x6
  csrwr $r0, 0xc

  li.w $r12, 0x1c008000
  csrwr $r12, 0xc

  jirl $r0, $r1, 0x0
.cfi_endproc ; .size test4_init,.-test4_init;

.text; .globl test5_init; .align 3; .cfi_startproc ; .type test5_init, @function; test5_init:
  csrwr $r0, 0x1
  csrwr $r0, 0x6
  csrwr $r0, 0xc
  csrwr $r0, 0x7
  csrwr $r0, 0x30
  csrwr $r0, 0x31
  csrwr $r0, 0x32
  csrwr $r0, 0x33
  csrwr $r0, 0x41

  li.w $r12, 0x1c008000
  csrwr $r12, 0xc

  jirl $r0, $r1, 0x0
.cfi_endproc ; .size test5_init,.-test5_init;

.text; .globl test6_init; .align 3; .cfi_startproc ; .type test6_init, @function; test6_init:
  csrwr $r0, 0x1
  csrwr $r0, 0x6
  csrwr $r0, 0xc
  csrwr $r0, 0x7
  csrwr $r0, 0x30
  csrwr $r0, 0x31
  csrwr $r0, 0x32
  csrwr $r0, 0x33
  csrwr $r0, 0x41
  csrwr $r0, 0x10
  csrwr $r0, 0x11
  csrwr $r0, 0x12
  csrwr $r0, 0x13
  csrwr $r0, 0x18
  csrwr $r0, 0x88

  li.w $r12, 0x0
  li.w $r13, 0x3
  csrxchg $r12, $r13, 0x0
  csrwr $r0, 0x180
  csrwr $r0, 0x181

  jirl $r0, $r1, 0x0
.cfi_endproc ; .size test6_init,.-test6_init;

.text; .globl test8_init; .align 3; .cfi_startproc ; .type test8_init, @function; test8_init:
  csrwr $r0, 0x1
  csrwr $r0, 0x6
  csrwr $r0, 0xc
  csrwr $r0, 0x7
  csrwr $r0, 0x30
  csrwr $r0, 0x31
  csrwr $r0, 0x32
  csrwr $r0, 0x33
  csrwr $r0, 0x41
  csrwr $r0, 0x10
  csrwr $r0, 0x11
  csrwr $r0, 0x12
  csrwr $r0, 0x13
  csrwr $r0, 0x18
  csrwr $r0, 0x88

  li.w $r12, 0x8
  li.w $r13, 0x18
  csrxchg $r12, $r13, 0x0
  csrwr $r0, 0x180
  csrwr $r0, 0x181

  jirl $r0, $r1, 0x0
.cfi_endproc ; .size test8_init,.-test8_init;

.text; .globl tlb_init; .align 3; .cfi_startproc ; .type tlb_init, @function; tlb_init:

 # addi.w $r23, $r23 ,1
# li.w $r25, 0x0

    li.w $r12, 0x0c000000 #index
    li.w $r14, 8<<13
    li.w $r15, 1<<13
    add.w $r13, $r14, $r15 #VPN

    li.w $r14, 0x000000aa
    csrwr $r14, 0x18

###TLB item 1
    li.w $r16, 0x0034ab00 ; li.w $r17, 0x00ffab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 2
    li.w $r16, 0x0035ab00 ; li.w $r17, 0x0100ab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 3
    li.w $r16, 0x0036ab00 ; li.w $r17, 0x0101ab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 4
    li.w $r16, 0x0037ab00 ; li.w $r17, 0x0102ab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17

    li.w $r14, 0x000000ab
    csrwr $r14, 0x18

###TLB item 5
    li.w $r16, 0x0038ab00 ; li.w $r17, 0x0103ab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 6
    li.w $r16, 0x0039ab00 ; li.w $r17, 0x0104ab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 7
    li.w $r16, 0x003aab00 ; li.w $r17, 0x00ffab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 8
    li.w $r16, 0x0134ab00 ; li.w $r17, 0x01ffab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17

    li.w $r14, 0x000000ac
    csrwr $r14, 0x18

###TLB item 9
    li.w $r16, 0x0e34ab00 ; li.w $r17, 0x0effab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 10
    li.w $r16, 0x0f34ab00 ; li.w $r17, 0x0fffab00 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 11
    li.w $r16, 0x034ab100 ; li.w $r17, 0x0ffab100 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 12
    li.w $r16, 0x034ab200 ; li.w $r17, 0x0ffab200 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17

    li.w $r14, 0x000000ad
    csrwr $r14, 0x18

###TLB item 13 G=1
    li.w $r16, 0x034ab940 ; li.w $r17, 0x0ffab940 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 14 G=1
    li.w $r16, 0x034aba40 ; li.w $r17, 0x0ffaba40 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 15 G=1
    li.w $r16, 0x034ab740 ; li.w $r17, 0x0ffab740 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17
###TLB item 16 G=1
    li.w $r16, 0x034ab840 ; li.w $r17, 0x0ffab840 ; addi.w $r14, $r12, 0x0 ; addi.w $r15, $r13, 0x0 ; csrwr $r14, 0x10 ; csrwr $r15, 0x11 ; csrwr $r16, 0x12 ; csrwr $r17, 0x13 ; tlbwr ; addi.w $r12, $r12, 0x1 ; li.w $r17, 1<<13 ; add.w $r13, $r13, $r17

###clear tlb csr reg
    csrwr $r0, 0x11
    csrwr $r0, 0x12
    csrwr $r0, 0x13
    csrwr $r0, 0x10

    jirl $r0, $r1, 0x0

.cfi_endproc ; .size tlb_init,.-tlb_init;
