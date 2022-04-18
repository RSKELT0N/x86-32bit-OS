#########################
# variables
#########################
GPPFLAGS       := -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASFLAGS        := --32
LDFLAGS        := -melf_i386
TARGET         := myos.bin
IMAGE          := myos.iso
SRC_DIR        := ./src
BIN_DIR        := ./bin
ISO_DIR        := ./iso
OS_DIR         := ./os
CPP            = $(wildcard $(SRC_DIR)/*.cpp)
AS             = $(wildcard $(SRC_DIR)/*.as)
CPP_OBJ_RULE   = $(patsubst %.cpp, %.o, $(CPP)) #pattern, replacement, text
AS_OBJ_RULE    = $(patsubst %.as, %.o, $(AS)) #pattern, replacement, text
#########################
# make
#########################
all: install

install: $(IMAGE)

%.o: %.cpp
	g++ $(GPPFLAGS) -o $@ -c $<
%.o: %.as
	as $(ASFLAGS) -o $@ $<

$(TARGET): $(CPP_OBJ_RULE) $(AS_OBJ_RULE)
	ld $(LDFLAGS) -T $(SRC_DIR)/linker.ld -o $@ $(SRC_DIR)/*.o

$(IMAGE): $(TARGET)
	mkdir $(BIN_DIR)
	mv $(SRC_DIR)/*.o $(BIN_DIR)/
	mkdir $(ISO_DIR)
	mkdir $(ISO_DIR)/boot
	mkdir $(ISO_DIR)/boot/grub
	mv ./$< $(ISO_DIR)/boot/
	echo 'set timeout=0'                     >  $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0'                     >> $(ISO_DIR)/boot/grub/grub.cfg
	echo ''                                  >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/myos.bin'        >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  boot'                            >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}'                                 >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso

	mkdir $(OS_DIR)
	mv $(ISO_DIR)/boot/$(TARGET) $(OS_DIR)
	mv $(IMAGE) $(OS_DIR)
	rm -rf $(ISO_DIR)

run:
	# VirtualBox --startvm "My Operating System" &
	(killall qemu-system-i386 && sleep 1) || true
	qemu-system-i386 -kernel $(OS_DIR)/$(TARGET) &
#########################
# clean
#########################
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OS_DIR)
#########################
# rebuild
#########################
rb: clean all run
#########################
