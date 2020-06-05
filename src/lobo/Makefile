# Directories
DIR_BUILD ?= ../build
DIR_SOURCE := .
DIR_SYSROOT := $(DIR_BUILD)/sysroot
# Flags
LINK_FLAGS := 
LINK_POST_FLAGS := -lgcc -lk
ASM_FLAGS := -felf64

WARNING_FLAGS := -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Wshadow -Wno-unused-parameter
CPU_FEATURE_FLAGS := -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-omit-frame-pointer 

DEBUG_FLAGS := -D DEBUG -Og -g
OPTIMIZATION_FLAGS:= $(DEBUG_FLAGS)
FEATURE_FLAGS := -fstack-protector -D __is_kernel
SHARED_FLAGS := -m64 -ffreestanding -mcmodel=kernel $(OPTIMIZATION_FLAGS) -MMD -z max-page-size=0x1000 \
 --sysroot=$(DIR_SYSROOT) -I$(DIR_SYSROOT)/usr/include/ -I$(DIR_SOURCE)/include/ \
 $(CPU_FEATURE_FLAGS) $(FEATURE_FLAGS) $(WARNING_FLAGS) -nostdlib

C_FLAGS := $(SHARED_FLAGS)  
CPP_FLAGS := $(SHARED_FLAGS) -std=c++17 -fno-exceptions -fno-rtti -Woverloaded-virtual

# Compilers
ASM := nasm
CC  := x86_64-elf-gcc
CPP := x86_64-elf-g++

# Files
LOBO_OBJDIR := $(DIR_BUILD)/lobo
LOBO_SOURCES := $(shell find $(DIR_SOURCE) -type f \( -iname '*.cpp' -or -iname '*.c' -or -iname '*.s' \) -not -iname "crt*"  )
LOBO_OBJECTS := $(LOBO_SOURCES:%=$(LOBO_OBJDIR)/%.o)
CRTBEGIN_OBJ:=$(shell $(CPP) $(CPP_FLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CPP) $(CPP_FLAGS) -print-file-name=crtend.o)

CRTI_SRC = $(DIR_SOURCE)/crt/crti.s
CRTN_SRC = $(DIR_SOURCE)/crt/crti.s
CRTI_OBJ=$(LOBO_OBJDIR)/arch/x86_64/crti.s.o
CRTN_OBJ=$(LOBO_OBJDIR)/arch/x86_64/crtn.s.o

OBJ_DEPEND = $(LOBO_OBJECTS:%.o=%.d)

LOBO_LINK_LIST := $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(LOBO_OBJECTS) $(CRTEND_OBJ) $(CRTN_OBJ)

-include $(OBJ_DEPEND)

# Make Rules
all: lobo

lobo: $(DIR_BUILD)/lobo/lobo.elf

clean:
	$(RM) -r $(DIR_BUILD)

# Build Rules
$(DIR_BUILD)/lobo/lobo.elf: $(LOBO_LINK_LIST)
	@mkdir -p $(dir $@)
	@echo "      LNK | lobo.elf" 
	@$(CPP) -T $(DIR_SOURCE)/arch/x86_64/link.ld -o $(DIR_BUILD)/lobo/lobo.elf $(CPP_FLAGS) $(LINK_FLAGS) $(LOBO_LINK_LIST) $(LINK_POST_FLAGS)

$(LOBO_OBJDIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@echo "      ASM | $<" 
	@$(ASM) $(ASM_FLAGS) -o $@ $<

$(LOBO_OBJDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "        C | $<" 
	@$(CC) $(C_FLAGS) -c $< -o $@ 

$(LOBO_OBJDIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "      CPP | $<" 
	@$(CPP) $(CPP_FLAGS) -c $< -o $@ 