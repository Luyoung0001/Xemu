CC = gcc

WORK_PATH = $(shell pwd)
INC_COMMEN = $(WORK_PATH)/commen
INC_PATH_CPU = $(WORK_PATH)/CPU/include
INC_PATH_DEV = $(WORK_PATH)/device/include

SRC_CPU = $(wildcard $(WORK_PATH)/CPU/src/core.c)
SRC_DEV = $(wildcard $(WORK_PATH)/device/src/memory.c)

SRC_SIMU = $(WORK_PATH)/simulator/main.c

SRC = $(SRC_CPU) $(SRC_DEV) $(SRC_SIMU)

CFLAGS = -Wall -g -std=c99 -I$(INC_PATH_CPU) -I$(INC_PATH_DEV) -I$(INC_COMMEN)
OBJ = $(SRC:.c=.o)
EXEC = riscv-simulator

all: check_bear $(EXEC)

check_bear:
	@command -v bear > /dev/null || { echo "bear tool not found, please install it"; exit 1; }

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	bear --append --output $(WORK_PATH)/compile_commands.json -- $(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
