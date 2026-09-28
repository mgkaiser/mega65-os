ifeq ($(origin CC),default)
CC := mos-mega65-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -mcpu=mos45gs02 -I../include
LDFLAGS ?= -nostdlib -Wl,-T,link.ld
TARGET := kernel.bin
MAP := kernel.map
LISTING := kernel.lst
SOURCES := startup.s kernel.c brk.c mapper.c mapper.s vm_bootstrap.c kmodule.c
C_SOURCES := kernel.c brk.c mapper.c vm_bootstrap.c kmodule.c
RESIDENT_LIMIT := 8192
.PHONY: all clean listings
all: $(TARGET) listings
$(TARGET): $(SOURCES) link.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $(SOURCES)
	@size=$$(wc -c < $@); \
	if [ $$size -gt $(RESIDENT_LIMIT) ]; then \
		echo "ERROR: resident kernel is $$size bytes; limit is $(RESIDENT_LIMIT)"; \
		rm -f $@; exit 1; \
	fi
	@echo "resident kernel: $$(( $$(wc -c < $@) )) / $(RESIDENT_LIMIT) bytes"
listings: $(C_SOURCES)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done
clean:
	rm -f $(TARGET) *.o *.obj *.elf *.map *.lst *.s.tmp *.ll
