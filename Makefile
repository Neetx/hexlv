default: run

.PHONY: default build run clean

build/multiboot_header.o: src/asm/multiboot_header.asm
	mkdir -p build
	nasm -f elf64 src/asm/multiboot_header.asm -o build/multiboot_header.o

build/boot.o: src/asm/boot.asm
	mkdir -p build
	nasm -f elf64 src/asm/boot.asm -o build/boot.o

build/test_library.o: src/lib/test_library.c
	gcc -c src/lib/test_library.c -o build/test_library.o -std=gnu99 -ffreestanding -O2 -Wall -m64 -Wextra

build/kernel.o: src/kernel/kernel.c
	gcc -c src/kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -m64 -Wextra

build/kernel.bin: build/multiboot_header.o build/boot.o linker.ld build/kernel.o build/test_library.o
	ld -n -o build/kernel.bin -T linker.ld build/multiboot_header.o build/boot.o build/kernel.o build/test_library.o

build/hexlv.iso: build/kernel.bin grub.cfg
	mkdir -p build/isodir/boot/grub
	cp grub.cfg build/isodir/boot/grub
	cp build/kernel.bin build/isodir/boot/
	grub-mkrescue -o build/hexlv.iso build/isodir

run: build/hexlv.iso

build: build/hexlv.iso

clean:
	rm -rf build
