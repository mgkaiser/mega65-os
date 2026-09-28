ifeq ($(origin CC),default)
CC := mos-common-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02 -I../include
LDFLAGS ?= -nostdlib

# All generated component products live together outside the source tree.
BUILD_DIR ?= ../build
DEBUG_DIR := $(BUILD_DIR)/debug
TARGET := $(BUILD_DIR)/console.bin
MAP := $(BUILD_DIR)/console.map
LISTING := $(BUILD_DIR)/console.lst

SOURCES := header.s console.c
C_SOURCES := console.c

.PHONY: all clean listings

all: $(TARGET) listings

$(BUILD_DIR):
	mkdir -p $@

$(DEBUG_DIR): | $(BUILD_DIR)
	mkdir -p $@

$(TARGET): $(SOURCES) link.ld | $(BUILD_DIR) $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $(SOURCES)
	@# LLVM-MOS emits a companion ELF beside flat/PRG outputs when the linker
	@# script uses its custom OUTPUT_FORMAT. Keep the runnable image in build/
	@# and move the exact-link debug companion under build/debug/.
	@test -f $(BUILD_DIR)/console.bin.elf || (echo "ERROR: linker did not produce $(BUILD_DIR)/console.bin.elf"; false)
	mv $(BUILD_DIR)/console.bin.elf $(DEBUG_DIR)/console.elf
	@test $$(wc -c < $@) -le 8192 || (echo "console exceeds 8 KiB upper working-set page"; rm -f $@; false)

listings: $(C_SOURCES) | $(BUILD_DIR)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done

clean:
	rm -f $(TARGET) $(MAP) $(LISTING) $(DEBUG_DIR)/console.elf $(BUILD_DIR)/console.bin.elf
