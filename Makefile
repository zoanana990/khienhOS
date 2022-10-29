all: ./boot.asm
	nasm -f bin ./boot.asm -o ./boot.bin
	dd if=./message.txt >> ./boot.bin
	dd if=/dev/zero bs=512 count=1 >> ./boot.bin
qemu: ./boot.bin
	qemu-system-x86_64 -hda ./boot.bin
clean:
	rm -f *.bin
color: color.c
	gcc -o color color.c
	./color
	rm -f color