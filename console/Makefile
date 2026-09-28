ifeq ($(origin CC),default)
CC := mos-common-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02 -I../include
LDFLAGS ?= -nostdlib

# All generated component products live together outside the source tree.
BUILD_DIR ?= ../build
TARGET := $(BUILD_DIR)/console.bin
MAP := $(BUILD_DIR)/console.map
LISTING := $(BUILD_DIR)/console.lst

SOURCES := header.s console.c
C_SOURCES := console.c

.PHONY: all clean listings

all: $(TARGET) listings

$(BUILD_DIR):
	mkdir -p $@

$(TARGET): $(SOURCES) link.ld | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $(SOURCES)
	@test $$(wc -c < $@) -le 8192 || (echo "console exceeds 8 KiB upper working-set page"; rm -f $@; false)

listings: $(C_SOURCES) | $(BUILD_DIR)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done

clean:
	rm -f $(TARGET) $(MAP) $(LISTING)
