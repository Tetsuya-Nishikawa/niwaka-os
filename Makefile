all: ./fs/EFI/BOOT/BOOTIA32.EFI

OBJS = font.o consol.o pic.o keyboard.o

run:console.c pic.c keyboard.c font.c niwaka-os.c memory.c apic.c gdt-idt.c asmlib.asm main.c niwaka-os.ls Makefile
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o console.o console.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o pic.o pic.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o keyboard.o keyboard.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o font.o font.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o niwaka-os.o niwaka-os.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o apic.o apic.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o gdt-idt.o gdt-idt.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o memory.o memory.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o proc.o proc.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o floppy.o floppy.c -Wall
	/usr/local/bin/i386-elf-gcc -c -O0  -fno-pic -o dma.o dma.c -Wall

	/usr/local/bin/nasm -f elf32 -o asmlib.o asmlib.asm
	/usr/local/bin/i386-elf-ld -O0 -nostdlib -nostartfiles -T niwaka-os.ls -o niwaka-os.bin dma.o floppy.o niwaka-os.o proc.o memory.o asmlib.o font.o apic.o gdt-idt.o console.o pic.o keyboard.o 
	mkdir -p fs/EFI/BOOT
	i686-w64-mingw32-gcc -O0 -m32 -Wall -Wextra -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -o main.efi main.c -shared

	cp main.efi ./fs/EFI/BOOT/BOOTIA32.EFI
	rm ./fs/niwaka-os.bin
	cp niwaka-os.bin ./fs/niwaka-os.bin
	qemu-system-i386 -m 4G -bios OVMF.fd -hda fat:rw:fs -fda fat:rw:fs
clean:
	rm *.o
	rm *.bin 
	rm main.efi

.PHONY: clean