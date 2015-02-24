# Programs for building
AS = nasm
CC = clang
LD = ld

# Respective Flags
ASFLAGS = -f elf32
CFLAGS = -Wall -Werror -pedantic -m32 -O0 -I include
LDFLAGS = -m elf_i386 -T

# Assembly source files
ASM_SRC = $(shell find . -name *.asm)
ASM_OBJ = $(ASM_SRC:%.asm=%.o)

# C source files
C_SRC = $(shell find . -name *.c)
C_OBJ = $(C_SRC:%.c=%.o)

# Kernel image name
BIN = humboldt_image

# QEMU options
CMD = qemu-system-i386
CMDFLAGS = -kernel

.PHONY: all
all: $(BIN)

$(BIN): $(ASM_OBJ) $(C_OBJ)
	$(LD) $(LDFLAGS) link.ld -o $(BIN) $(ASM_OBJ) $(C_OBJ)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# I hate typing this a lot...
.PHONY: run
run:
	$(CMD) $(CMDFLAGS) $(BIN)

.PHONY: clean
clean:
	rm -rf $(ASM_OBJ) $(C_OBJ)
	rm -rf $(BIN)
