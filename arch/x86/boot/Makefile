# Programs for building
AS = nasm
CC = clang

# Respective flags
ASFLAGS = -f elf32
CPPFLAGS = -I $(ROOT)/include -I $(ROOT)/arch/include
CFLAGS = -nostdlib -Wall -Werror -Wno-unused-command-line-argument -pedantic -m32 -O0

# Debugging options
CPPFLAGS += 

# C source files
C_SRC = $(shell ls *.c)
C_OBJ = $(C_SRC:%.c=%.o)

# ASM source files
ASM_SRC = $(shell ls *.asm)
ASM_OBJ = $(ASM_SRC:%.asm=%.o)

# Rule for compiling .c files
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Rule for compiling .asm files
%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: all
all: $(ASM_OBJ) $(C_OBJ)
