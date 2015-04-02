# Programs for building
AS = nasm
CC = clang
LD = ld

# Respective flags
ASFLAGS = -f elf32
CPPFLAGS = -Iinclude -Iarch/include
CFLAGS = -nostdlib -Wall -Werror -Wno-unused-command-line-argument -pedantic -m32 -O0
LDFLAGS = -m elf_i386 -T

# Debugging options
ASFLAGS += -D_FILE_=\"$(<F)\"
CPPFLAGS += -D_FILE_=\"$(<F)\"

# Assembly source files
ASM_SRC = $(shell find . -name *.asm)
ASM_OBJ = $(ASM_SRC:%.asm=%.o)

# C source files
C_SRC = $(shell find . -name *.c)
C_OBJ = $(C_SRC:%.c=%.o)

# Kernel image name
BIN = humboldt_debug

# QEMU options
QEMU = qemu-system-x86_64
QEMUFLAGS = -monitor stdio -kernel $(BIN)

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
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: qemu
qemu: all
	$(QEMU) $(QEMUFLAGS)

.PHONY: clean
clean:
	rm -rf $(ASM_OBJ) $(C_OBJ)
	rm -rf $(BIN)
