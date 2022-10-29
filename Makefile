all: ./boot.asm
	nasm -f bin ./boot.asm -o ./boot.bin
qemu: ./boot.bin
	qemu-system-x86_64 -hda ./boot.bin
clean:
	rm -f *.bin
color: color.c
	gcc -o color color.c
	./color
	rm -f color