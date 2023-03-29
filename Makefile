# we need to seperate the bootloader and the kernel

## DIRECTORY
KERNEL = ./bin/kernel.bin
FILES = ./build/kernel.asm.o
BOOT = ./bin/boot.bin
SRC = ./src
BOOT_SRC = $(SRC)/boot
LINKER = $(SRC)/linker.ld

## COMPILE
PREFIX = i686-elf-
GCC = gcc
LD = ld

# -ffreestanding flags is one in which the standard library may not exist, 
# and the program startup may not nexessarily be at main
# The option -ffreestanding directs the compiler to not assume 
# that standard functions have their usual definition.
CFLAGS = -nostdlib -ffreestanding -O0
LDFLAGS = -g -relocatable

all: clean $(BOOT) $(KERNEL)
	# read the file instead to use stdin
	# then we output to the os.bin
	dd if=$(BOOT) >> ./bin/os.bin
	dd if=$(KERNEL) >> ./bin/os.bin

$(KERNEL): $(FILES)
	$(PREFIX)$(LD) $(LDFLAGS) $(FILES) -o ./build/kernelfull.o
	$(PREFIX)$(GCC) -T $(LINKER) $(CFLAGS) ./build/kernelfull.o -o $(KERNEL) 

$(BOOT): $(BOOT_SRC)/boot.asm
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(FILES): $(SRC)/kernel.asm
	nasm -f elf -g $(SRC)/kernel.asm -o $(FILES)

qemu:
	qemu-system-x86_64 -hda $(BOOT)

asm:
	ndisasm ./kernel.bin

clean:
	rm -rf ./bin/*.bin
	rm -rf ./build/*.o