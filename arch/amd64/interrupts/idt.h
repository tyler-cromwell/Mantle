/**********************************************************************
  Copyright (C) 2016-Present Tyler Cromwell <tjc6185@gmail.com>

  This file is part of Mantle.

  Mantle is free software: you can redistribute it and/or modify
  it under Version 2 of the terms of the GNU General Public License
  as published by the Free Software Foundation.

  Mantle is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Mantle.
  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
**********************************************************************/

#ifndef AMD64_IDT_H
#define AMD64_IDT_H

// Kernel header(s)
#include <kernel/types.h>


// A Long Mode IDT interrupt gate
struct IdtGate {
    word_t offset_low;      // Lower portion of the offset
    word_t selector;        // Interrupt selector
    byte_t zero1;           // Always zero
    byte_t type_attr;       // Type attributes
    word_t offset_middle;   // Middle portion of the offset
    dword_t offset_high;    // Upper portion of the offset
    dword_t zero2;          // Always zero
} __attribute__((__packed__));


// The Long Mode IDTR register
struct Idtr {
    word_t limit;   // The length of the IDT
    qword_t base;   // IDT base address
} __attribute__((__packed__));


// Selector Error code
struct SelectorError {
    dword_t ext         : 1;
    dword_t idt         : 1;
    dword_t ti          : 1;
    dword_t index       : 13;
    dword_t reserved    : 16;
} __attribute__((__packed__));


// Page Fault Error code
struct PageFaultError {
    dword_t p           : 1;
    dword_t rw          : 1;
    dword_t us          : 1;
    dword_t rsv         : 1;
    dword_t id          : 1;
    dword_t reserved    : 27;
} __attribute__((__packed__));


// Register states before Interrupt
struct InterruptStack {
    qword_t ds;
    qword_t r15, r14, r13, r12, r11, r10, r9, r8;
    qword_t rdi, rsi, rbp, rsp;
    qword_t rdx, rcx, rbx, rax;
    qword_t vector, error;
    qword_t rip, cs, rflags, retrsp, retss;
} __attribute__((__packed__));

#endif
