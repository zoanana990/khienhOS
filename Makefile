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
BIN           = $(BUILD)/bin

BOOT_SRC      = $(SRC)/boot
IDT_SRC       = $(SRC)/idt
MEMORY_SRC    = $(SRC)/memory
IO_SRC        = $(SRC)/io
FS_SRC        = $(SRC)/fs
COMMON_SRC    = $(SRC)/common
DISK_SRC      = $(SRC)/disk

IDT_BUILD     = $(BUILD)/idt
MEMORY_BUILD  = $(BUILD)/memory
IO_BUILD      = $(BUILD)/io
DISK_BUILD    = $(BUILD)/disk
FS_BUILD      = $(BUILD)/fs
COMMON_BUILD  = $(BUILD)/common
DISK_BUILD    = $(BUILD)/disk

# files
KERNEL        = $(BIN)/kernel.bin
BOOT          = $(BIN)/boot.bin
OS            = $(BIN)/os.bin
LINKER        = $(SRC)/linker.ld
KERNEL_FILES  = $(BUILD)/kernel.asm.o       \
                $(BUILD)/kernel.o           \
                $(BUILD)/print.o            \
                $(IDT_BUILD)/idt.asm.o      \
                $(IDT_BUILD)/idt.o          \
                $(IO_BUILD)/io.asm.o        \
                $(MEMORY_BUILD)/memory.o    \
                $(MEMORY_BUILD)/heap.o      \
                $(MEMORY_BUILD)/kheap.o     \
                $(MEMORY_BUILD)/page.o      \
                $(MEMORY_BUILD)/page.asm.o  \
                $(DISK_BUILD)/disk.o        \
                $(FS_BUILD)/parser.o        \
                $(FS_BUILD)/file.o          \
                $(FS_BUILD)/fat16.o         \
                $(DISK_BUILD)/stream.o      \
                $(COMMON_BUILD)/string.o    \

# refer to the arm_hal project
# TODO: refine the makefile with makefile functions
C_SOURCES     = $(wildcard $(SRC)/*.c)          \
                $(wildcard $(BOOT_SRC)/*.c)     \
                $(wildcard $(IDT_SRC)/*.c)      \
                $(wildcard $(MEMORY_SRC)/*.c)   \
                $(wildcard $(FS_SRC)/*.c)       \
                $(wildcard $(COMMON_SRC)/*.c)   \

ASM_SOURCES   = $(wildcard $(SRC)/*.asm)        \
                $(wildcard $(BOOT_SRC)/*.asm)   \
                $(wildcard $(IDT_SRC)/*.asm)    \
                $(wildcard $(MEMORY_SRC)/*.asm)

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

all: clean mkd $(BOOT) $(KERNEL)
	# read the file instead to use stdin
	# then we output to the os.bin
	dd if=$(BOOT) >> $(BIN)/os.bin
	dd if=$(KERNEL) >> $(BIN)/os.bin

	# due to the padding issue, we need to padding zero to 512 bytes
	dd if=/dev/zero bs=1048576 count=16 >> $(BIN)/os.bin
#	sudo mount -t vfat ./bin/os.bin /mnt/d
#	# Copy a file over
#	sudo cp ./hello.txt /mnt/d
#	sudo umount /mnt/d

mkd:
	$(shell if [ ! -e $(BIN) ];then mkdir -p $(BIN); fi)
	$(shell if [ ! -e $(BUILD) ];then mkdir -p $(BUILD); fi)
	$(shell if [ ! -e $(IDT_BUILD) ];then mkdir -p $(IDT_BUILD); fi)
	$(shell if [ ! -e $(IO_BUILD) ];then mkdir -p $(IO_BUILD); fi)
	$(shell if [ ! -e $(MEMORY_BUILD) ];then mkdir -p $(MEMORY_BUILD); fi)
	$(shell if [ ! -e $(DISK_BUILD) ];then mkdir -p $(DISK_BUILD); fi)
	$(shell if [ ! -e $(FS_BUILD) ];then mkdir -p $(FS_BUILD); fi)
	$(shell if [ ! -e $(COMMON_BUILD) ];then mkdir -p $(COMMON_BUILD); fi)

$(KERNEL): $(KERNEL_FILES)
	$(LD) $(LDFLAGS) $^ -o $(BUILD)/kernelfull.o
	$(CC) -T $(LINKER) $(CFLAGS) $(BUILD)/kernelfull.o -o $@

$(BOOT): $(BOOT_SRC)/boot.asm
	nasm -f bin $(BOOT_SRC)/boot.asm -o $(BOOT)

$(BUILD)/kernel.asm.o: $(SRC)/kernel.asm
	nasm -f elf -g $^ -o $@

$(IDT_BUILD)/idt.asm.o: $(IDT_SRC)/idt.asm
	nasm -f elf -g $^ -o $@

$(IO_BUILD)/io.asm.o: $(IO_SRC)/io.asm
	nasm -f elf -g $^ -o $@

$(MEMORY_BUILD)/page.asm.o: $(MEMORY_SRC)/page.asm
	nasm -f elf -g $^ -o $@

$(BUILD)/%.o: %.c Makefile | $(BUILD)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	#TODO: recursive remove the files
	rm -rf $(BIN)/*.bin
	rm -rf $(BUILD)/*.o
	rm -rf $(BUILD)/idt/*.o
	rm -rf $(BUILD)/memory/*.o
	rm -rf $(BUILD)/io/*.o
	rm -rf $(BUILD)/disk/*.o
	rm -rf $(BUILD)/common/*.o
	rm -rf $(BUILD)/fs/*.o

#############################################
# OTHER TOOLS
#############################################
qemu:
	qemu-system-x86_64 -hda $(OS)

asm:
	ndisasm $(BIN)/kernel.bin

bless:
	bless $(BIN)/os.bin