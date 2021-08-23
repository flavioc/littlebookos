OBJECTS = fb.o loader.o kmain.o io.o serial.o gdt.o gdt_flush.o idt.o interrupt.o
CC = gcc
CFLAGS = -std=c11 -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso

bochs: os.iso
	bochs -f bochsrc.txt -q

qemu: os.iso
	qemu-system-i386 -s -boot d -cdrom os.iso -serial stdio


%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso iso/boot/kernel.elf bochslog.txt
