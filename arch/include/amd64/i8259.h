/**********************************************************************
  Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

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
**********************************************************************/

#ifndef AMD64_I8259_H
#define AMD64_I8259_H

/* Master PIC */
#define I8259_MASTER_CMD    0x20
#define I8259_MASTER_DATA   0x21

/* Slave PIC */
#define I8259_SLAVE_CMD     0xA0
#define I8259_SLAVE_DATA    0xA1

extern void i8259_init(void);
extern void i8259_mask(void);
extern void i8259_unmask(void);

#endif
