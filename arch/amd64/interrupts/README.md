# Interrupts
This directory contains code for interrupt handlers and the IDT.

## Contents
- `exceptions.asm` - Implements the (Assembly half) the system Exception handlers.
- `idt.c` - Defines a 64-bit Interrupt Descriptor Table and installs handlers (implements C half).
- `idt.h` - Struct definitions for the Interrupt Descriptor Table and interrupt handling.
- `interrupts.h` - Forward declarations for all interrupt handlers in C.
- `irqs.asm` - Implements the Interrupt Request handlers.
