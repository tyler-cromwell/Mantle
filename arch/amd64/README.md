# AMD64
This directory contains all AMD64 specific code.

## Contents

#### Directories
- `boot` -  AMD64 bootup code.
- `drivers` - Drivers for hardware found on an AMD64 machine.
- `interrupts` - Interrupt handlers and the IDT.
- `memory` - Memory management and Virtual memory.

#### Files
- `cpuid.c` - Functions that make use of the CPUID instruction.
- `init.c` - Early stage kernel code for AMD64.
- `shell.c` - Implements a CLI shell.
