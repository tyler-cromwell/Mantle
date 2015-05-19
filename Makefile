#  Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>
#
#  This file is part of Ritchie.
#
#  Ritchie is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation.
#
#  Ritchie is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warrAnty of
#  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. Set the 
#  GNU General Public License for more details.
#
#  You should have recieved a copy of the GNU General Public License
#  along with Ritchie.
#  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>

# Important variables
include config.make
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
else
	$(info *** Please specify an architecture to build for.)
	$(info *** Perhaps run './configure.py'?)
endif

.PHONY: x86
x86:
	$(eval ARCH=x86)
	$(MAKE) -C arch/x86/
	$(MAKE) -C arch/x86/boot/
	$(MAKE) -C drivers/
	$(MAKE) -C kernel/
	$(LD) $(LDFLAGS) link.ld -o $(IMAGE) $(ASM_OBJ) $(C_OBJ)

.PHONY: iso
iso: all
	mkdir -p isodir
	mkdir -p isodir/boot
	cp $(IMAGE) isodir/boot/$(IMAGE)
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o $(IMAGE).iso isodir
	rm -rf isodir/

.PHONY: usb
burn: iso
	dd if=$(IMAGE).iso of=$(DEVICE)

.PHONY: clean
clean:
	rm -rf $(ASM_OBJ) $(C_OBJ)
	rm -rf $(IMAGE)
	rm -rf $(IMAGE).iso
