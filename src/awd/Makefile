# Directories
DIR_BUILD ?= ../../build/
DIR_SOURCE := .

# Flags
LINK_FLAGS 		:= 
LINK_POST_FLAGS ?= -lgcc
ASM_FLAGS 		:= -felf32 -w-zext-reloc

WARNING_FLAGS := -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wredundant-decls -Wshadow -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast
FEATURE_FLAGS := -mno-mmx -mno-sse -mno-sse2 -fno-omit-frame-pointer 
C_FLAGS := -ffreestanding -Og -g -nostdlib -I$(DIR_SOURCE)/includes/ $(FEATURE_FLAGS) $(WARNING_FLAGS)

# Compilers
ASM 	 := nasm
CC  	 := i686-elf-gcc

# Files
AWD_OBJDIR := $(DIR_BUILD)/awd/
AWD_SOURCES := $(shell find $(DIR_SOURCE) -name '*.c' -or -name '*.s')
AWD_OBJECTS := $(AWD_SOURCES:%=$(AWD_OBJDIR)/%.o)

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
	@echo "      LNK | awd.elf" 
	@$(CC) -T $(DIR_SOURCE)/link.ld -o $(DIR_BUILD)/awd/awd.elf $(C_FLAGS) $(LINK_FLAGS) $(AWD_OBJECTS) $(LINK_POST_FLAGS)

$(AWD_OBJDIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@echo "      ASM | $<" 
	@$(ASM) $(ASM_FLAGS) -o $@ $<

$(AWD_OBJDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "        C | $<" 
	@$(CC) $(C_FLAGS) -c $< -o $@ 