ifeq ($(origin CC),default)
CC := mos-mega65-clang
endif
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra -I../include
LDFLAGS ?=
TARGET := loader.prg
MAP := loader.map
LISTING := loader.lst
SOURCES := loader.c hyppo.s
C_SOURCES := loader.c
.PHONY: all clean listings
all: $(TARGET) listings
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map,$(MAP) -o $@ $^
listings: $(C_SOURCES)
	@rm -f $(LISTING)
	@for src in $(C_SOURCES); do \
		echo "===== $$src =====" >> $(LISTING); \
		$(CC) $(CFLAGS) -S -fverbose-asm -o - $$src >> $(LISTING); \
		echo >> $(LISTING); \
	done
clean:
	rm -f $(TARGET) *.o *.obj *.elf *.map *.lst *.s.tmp *.ll
