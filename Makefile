# we need to seperate the bootloader and the kernel

## DIRECTORY
FILES = ./build/kernel.asm.o
BOOT = ./bin/boot.bin
BOOT_SRC = ./src/boot
SRC = ./src

## COMPILE


all: $(BOOT) $(FILES)

# read the file instead to use stdin
# then we output to the os.bin
	dd if=$(BOOT) >> ./bin/os.bin

$(BOOT): $(BOOT_SRC)/boot.asm
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(FILES): $(SRC)/kernel.asm
	nasm -f bin $(SRC)/kernel.asm -o $(FILES)

qemu:
	qemu-system-x86_64 -hda $(BOOT)

clean:
	rm -rf ./bin/*.bin
	rm -rf ./build/*.o