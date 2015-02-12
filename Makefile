AS=nasm
CC=clang
LD=ld

ASFLAGS=-f elf32
CFLAGS=-Wall -Werror -pedantic -m32 -c -O0 -I ./include
LDFLAGS=-m elf_i386 -T

CMD=qemu-system-i386
CMDFLAGS=-kernel

IMAGE=humboldt_image

.PHONY: all
all:
	$(AS) $(ASFLAGS) boot/start.asm -o boot/start.o
	$(CC) $(CFLAGS) kernel/kernel.c -o kernel/kernel.o
	$(CC) $(CFLAGS) drivers/console.c -o drivers/console.o
	$(CC) $(CFLAGS) lib/string.c -o lib/string.o
	$(LD) $(LDFLAGS) link.ld -o $(IMAGE) boot/start.o drivers/console.o kernel/kernel.o lib/string.o

# Because I don't want to always type this...
.PHONY: run
run:
	$(CMD) $(CMDFLAGS) $(IMAGE)

.PHONY: clean
clean:
	rm -rf ./boot/*.o
	rm -rf ./drivers/*.o
	rm -rf ./kernel/*.o
	rm -rf ./lib/*.o 

.PHONY: clean-image
clean-image: clean
	rm -f $(IMAGE)
