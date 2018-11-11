CC = gcc
AS = nasm
CFLAGS = -std=gnu99 -O2 -nostdlib -ffreestanding -Wall -Wextra -m64 -Ikernel/include/ -Ilibk/include/
ASFLAGS = -f elf64
OBJECTS =  build/multiboot_header.o build/boot.o build/kernel.o build/tty.o build/io.o build/serial.o build/keyboard.o
LIBK_OBJECTS = build/libk/strlen.o build/libk/strcmp.o build/libk/memset.o build/libk/memmove.o build/libk/memcpy.o build/libk/memcmp.o

default: run

.PHONY: default build run clean

build/multiboot_header.o: kernel/arch/x86-64/multiboot_header.asm
	mkdir -p build
	$(AS) $(ASFLAGS) kernel/arch/x86-64/multiboot_header.asm -o build/multiboot_header.o

build/boot.o: kernel/arch/x86-64/boot.asm
	mkdir -p build
	$(AS) -f elf64 kernel/arch/x86-64/boot.asm -o build/boot.o

build/io.o: kernel/arch/x86-64/io.c
	$(CC) -c kernel/arch/x86-64/io.c -o build/io.o $(CFLAGS)

build/tty.o: kernel/arch/x86-64/tty.c
	$(CC) -c kernel/arch/x86-64/tty.c -o build/tty.o $(CFLAGS)

build/serial.o: kernel/include/kernel/serial.c
	$(CC) -c kernel/include/kernel/serial.c -o build/serial.o $(CFLAGS)

build/libk/strlen.o: libk/string/strlen.c
	mkdir -p build/libk/
	$(CC) -c libk/string/strlen.c -o build/libk/strlen.o $(CFLAGS)

build/libk/strcmp.o: libk/string/strcmp.c
	mkdir -p build/libk/
	$(CC) -c libk/string/strcmp.c -o build/libk/strcmp.o $(CFLAGS)

build/libk/memset.o: libk/string/memset.c
	mkdir -p build/libk/
	$(CC) -c libk/string/memset.c -o build/libk/memset.o $(CFLAGS)

build/libk/memmove.o: libk/string/memmove.c
	mkdir -p build/libk/
	$(CC) -c libk/string/memmove.c -o build/libk/memmove.o $(CFLAGS)

build/libk/memcpy.o: libk/string/memcpy.c
	mkdir -p build/libk/
	$(CC) -c libk/string/memcpy.c -o build/libk/memcpy.o $(CFLAGS)

build/libk/memcmp.o: libk/string/memcmp.c
	mkdir -p build/libk/
	$(CC) -c libk/string/memcmp.c -o build/libk/memcmp.o $(CFLAGS)

build/keyboard.o: kernel/include/kernel/keyboard.c
	$(CC) -c kernel/include/kernel/keyboard.c -o build/keyboard.o $(CFLAGS)

build/kernel.o: kernel/kernel/kernel.c
	$(CC) -c kernel/kernel/kernel.c -o build/kernel.o $(CFLAGS)

build/kernel.bin: linker.ld $(OBJECTS) $(LIBK_OBJECTS)
	ld -n -o build/kernel.bin -T linker.ld $(OBJECTS) $(LIBK_OBJECTS)

build/hexlv.iso: build/kernel.bin grub.cfg
	mkdir -p build/isodir/boot/grub
	cp grub.cfg build/isodir/boot/grub
	cp build/kernel.bin build/isodir/boot/
	grub-mkrescue -o build/hexlv.iso build/isodir

run: build/hexlv.iso

build: build/hexlv.iso

clean:
	rm -rf build