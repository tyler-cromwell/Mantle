# x86
This directory contains all x86 specific code.

## Contents

#### Directories
- `boot` - Contains code needed for booting/starting the Kernel on an x86 machine.

#### Files
- `cpuid.c` - Functions that make use of the CPUID instruction.
- `idt.c` - Defines the Interrupt Descriptor Table.
- `isrs.asm` - Implements the system Interrupt Service Routines.
- `isrs.h` - Forward declarations for ISRs in C.
- `gdt.c` - Functions for initializing the Global Descriptor Table.
