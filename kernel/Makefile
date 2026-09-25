CC ?= mos-mega65-clang
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02
LDFLAGS ?= -nostdlib -Wl,-T,link.ld
TARGET := kernel.bin
SOURCES := startup.s kernel.c brk.c
RESIDENT_LIMIT := 8192

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(SOURCES) link.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SOURCES)
	@size=$$(wc -c < $@); \
	if [ $$size -gt $(RESIDENT_LIMIT) ]; then \
		echo "ERROR: resident kernel is $$size bytes; limit is $(RESIDENT_LIMIT)"; \
		rm -f $@; exit 1; \
	fi
	@echo "resident kernel: $$(( $$(wc -c < $@) )) / $(RESIDENT_LIMIT) bytes"

clean:
	rm -f $(TARGET) *.o *.elf *.map
