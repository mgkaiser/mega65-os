ifeq ($(origin CC),default)
CC := mos-mega65-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -I../include
LDFLAGS ?=

# The loader is the bootable PRG, but it belongs beside the kernel and module
# images rather than in the loader source directory.
BUILD_DIR ?= ../build
TARGET := $(BUILD_DIR)/loader.prg
MAP := $(BUILD_DIR)/loader.map
LISTING := $(BUILD_DIR)/loader.lst

SOURCES := loader.c hyppo.s
C_SOURCES := loader.c

.PHONY: all clean listings

all: $(TARGET) listings

$(BUILD_DIR):
	mkdir -p $@

$(TARGET): $(SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $^

listings: $(C_SOURCES) | $(BUILD_DIR)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done

clean:
	rm -f $(TARGET) $(MAP) $(LISTING)
