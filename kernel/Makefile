CC ?= mos-mega65-clang
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra
TARGET := kernel.prg
SOURCES := startup.s kernel.c

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o *.elf *.map
