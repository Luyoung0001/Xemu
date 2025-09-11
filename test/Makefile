# 首先找到工具链，目标平台是 riscv，因此这里使用交叉编译
CROSS_COMPILE = riscv64-linux-gnu-

AS        = $(CROSS_COMPILE)gcc
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
LD        = $(CROSS_COMPILE)ld
AR        = $(CROSS_COMPILE)ar
OBJDUMP   = $(CROSS_COMPILE)objdump
OBJCOPY   = $(CROSS_COMPILE)objcopy
READELF   = $(CROSS_COMPILE)readelf

# 平台
COMMON_CFLAGS = -march=rv32ima -mabi=ilp32 -O2

#======================编译被运行的程序========================
INC_LIB = ../libs/include
INC_DEV = ../device/include
SRC_LIB = ../libs/src/my_stdio.c
SRC_DEV = ../device/src/serial.c

SRC = $(SRC_LIB)

CFLAGS = -I$(INC_LIB) -I$(INC_DEV) $(COMMON_CFLAGS)

LDFLAGS = -march=rv32ima -mabi=ilp32

# 默认的 NAME
NAME = hello
OUT_DIR = out

# 创建目标路径
OBJ_DIR = $(OUT_DIR)/$(NAME)

# 生成目标文件的完整路径
ELF = $(OBJ_DIR)/$(NAME).elf
BIN = $(OBJ_DIR)/$(NAME).bin
TXT = $(OBJ_DIR)/$(NAME).txt
OBJ = $(OBJ_DIR)/$(NAME).o $(OBJ_DIR)/my_stdio.o $(OBJ_DIR)/serial.o

# 编译 hello.c
$(OBJ_DIR)/$(NAME).o: $(NAME)/$(NAME).c
	@mkdir -p $(OBJ_DIR)
	bear -- $(CC) $(CFLAGS) -c $< -o $@

# 编译 my_stdio.c
$(OBJ_DIR)/my_stdio.o: $(SRC_LIB)
	@mkdir -p $(OBJ_DIR)
	bear -- $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/serial.o: $(SRC_DEV)
	@mkdir -p $(OBJ_DIR)
	bear -- $(CC) $(CFLAGS) -c $< -o $@


# 链接目标文件生成 ELF 文件
$(ELF): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(ELF) -v

# 生成二进制文件
$(BIN): $(ELF)
	$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(ELF) $(BIN)

# 生成汇编文件
$(TXT): $(ELF)
	$(OBJDUMP) -d $(ELF) > $(TXT)

all: $(TXT)


clean:
	rm -rf $(OUT_DIR)

.PHONY: clean $(ELF) $(BIN) $(TXT)
