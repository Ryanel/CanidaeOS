# Directories
DIR_BUILD ?= ../../build/
DIR_SOURCE := src

# Flags
LINK_FLAGS := 
LINK_POST_FLAGS := -lgcc
ASM_FLAGS := -felf64

WARNING_FLAGS := -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Wshadow -Wno-unused-parameter
CPU_FEATURE_FLAGS := -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-omit-frame-pointer 
FEATURE_FLAGS := -D DEBUG
SHARED_FLAGS := -m64 -ffreestanding -mcmodel=kernel -O2 -g  \
-z max-page-size=0x1000 -Isrc/includes/ $(CPU_FEATURE_FLAGS) $(FEATURE_FLAGS) $(WARNING_FLAGS) -nostdlib

C_FLAGS := $(SHARED_FLAGS) 
CPP_FLAGS := $(SHARED_FLAGS) -fno-exceptions -fno-rtti  -Woverloaded-virtual

# Compilers
ASM := nasm
CC  := x86_64-elf-gcc
CPP := x86_64-elf-g++

# Files
LOBO_SOURCES := $(shell find $(DIR_SOURCE) -type f \( -iname *.cpp -or -iname *.c -or -iname *.s \) -not -iname "crt*"  )
LOBO_OBJECTS := $(LOBO_SOURCES:%=$(DIR_BUILD)/%.o)
CRTBEGIN_OBJ:=$(shell $(CPP) $(CPP_FLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CPP) $(CPP_FLAGS) -print-file-name=crtend.o)

CRTI_SRC = $(DIR_SOURCE)/crt/crti.s
CRTN_SRC = $(DIR_SOURCE)/crt/crti.s
CRTI_OBJ=$(DIR_BUILD)/src/x86_64/crt/crti.s.o
CRTN_OBJ=$(DIR_BUILD)/src/x86_64/crt/crtn.s.o

LOBO_LINK_LIST := $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(LOBO_SOURCES:%=$(DIR_BUILD)/%.o) $(CRTEND_OBJ) $(CRTN_OBJ)

# Make Rules
all: lobo

lobo: $(DIR_BUILD)/lobo/lobo.elf

clean:
	$(RM) -r $(DIR_BUILD)

# Build Rules
$(DIR_BUILD)/lobo/lobo.elf: $(LOBO_LINK_LIST)
	@mkdir -p $(dir $@)
	@echo "      LNK | lobo.elf" 
	@$(CPP) -T $(DIR_SOURCE)/x86_64/link.ld -o $(DIR_BUILD)/lobo/lobo.elf $(CPP_FLAGS) $(LINK_FLAGS) $(LOBO_LINK_LIST) $(LINK_POST_FLAGS)

$(DIR_BUILD)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@echo "      ASM | $<" 
	@$(ASM) $(ASM_FLAGS) -o $@ $<

$(DIR_BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "        C | $<" 
	@$(CC) $(C_FLAGS) -c $< -o $@ 

$(DIR_BUILD)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "      CPP | $<" 
	@$(CPP) $(CPP_FLAGS) -c $< -o $@ 