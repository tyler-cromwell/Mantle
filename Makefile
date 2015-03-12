# Programs for building
AS = nasm
CC = clang
LD = ld

# Respective flags
ASFLAGS = -f elf32 -D _FILE_=\"$(<F)\"
CFLAGS = -nostdlib -Wall -Werror -pedantic -m32 -O0 -I include -I arch/include -D _FILE_=\"$(<F)\"
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
QEMU = qemu-system-x86_64
QEMUFLAGS = -kernel

.PHONY: all
all: $(BIN)

# Main rule for compiling the kernel
$(BIN): $(ASM_OBJ) $(C_OBJ)
	$(LD) $(LDFLAGS) link.ld -o $(BIN) $(ASM_OBJ) $(C_OBJ)

# Rule for compiling .asm files
%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

# Rule for compiling .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: qemu
qemu: all
	$(QEMU) $(QEMUFLAGS) $(BIN)

.PHONY: clean
clean:
	rm -rf $(ASM_OBJ) $(C_OBJ)
	rm -rf $(BIN)
