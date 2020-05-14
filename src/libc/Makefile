# Directories
DIR_BUILD ?= ../build/
DIR_SOURCE := .

# Flags
LINK_FLAGS := 
LINK_POST_FLAGS := -lgcc
ASM_FLAGS := -felf64

WARNING_FLAGS := -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wredundant-decls -Wshadow -Wno-unused-parameter
CPU_FEATURE_FLAGS := -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-omit-frame-pointer 
FEATURE_FLAGS := -fstack-protector -D DEBUG

LIBC_FLAGS := -m64 -Og -g -ffreestanding -MMD \
-z max-page-size=0x1000 -I$(DIR_SOURCE)/includes/ $(CPU_FEATURE_FLAGS) $(FEATURE_FLAGS) $(WARNING_FLAGS) -nostdlib

LIBK_FLAGS := -m64 -ffreestanding -mcmodel=kernel -Og -g -MMD \
-z max-page-size=0x1000 -I$(DIR_SOURCE)/includes/ $(CPU_FEATURE_FLAGS) $(FEATURE_FLAGS) $(WARNING_FLAGS) -nostdlib


C_FLAGS := $(SHARED_FLAGS)  
CPP_FLAGS := $(SHARED_FLAGS) -fno-exceptions -fno-rtti -Woverloaded-virtual

# Compilers
ASM := nasm
CC  := x86_64-elf-gcc
CPP := x86_64-elf-g++
AR  := x86_64-elf-ar

# Files
OBJDIR := $(DIR_BUILD)/libc
SOURCES := $(shell find $(DIR_SOURCE) -type f \( -iname '*.cpp' -or -iname '*.c' -or -iname '*.s' \) )
OBJECTS := $(SOURCES:%=$(OBJDIR)/%.o)
LIBC_LINK_LIST := $(OBJECTS)
LIBK_LINK_LIST := $(OBJECTS:.o=.libk.o)
 
LIBC_DEPEND := $(LIBC_LINK_LIST:%.o=%.d)
LIBK_DEPEND := $(LIBK_LINK_LIST:%.o=%.d)

-include $(LIBC_DEPEND)
-include $(LIBK_DEPEND)

BINARIES := libk

# Make Rules
all: libc libk

libc: $(DIR_BUILD)/libc/libc.a
libk: $(DIR_BUILD)/libc/libk.a

clean:
	$(RM) -r $(DIR_BUILD)

install: install-headers install-libs
 
install-headers:
	@mkdir -p $(DIR_BUILD)/sysroot/usr/include
	@cp -R --preserve=timestamps $(DIR_SOURCE)/includes/. $(DIR_BUILD)/sysroot/usr/include/
 
install-libs: libc libk
	@mkdir -p $(DIR_BUILD)/sysroot/usr/lib/
	@cp $(DIR_BUILD)/libc/libc.a $(DIR_BUILD)/sysroot/usr/lib/libc.a
	@cp $(DIR_BUILD)/libc/libk.a $(DIR_BUILD)/sysroot/usr/lib/libk.a

# Build Rules

$(DIR_BUILD)/libc/libc.a: $(LIBC_LINK_LIST)
	@mkdir -p $(dir $@)
	@echo "      LNK | libc.a" 
	@$(AR) rcs $@ $(LIBC_LINK_LIST)

$(DIR_BUILD)/libc/libk.a: $(LIBK_LINK_LIST)
	@mkdir -p $(dir $@)
	@echo "      LNK | libk.a" 
	@$(AR) rcs $@ $(LIBK_LINK_LIST)

$(OBJDIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@echo "      ASM | $<" 
	@$(ASM) $(ASM_FLAGS) -o $@ $<

$(OBJDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "        C | $<" 
	@$(CC) $(LIBC_FLAGS) $(C_FLAGS) -c $< -o $@ 

$(OBJDIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "      CPP | $<" 
	@$(CPP) $(LIBC_FLAGS) $(CPP_FLAGS) -c $< -o $@ 


$(OBJDIR)/%.s.libk.o: %.s
	@mkdir -p $(dir $@)
	@echo "  [K] ASM | $<" 
	@$(ASM) $(ASM_FLAGS) -o $@ $<

$(OBJDIR)/%.c.libk.o: %.c
	@mkdir -p $(dir $@)
	@echo "  [K]   C | $<" 
	@$(CC) $(LIBK_FLAGS) $(C_FLAGS) -D__is_libk -c $< -o $@ 

$(OBJDIR)/%.cpp.libk.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "  [K] CPP | $<" 
	@$(CPP) $(LIBK_FLAGS) $(CPP_FLAGS) -D__is_libk  -c $< -o $@ 