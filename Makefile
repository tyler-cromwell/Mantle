# Programs for building
LD = ld

# Respective flags
LDFLAGS = -m elf_i386 -T

# Assembly source files
ASM_SRC = $(shell find . -name *.asm)
ASM_OBJ = $(ASM_SRC:%.asm=%.o)

# C source files
C_SRC = $(shell find . -name *.c)
C_OBJ = $(C_SRC:%.c=%.o)

# Kernel image name
BIN = ritchie_$(ARCH)_debug

# Root directory
ROOT = $(shell pwd)
export ROOT

# QEMU options
QEMU = qemu-system-x86_64
QEMUFLAGS = -monitor stdio -m 4G

.PHONY: all
all:
ifeq ($(ARCH),x86)
	$(MAKE) x86
endif

.PHONY: x86
x86:
	$(MAKE) -C arch/x86/
	$(MAKE) -C arch/x86/boot/
	$(MAKE) -C drivers/
	$(MAKE) -C kernel/
	$(LD) $(LDFLAGS) link.ld -o $(BIN) $(ASM_OBJ) $(C_OBJ)

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
