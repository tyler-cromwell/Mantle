/**********************************************************************
  Copyright (C) 2015 Tyler Cromwell <tjc6185@gmail.com>

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

#ifndef KERNEL_H
#define KERNEL_H

/* Linker Script Symbols */
extern struct undefined KERNEL_LMA;
extern struct undefined KERNEL_END;
extern struct undefined KERNEL_SIZE;
extern struct undefined BOOTSTRAP;
extern struct undefined SECTION_TEXT;
extern struct undefined SECTION_DATA;
extern struct undefined SECTION_BSS;

#endif
