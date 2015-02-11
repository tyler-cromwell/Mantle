#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include <stddef.h>
#include <stdint.h>

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

extern void console_clear(void);
extern size_t console_write(char*, size_t, uint8_t);
extern size_t console_write_line(char*, size_t, uint8_t);

#endif
