# Programs for building
LD = ld

# Respective flags
LDFLAGS = -m elf_i386 -T

# Block device to burn ISO to
BDEV = 

# Object files
OBJS = $(shell find . -name *.o)
OBJS = $(OBJS:%.o=%.o)

# Kernel image name
BIN = ritchie_debug

# Root directory
ROOT = $(shell pwd)
export ROOT

# QEMU options
QEMU = qemu-system-x86_64
QEMUFLAGS = -monitor stdio -m 4G

.PHONY: x86
x86:
	$(MAKE) -C arch/x86/
	$(MAKE) -C arch/x86/boot/
	$(MAKE) -C drivers/
	$(MAKE) -C kernel/
	$(LD) $(LDFLAGS) link.ld -o $(BIN) $(OBJS)

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
	rm -rf $(OBJS)
	rm -rf $(BIN)
	rm -rf $(BIN).iso
