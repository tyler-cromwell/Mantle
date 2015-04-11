#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>

extern char* itoa(long);
extern char* strncpy(char*, const char*, size_t);
extern size_t strlen(const char*);

#endif
