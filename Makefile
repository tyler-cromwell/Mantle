# Important variables
ARCH = $(shell head -1 make.conf | tail -1)
BDEV = $(shell head -2 make.conf | tail -1)
export ARCH

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

# Root directory
ROOT = $(shell pwd)
export ROOT

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
	$(LD) $(LDFLAGS) link.ld -o ritchie $(ASM_OBJ) $(C_OBJ)

.PHONY: iso
iso: all
	mkdir -p isodir
	mkdir -p isodir/boot
	cp ritchie isodir/boot/ritchie
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o ritchie.iso isodir
	rm -rf isodir/

.PHONY: usb
burn: iso
	dd if=ritchie.iso of=$(BDEV)

.PHONY: clean
clean:
	rm -rf $(ASM_OBJ) $(C_OBJ)
	rm -rf ritchie
	rm -rf ritchie.iso
