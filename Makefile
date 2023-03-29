#############################################
# DIRECTORY AND FILES
#############################################

# directory
SRC           = ./src
BOOT_SRC      = $(SRC)/boot

# files
KERNEL        = ./bin/kernel.bin
FILES         = ./build/kernel.asm.o
BOOT          = ./bin/boot.bin
OS            = ./bin/os.bin
LINKER        = $(SRC)/linker.ld

#############################################
# COMPILE TOOLS AND FLAGS
#############################################
# compile flags and tools
PREFIX = i686-elf-
GCC = gcc
LD = ld

# -ffreestanding flags is one in which the standard library may not exist, 
# and the program startup may not nexessarily be at main
# The option -ffreestanding directs the compiler to not assume 
# that standard functions have their usual definition.
CFLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce \
         -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -nodefaultlibs -O0     \
         -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -Wall -Iinc  
LDFLAGS = -g -relocatable

#############################################
# BUILD
#############################################
all: clean $(BOOT) $(KERNEL)
    # read the file instead to use stdin
    # then we output to the os.bin
	dd if=$(BOOT) >> ./bin/os.bin
	dd if=$(KERNEL) >> ./bin/os.bin

    # due to the padding issue, we need to padding zero to 512 bytes
	dd if=/dev/zero bs=512 count=10 >> ./bin/os.bin

$(KERNEL): $(FILES)
	$(PREFIX)$(LD) $(LDFLAGS) $(FILES) -o ./build/kernelfull.o
	$(PREFIX)$(GCC) -T $(LINKER) $(CFLAGS) ./build/kernelfull.o -o $(KERNEL) 

$(BOOT): $(BOOT_SRC)/boot.asm
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(FILES): $(SRC)/kernel.asm
	nasm -f elf -g $(SRC)/kernel.asm -o $(FILES)

clean:
	rm -rf ./bin/*.bin
	rm -rf ./build/*.o

#############################################
# OTHER TOOLS
#############################################
qemu:
	qemu-system-x86_64 -hda $(OS)

asm:
	ndisasm ./kernel.bin

bless:
	bless ./bin/os.bin