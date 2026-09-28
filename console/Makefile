ifeq ($(origin CC),default)
CC := mos-mega65-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02 -I../include
LDFLAGS ?= -nostdlib -Wl,-T,link.ld
TARGET := console.bin
SOURCES := header.s console.c
.PHONY: all clean
all: $(TARGET)
$(TARGET): $(SOURCES) link.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SOURCES)
	@test $$(wc -c < $@) -le 8192 || (echo "console exceeds 8 KiB upper working-set page"; rm -f $@; false)
clean:
	rm -f $(TARGET) *.o *.elf *.map
