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

#ifndef AMD64_CONSOLE_H
#define AMD64_CONSOLE_H

/* C Standard Library header(s) */
#include <stddef.h>
#include <stdint.h>

/* Kernel header(s) */
#include <kernel/types.h>

/* Text colors */
#define FG_BLACK        0x00
#define FG_BLUE         0x01
#define FG_GREEN        0x02
#define FG_CYAN         0x03
#define FG_RED          0x04
#define FG_MAGENTA      0x05
#define FG_BROWN        0x06
#define FG_GREY_L       0x07
#define FG_GREY         0x08
#define FG_BLUE_L       0x09
#define FG_GREEN_L      0x0a
#define FG_CYAN_L       0x0b
#define FG_RED_L        0x0c
#define FG_MAGENTA_L    0x0d
#define FG_BROWN_L      0x0e
#define FG_WHITE        0x0f

/* Background highlighting */
#define BG_BLUE         0x10
#define BG_GREEN        0x20
#define BG_CYAN         0x30
#define BG_RED          0x40
#define BG_MAGENTA      0x50
#define BG_BROWN        0x60
#define BG_GREY_L       0x70
#define BG_GREY         0x80
#define BG_BLUE_L       0x90
#define BG_GREEN_L      0xa0
#define BG_CYAN_L       0xb0
#define BG_RED_L        0xc0
#define BG_MAGENTA_L    0xd0
#define BG_BROWN_L      0xe0
#define BG_WHITE        0xf0

/* Function prototypes */
void console_clear(void);
void console_set_background(uchar_t color);
size_t console_write(uchar_t color, char *message, size_t length);
size_t console_printf(uchar_t color, char *format, ...);

#endif
