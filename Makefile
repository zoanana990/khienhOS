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
IO_SRC        = $(SRC)/io

IDT_BUILD     = $(BUILD)/idt
MEMORY_BUILD  = $(BUILD)/memory
IO_BUILD      = $(BUILD)/io

# files
KERNEL        = $(BIN)/kernel.bin
BOOT          = $(BIN)/boot.bin
OS            = $(BIN)/os.bin
LINKER        = $(SRC)/linker.ld
KERNEL_FILES  = $(BUILD)/kernel.asm.o    \
                $(BUILD)/kernel.o        \
                $(IDT_BUILD)/idt.asm.o   \
                $(IDT_BUILD)/idt.o       \
				$(IO_BUILD)/io.asm.o     \
                $(MEMORY_BUILD)/memory.o \
                $(MEMORY_BUILD)/heap.o   \
                $(MEMORY_BUILD)/kheap.o  \
                $(MEMORY_BUILD)/page.o

# refer to the arm_hal project
# TODO: refine the makefile with makefile functions
C_SOURCES     = $(wildcard $(SRC)/*.c)          \
                $(wildcard $(BOOT_SRC)/*.c)     \
				$(wildcard $(IDT_SRC)/*.c)      \
				$(wildcard $(MEMORY_SRC)/*.c)   \
ASM_SOURCES   = $(wildcard $(SRC)/*.asm)        \
				$(wildcard $(BOOT_SRC)/*.asm)   \
				$(wildcard $(IDT_SRC)/*.asm)    \
				$(wildcard $(MEMORY_SRC)/*.asm) \


#############################################
# COMPILE TOOLS AND FLAGS
#############################################
# compile flags and tools
PREFIX = i686-elf-
CC     = $(PREFIX)gcc
LD     = $(PREFIX)ld

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
# list of objects
OBJECTS = $(addprefix $(BUILD)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

all: clean $(BOOT) $(KERNEL)
	# read the file instead to use stdin
	# then we output to the os.bin
	dd if=$(BOOT) >> $(BIN)/os.bin
	dd if=$(KERNEL) >> $(BIN)/os.bin

	# due to the padding issue, we need to padding zero to 512 bytes
	dd if=/dev/zero bs=512 count=100 >> $(BIN)/os.bin

$(KERNEL): $(KERNEL_FILES)
	$(shell if [ ! -e $(BIN) ];then mkdir -p $(BIN); fi)
	$(LD) $(LDFLAGS) $^ -o $(BUILD)/kernelfull.o
	$(CC) -T $(LINKER) $(CFLAGS) $(BUILD)/kernelfull.o -o $@

$(BOOT): $(BOOT_SRC)/boot.asm
	$(shell if [ ! -e $(BUILD) ];then mkdir -p $(BUILD); fi)
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(BUILD)/kernel.asm.o: $(SRC)/kernel.asm
	nasm -f elf -g $^ -o $@

$(IDT_BUILD)/idt.asm.o: $(IDT_SRC)/idt.asm
	$(shell if [ ! -e $(IDT_BUILD) ];then mkdir -p $(IDT_BUILD); fi)
	nasm -f elf -g $^ -o $@

$(IO_BUILD)/io.asm.o: $(IO_SRC)/io.asm
	$(shell if [ ! -e $(IO_BUILD) ];then mkdir -p $(IO_BUILD); fi)
	nasm -f elf -g $^ -o $@

$(BUILD)/%.o: %.c Makefile | $(BUILD)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	#TODO: recursive remove the files
	rm -rf $(BIN)/*.bin
	rm -rf $(BUILD)/*.o
	rm -rf $(BUILD)/idt/*.o
	rm -rf $(BUILD)/memory/*.o

#############################################
# OTHER TOOLS
#############################################
qemu:
	qemu-system-x86_64 -hda $(OS)

asm:
	ndisasm $(BIN)/kernel.bin

bless:
	bless $(BIN)/os.bin