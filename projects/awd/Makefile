# Directories
DIR_BUILD ?= ../../build/
DIR_SOURCE := src

# Flags
LINK_FLAGS := 
LINK_POST_FLAGS := -lgcc
ASM_FLAGS := -felf32 -w-zext-reloc

WARNING_FLAGS := -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Wshadow
C_FLAGS := -ffreestanding -Og -g -nostdlib -Isrc/includes/  $(WARNING_FLAGS)

# Compilers
ASM := nasm
CC := i686-elf-gcc

# Files
AWD_SOURCES := $(shell find $(DIR_SOURCE) -name *.c -or -name *.s)
AWD_OBJECTS := $(AWD_SOURCES:%=$(DIR_BUILD)/%.o)

# Make Rules
all: awd

awd: $(DIR_BUILD)/awd/awd.elf

clean:
	$(RM) -r $(DIR_BUILD)

qemu: awd
	qemu-system-x86_64 -kernel .$(DIR_BUILD)/awd/awd.elf

# Build Rules
$(DIR_BUILD)/awd/awd.elf: $(AWD_OBJECTS)
	@mkdir -p $(dir $@)
	@echo "      LNK awd.elf" 
	@$(CC) -T $(DIR_SOURCE)/link.ld -o $(DIR_BUILD)/awd/awd.elf $(C_FLAGS) $(LINK_FLAGS) $(AWD_OBJECTS) $(LINK_POST_FLAGS)

$(DIR_BUILD)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@echo "      ASM $<" 
	@$(ASM) $(ASM_FLAGS) -o $@ $<

$(DIR_BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "        C $<" 
	@$(CC) $(C_FLAGS) -c $< -o $@ 