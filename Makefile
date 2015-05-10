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
CPPFLAGS += 

# Block device to burn ISO to
BDEV = 

# Assembly source files
ASM_SRC = $(shell find . -name *.asm)
ASM_OBJ = $(ASM_SRC:%.asm=%.o)

# C source files
C_SRC = $(shell find . -name *.c)
C_OBJ = $(C_SRC:%.c=%.o)

# Kernel image name
BIN = ritchie_debug

# QEMU options
QEMU = qemu-system-x86_64
QEMUFLAGS = -monitor stdio -m 4G

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

.PHONY: iso
iso: all
	mkdir -p isodir
	mkdir -p isodir/boot
	cp $(BIN) isodir/boot/$(BIN)
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o $(BIN).iso isodir
	rm -rf isodir/

.PHONY: usb
burn: iso
	dd if=$(BIN).iso of=$(BDEV)

.PHONY: qemu
qemu: all
	$(QEMU) $(QEMUFLAGS) -kernel $(BIN)

.PHONY: qemu-iso
qemu-iso: iso
	$(QEMU) $(QEMUFLAGS) $(BIN).iso

.PHONY: clean
clean:
	rm -rf $(ASM_OBJ) $(C_OBJ)
	rm -rf $(BIN)
	rm -rf $(BIN).iso
