ifeq ($(origin CC),default)
CC := mos-mega65-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -I../include
LDFLAGS ?=

# The loader is the bootable PRG, but it belongs beside the kernel and module
# images rather than in the loader source directory.
BUILD_DIR ?= ../build
DEBUG_DIR := $(BUILD_DIR)/debug
LIST_DIR := $(BUILD_DIR)/lst
OBJDUMP ?= llvm-objdump
TARGET := $(BUILD_DIR)/loader.prg
MAP := $(BUILD_DIR)/loader.map
LISTING := $(LIST_DIR)/loader.lst

SOURCES := loader.c hyppo.s
C_SOURCES := loader.c

.PHONY: all clean listings

all: $(TARGET) listings

$(BUILD_DIR):
	mkdir -p $@

$(DEBUG_DIR): | $(BUILD_DIR)
	mkdir -p $@

$(LIST_DIR): | $(BUILD_DIR)
	mkdir -p $@

$(TARGET): $(SOURCES) | $(BUILD_DIR) $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $^
	@# LLVM-MOS emits a companion ELF beside flat/PRG outputs when the linker
	@# script uses its custom OUTPUT_FORMAT. Keep the runnable image in build/
	@# and move the exact-link debug companion under build/debug/.
	@test -f $(BUILD_DIR)/loader.prg.elf || (echo "ERROR: linker did not produce $(BUILD_DIR)/loader.prg.elf"; false)
	mv $(BUILD_DIR)/loader.prg.elf $(DEBUG_DIR)/loader.elf

# Produce a source-interleaved disassembly of the exact linked ELF.  -S asks
# llvm-objdump to show source lines beside the final machine instructions;
# --line-numbers preserves file/line locations.  This is intentionally based
# on the linked ELF, not a second compiler-only assembly pass, so addresses and
# code match the runnable image.
listings: $(TARGET) | $(LIST_DIR)
	$(OBJDUMP) -d -S --line-numbers $(DEBUG_DIR)/loader.elf > $(LISTING)

clean:
	rm -f $(TARGET) $(MAP) $(LISTING) $(DEBUG_DIR)/loader.elf $(BUILD_DIR)/loader.prg.elf
