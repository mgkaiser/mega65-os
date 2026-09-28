ifeq ($(origin CC),default)
CC := mos-common-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02 -I../include
LDFLAGS ?= -nostdlib

# Keep generated artifacts out of the source tree.  The top-level Makefile
# supplies an absolute BUILD_DIR so every component deposits its products in
# one place.  The fallback makes "make -C kernel" useful by itself.
BUILD_DIR ?= ../build
TARGET := $(BUILD_DIR)/kernel.bin
MAP := $(BUILD_DIR)/kernel.map
LISTING := $(BUILD_DIR)/kernel.lst

SOURCES := startup.s kernel.c brk.c mapper.c mapper.s vm_bootstrap.c kmodule.c runtime.c runtime.s
C_SOURCES := kernel.c brk.c mapper.c vm_bootstrap.c kmodule.c runtime.c
RESIDENT_LIMIT := 8192

.PHONY: all clean listings

all: $(TARGET) listings

$(BUILD_DIR):
	mkdir -p $@

$(TARGET): $(SOURCES) link.ld | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $(SOURCES)
	@size=$$(wc -c < $@); \
	if [ $$size -gt $(RESIDENT_LIMIT) ]; then \
		echo "ERROR: resident kernel is $$size bytes; limit is $(RESIDENT_LIMIT)"; \
		rm -f $@; exit 1; \
	fi
	@echo "resident kernel: $$(( $$(wc -c < $@) )) / $(RESIDENT_LIMIT) bytes"

listings: $(C_SOURCES) | $(BUILD_DIR)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done

clean:
	rm -f $(TARGET) $(MAP) $(LISTING)
