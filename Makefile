#############################################
# DIRECTORY AND FILES
#############################################

# directory
SRC           = ./src
BUILD         = ./build
BIN           = ./bin
BOOT_SRC      = $(SRC)/boot

# files
KERNEL        = $(BIN)/kernel.bin
BOOT          = $(BIN)/boot.bin
OS            = $(BIN)/os.bin
LINKER        = $(SRC)/linker.ld
KERNEL_ASM    = $(BUILD)/kernel.asm.o

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
	dd if=$(BOOT) >> $(BIN)/os.bin
	dd if=$(KERNEL) >> $(BIN)/os.bin

    # due to the padding issue, we need to padding zero to 512 bytes
	dd if=/dev/zero bs=512 count=10 >> $(BIN)/os.bin

$(KERNEL): $(KERNEL_ASM)
	$(PREFIX)$(LD) $(LDFLAGS) $(KERNEL_ASM) -o .$(BUILD)/kernelfull.o
	$(PREFIX)$(GCC) -T $(LINKER) $(CFLAGS) $(BUILD)/kernelfull.o -o $(KERNEL) 

$(BOOT): $(BOOT_SRC)/boot.asm
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(KERNEL_ASM): $(SRC)/kernel.asm
	nasm -f elf -g $(SRC)/kernel.asm -o $(KERNEL_ASM)

clean:
	rm -rf $(BIN)/*.bin
	rm -rf $(BUILD)/*.o

#############################################
# OTHER TOOLS
#############################################
qemu:
	qemu-system-x86_64 -hda $(OS)

asm:
	ndisasm $(BIN)/kernel.bin

bless:
	bless $(BIN)/os.bin