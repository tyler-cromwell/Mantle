# Boot
This directory contains all code necessary for booting/starting the Kernel.

## Contents
- `multiboot.c` - Functions for manipulating Multiboot information.
- `boot.asm` - Declares a Multiboot header, boots into Long Mode, then jumps into the kernel.
