AS=nasm
CC=clang
LD=ld

ASFLAGS=-f elf32
CFLAGS=-Wall -Werror -pedantic -m32 -c -O0 -I ./include
LDFLAGS=-m elf_i386 -T

CMD=qemu-system-i386
CMDFLAGS=-kernel

IMAGE=kernel_image

.PHONY: all
all:
	$(AS) $(ASFLAGS) boot/start.asm -o boot/start.o
	$(CC) $(CFLAGS) kernel.c
	$(CC) $(CFLAGS) drivers/console.c -o drivers/console.o
	$(LD) $(LDFLAGS) link.ld -o $(IMAGE) kernel.o boot/start.o drivers/console.o

# Because I don't want to always type this...
.PHONY: run
run:
	$(CMD) $(CMDFLAGS) $(IMAGE)

.PHONY: clean
clean:
	rm -rf ./*.o ./boot/*.o ./drivers/*.o

.PHONY: clean-image
clean-image: clean
	rm -f $(IMAGE)
