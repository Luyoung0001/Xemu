# 设置交叉编译工具链
CROSS_COMPILE = loongarch32r-linux-gnusf-

LA32R_GCC     := $(CROSS_COMPILE)gcc
LA32R_AS      := $(CROSS_COMPILE)as
LA32R_OBJDUMP := $(CROSS_COMPILE)objdump
LA32R_OBJCOPY := $(CROSS_COMPILE)objcopy

# 平台相关的编译标志
ARCHFLAGS += -ffreestanding -fno-builtin -nostartfiles -msoft-float
LDFLAGS += -static  # 静态链接

# 包含路径
INC_COMMEN = ../commen
INC_LIB = ../libs/include
INC_DEV = ../device/include

# 源文件
SRC_LIB = ../libs/src/lib.c
SRC_MY_STDIO = ../libs/src/my_stdio.c
SRC_DEV = ../device/src/serial.c
SRC_MAIN = ./$(NAME)/$(NAME).c

# 所有源文件
SRC = $(SRC_LIB) $(SRC_DEV) $(SRC_MAIN) $(SRC_MY_STDIO)

CFLAGS = -I$(INC_LIB) -I$(INC_DEV) -I$(INC_COMMEN) $(ARCHFLAGS)

# 默认目标名称
NAME = hello
OUT_DIR = out
OBJ_DIR = $(OUT_DIR)/$(NAME)
ELF = $(OBJ_DIR)/$(NAME).elf
BIN = $(OBJ_DIR)/$(NAME).bin
TXT = $(OBJ_DIR)/$(NAME).txt

EXEC = $(TXT)

OBJ = $(SRC:.c=.o)

all: check_bear $(EXEC)

check_bear:
	@command -v bear > /dev/null || { echo "bear tool not found, please install it"; exit 1; }

%.o: %.c
	@mkdir -p $(OBJ_DIR)  # 创建目标文件夹
	bear --append --output ../compile_commands.json -- $(LA32R_GCC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ)
	@mkdir -p $(OBJ_DIR)  # 创建目标文件夹
	$(LA32R_GCC) $(OBJ) -o $(ELF) $(LDFLAGS)

$(BIN): $(ELF)
	$(LA32R_OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(ELF) $(BIN)

$(TXT): $(ELF) $(BIN)
	$(LA32R_OBJDUMP) -d $(ELF) > $(TXT)

clean:
	rm -rf $(OUT_DIR) ../libs/src/*.o ../device/src/*.o ./$(NAME)/$(NAME).o

.PHONY: clean check_bear all $(ELF) $(BIN) $(TXT)
