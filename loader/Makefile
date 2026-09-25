CC ?= mos-mega65-clang
CFLAGS ?= -Os -ffreestanding -fno-builtin -Wall -Wextra
TARGET := loader.prg
SOURCES := loader.c hyppo.s
.PHONY: all clean
all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f $(TARGET) *.o *.elf *.map
