ifeq ($(origin CC),default)
CC := mos-common-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02 -I../include
LDFLAGS ?= -nostdlib
TARGET := console.bin
MAP := console.map
LISTING := console.lst
SOURCES := header.s console.c
C_SOURCES := console.c
.PHONY: all clean listings
all: $(TARGET) listings
$(TARGET): $(SOURCES) link.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $(SOURCES)
	@test $$(wc -c < $@) -le 8192 || (echo "console exceeds 8 KiB upper working-set page"; rm -f $@; false)
listings: $(C_SOURCES)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done
clean:
	rm -f $(TARGET) *.o *.obj *.elf *.map *.lst *.s.tmp *.ll
