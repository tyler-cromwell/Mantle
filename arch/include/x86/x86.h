/* Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

   This file is part of Ritchie.

   Ritchie is free software: you can redistribute it and/or modify
   it under Version 2 of the terms of the GNU General Public License
   as published by the Free Software Foundation.

   Ritchie is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Ritchie.
   If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
*/

#ifndef X86_H
#define X86_H

/* CPUID */
extern void cpuid_vendor(char*);

/* Global Descriptor Table */
extern void gdt_init(void);

/* Interrupt Descriptor Table */
extern void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);
extern void idt_init(void);

#endif
