CC ?= mos-mega65-clang
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra
TARGET := kernel.prg
SOURCES := startup.s kernel.c brk.c

# The resident kernel nucleus is deliberately constrained to one 8 KiB extent.
# KERNEL_BASE is also the loader's handoff address.
KERNEL_BASE ?= 0x4000
RESIDENT_LIMIT ?= 8192

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -Wl,--defsym=__kernel_base=$(KERNEL_BASE) -o $@ $^
	@size=$$(wc -c < $@); \
	if [ $$size -gt $$(( $(RESIDENT_LIMIT) + 2 )) ]; then \
		echo "ERROR: resident kernel image is $$((size - 2)) bytes; limit is $(RESIDENT_LIMIT)"; \
		rm -f $@; exit 1; \
	fi

clean:
	rm -f $(TARGET) *.o *.elf *.map
