# Programs for building
CC = clang

# Respective flags
CPPFLAGS = -I $(ROOT)/include -I $(ROOT)/arch/include
CFLAGS = -nostdlib -Wall -Werror -Wno-unused-command-line-argument -pedantic -m32 -O0

# Debugging options
CPPFLAGS +=

# C source files
C_SRC = $(shell ls *.c)
C_OBJ = $(C_SRC:%.c=%.o)

# Rule for compiling .c files
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: all
all: $(C_OBJ)
