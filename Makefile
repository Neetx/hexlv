CC = gcc
AS = nasm
CFLAGS = -std=gnu99 -O2 -nostdlib -ffreestanding -Wall -Wextra -m64 -Iinclude/lib/
ASFLAGS = -f elf64
OBJECTS =  build/multiboot_header.o build/boot.o build/kernel.o build/test_library.o

default: run

.PHONY: default build run clean

build/multiboot_header.o: src/asm/multiboot_header.asm
	mkdir -p build
	$(AS) $(ASFLAGS) src/asm/multiboot_header.asm -o build/multiboot_header.o

build/boot.o: src/asm/boot.asm
	mkdir -p build
	$(AS) -f elf64 src/asm/boot.asm -o build/boot.o

build/test_library.o: src/lib/test_library.c
	$(CC) -c src/lib/test_library.c -o build/test_library.o $(CFLAGS)

build/kernel.o: src/kernel/kernel.c
	$(CC) -c src/kernel/kernel.c -o build/kernel.o $(CFLAGS)
build/kernel.bin: linker.ld $(OBJECTS)
	ld -n -o build/kernel.bin -T linker.ld $(OBJECTS)

build/hexlv.iso: build/kernel.bin grub.cfg
	mkdir -p build/isodir/boot/grub
	cp grub.cfg build/isodir/boot/grub
	cp build/kernel.bin build/isodir/boot/
	grub-mkrescue -o build/hexlv.iso build/isodir

run: build/hexlv.iso

build: build/hexlv.iso

clean:
	rm -rf build