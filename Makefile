#############################################
# source
#############################################
INCLUDES      = -Iinc

#############################################
# DIRECTORY AND FILES
#############################################
# directory
SRC           = ./src
BUILD         = ./build
BIN           = ./bin
BOOT_SRC      = $(SRC)/boot
IDT_SRC       = $(SRC)/idt
MEMORY_SRC    = $(SRC)/memory
IDT_BUILD     = $(BUILD)/idt
MEMORY_BUILD  = $(BUILD)/memory

# files
KERNEL        = $(BIN)/kernel.bin
BOOT          = $(BIN)/boot.bin
OS            = $(BIN)/os.bin
LINKER        = $(SRC)/linker.ld
KERNEL_FILES  = $(BUILD)/kernel.asm.o    \
                $(BUILD)/kernel.o        \
                $(IDT_BUILD)/idt.asm.o   \
                $(IDT_BUILD)/idt.o       \
                $(MEMORY_BUILD)/memory.o \

#############################################
# COMPILE TOOLS AND FLAGS
#############################################
# compile flags and tools
PREFIX = i686-elf-
GCC    = gcc
LD     = ld

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

	# build the directory if the folder is not exist
	$(shell if [ ! -e $(BIN) ];then mkdir -p $(BIN); fi)
	$(shell if [ ! -e $(BUILD) ];then mkdir -p $(BUILD); fi)
	$(shell if [ ! -e $(IDT_BUILD) ];then mkdir -p $(IDT_BUILD); fi)
	$(shell if [ ! -e $(MEMORY_BUILD) ];then mkdir -p $(MEMORY_BUILD); fi)

	# read the file instead to use stdin
	# then we output to the os.bin
	dd if=$(BOOT) >> $(BIN)/os.bin
	dd if=$(KERNEL) >> $(BIN)/os.bin

	# due to the padding issue, we need to padding zero to 512 bytes
	dd if=/dev/zero bs=512 count=100 >> $(BIN)/os.bin

$(KERNEL): $(KERNEL_FILES)
	$(PREFIX)$(LD) $(LDFLAGS) $^ -o $(BUILD)/kernelfull.o
	$(PREFIX)$(GCC) -T $(LINKER) $(CFLAGS) $(BUILD)/kernelfull.o -o $@

$(BOOT): $(BOOT_SRC)/boot.asm
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(BUILD)/kernel.asm.o: $(SRC)/kernel.asm
	nasm -f elf -g $^ -o $@

$(IDT_BUILD)/idt.asm.o: $(IDT_SRC)/idt.asm
	nasm -f elf -g $^ -o $@

$(BUILD)/kernel.o : $(SRC)/kernel.c
	$(PREFIX)$(GCC) $(INCLUDES) $(CFLAGS) -std=gnu99 -c $^ -o $@

$(IDT_BUILD)/idt.o : $(IDT_SRC)/idt.c
	$(PREFIX)$(GCC) $(INCLUDES) $(CFLAGS) -std=gnu99 -c $^ -o $@

$(MEMORY_BUILD)/memory.o : $(MEMORY_SRC)/memory.c
	$(PREFIX)$(GCC) $(INCLUDES) $(CFLAGS) -std=gnu99 -c $^ -o $@

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