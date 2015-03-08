# Boot
This directory contains all code necessary for booting/starting the Kernel.

## Contents
- `longmode.c` - Provides the functions for switching into Long mode.
- `start.asm` - Checks if the CPU supports Long mode, then jumps into the kernel. Follows the Multiboot specification.
