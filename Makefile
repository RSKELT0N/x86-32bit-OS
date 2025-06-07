####################################################################
# variables
# Flags set to compile OS with no dynamic libraries
# and to be self hosted with generated 32 bit object binary files.
#
## Flags ##
# -m32                        - GCC compiles files in 32-bit mode
# -fno-stack-protector        - Disables stack protection
# -fno-use-cxa-atexit         - Generates codes for .fini section
# -nostdlib                   - Disables linkage with std library
# -fno-builtin                - GCC will not try to replace library functions with builtin compiled code
# -fno-rtti                   - Disables run time type information
# -fno-exceptions             - Disables exception support
# -fno-leading-underscore     - Disables prefixed '_' for function names
# -melf_i386                  - Linker format output towards i386 architecture.
####################################################################

OS_DIR      := ./os
ISO_DIR     := ./iso
CROSS_C_DIR := ./cross-compiler
BUILD_DIR   := ./build
KERNEL_DIR  := ./kernel
STDLIBC_DIR := ./stdlibc
BIN_DIR     := ./bin
TARGET      := os.elf
IMAGE       := os.iso
LINKER      := $(OS_DIR)/linker.ld
CXX         := i686-elf-g++
LD          := i686-elf-ld
LD_FLAGS    := -melf_i386
NASM_FLAGS  = -f elf32
ELTORITO    = stage2_eltorito
MENU_CONFIG = menu.lst

CXXFLAGS := -m32               \
	-nostdlib                   \
	-nostdinc                   \
	-fno-leading-underscore     \
	-fno-stack-protector        \
	-fno-use-cxa-atexit         \
	-fno-exceptions             \
	-nodefaultlibs              \
	-nostartfiles               \
	-std=gnu++20                \
	-fno-builtin                \
	-fno-rtti                   \
	-Werror                     \
	-Wall                       \
	-I$(OS_DIR)                 \
	-c


CPP          = $(shell find $(OS_DIR)/$(KERNEL_DIR) $(OS_DIR)/$(STDLIBC_DIR) -name "*.cpp")  # Retrieves all .cpp files from $(SRC_DIR)
ASM          = $(shell find $(OS_DIR)/$(KERNEL_DIR) $(OS_DIR)/$(STDLIBC_DIR) -name "*.asm")  # Retrieves all .asm files from $(SRC_DIR)
CPP_OBJ_RULE = $(patsubst %.cpp, %.o, $(CPP))                                                # Replaces all .cpp, corresponding with .o
AS_OBJ_RULE  = $(patsubst %.asm, %.o, $(ASM))                                                # Replaces all .asm, corresponding with .o

####################################
# make
# Compiles all source files,
# links and generated image file.
####################################
all: setup install

build-cross-compiler:
	./scripts/build-cross-compiler.sh

setup:
	mkdir $(BIN_DIR)

# Updates binary file to /boot and places image file to os directory.
install: $(IMAGE)
	mkdir $(BUILD_DIR)
	cp $(ISO_DIR)/boot/$(TARGET) $(BUILD_DIR)
	mv $(IMAGE) $(BUILD_DIR)

# Compiles .cpp files with set flags.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	mv $@ $(BIN_DIR)

# Compiles .asm files with set flags.
%.o: %.asm
	nasm $(NASM_FLAGS) -o $@ $<
	mv $@ $(BIN_DIR)

# Links all .o files together into executable binary.
$(TARGET): $(CPP_OBJ_RULE) $(AS_OBJ_RULE)
	$(LD) $(LD_FLAGS) -T $(LINKER) -o $@ $(BIN_DIR)/*.o

# Generates iso grub configuration folder for creating image. To be booted by qemu.
$(IMAGE): $(TARGET)
	mkdir -p $(ISO_DIR)/boot/grub
	mv ./$< $(ISO_DIR)/boot/
	cp templates/$(ELTORITO) $(ISO_DIR)/boot/grub/
	cp templates/$(MENU_CONFIG) $(ISO_DIR)/boot/grub/
	mkisofs -R                      \
	-b boot/grub/$(ELTORITO)        \
	-no-emul-boot                   \
	-boot-load-size 4               \
	-input-charset utf8             \
	-boot-info-table                \
	-o $(IMAGE)                     \
	$(ISO_DIR)

# Launches qemu and loads image as boot medium to be loaded by grub to start entry within kernel.
run: run-qemu

run-qemu:
	(killall qem-system-i386 && sleep 1) || true
	qemu-system-i386 -no-reboot -cdrom $(BUILD_DIR)/$(IMAGE) &

run-vm:
	(killall vboxmanage && sleep 1) || true
	vboxmanage startvm "OS"

	
#########################
# clean
#########################
clean:
	rm -rf $(BIN_DIR) || true
	rm -rf $(BUILD_DIR)  || true
	rm -rf $(BUILD_DIR)  || true
	rm -rf $(CROSS_C_DIR) || true
#########################
# rebuild
#########################
rb: clean all run-qemu
#########################
