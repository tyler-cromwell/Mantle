# x86
This directory contains all x86 specific code.

## Contents

#### Directories
- `boot` - Contains code needed for booting/starting the Kernel on an x86 machine.

#### Files
- `cpuid.c` - Functions that make use of the CPUID instruction.
- `exceptions.asm` - Implements the system Exception handlers.
- `gdt.c` - Functions for initializing the Global Descriptor Table.
- `idt.c` - Defines the Interrupt Descriptor Table.
- `interrupts.h` - Forward declarations for all interrupt handlers in C.
- `irqs.asm` - Implements the Interrupt Request handlers.
