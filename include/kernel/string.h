#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>
#include <stdint.h>

extern char* itoa(int32_t, int32_t);
extern int32_t strncmp(const char*, const char*, size_t);
extern char* strncpy(char*, const char*, size_t);
extern size_t strlen(const char*);
extern size_t strnlen(const char*, size_t);

#endif
