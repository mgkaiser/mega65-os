COMPONENTS := kernel console loader
BUILD_DIR := $(CURDIR)/build

# xemu-lgb MEGA65 SD-card image on the Windows host.  WSL exposes C: below
# /mnt/c, so no Windows-side helper is required.
SD_IMAGE ?= /mnt/c/Users/mgkai/AppData/Roaming/xemu-lgb/mega65/mega65.img
SD_INSTALL_DIR ?= ::/mega65-os
MTOOLS ?=

# mtools operates directly on the FAT filesystem inside the image.  This is
# preferable to mounting the image: install stays unprivileged and does not
# leave a loop/mount behind.  Set MTOOLS_SKIP_CHECK=1 for MEGA65/xemu images
# whose geometry metadata mtools considers unusual.
export MTOOLS_SKIP_CHECK := 1

.PHONY: all clean install $(COMPONENTS)

all: $(COMPONENTS)

loader: kernel console
	$(MAKE) -C loader BUILD_DIR=$(BUILD_DIR)

console:
	$(MAKE) -C console BUILD_DIR=$(BUILD_DIR)

kernel:
	$(MAKE) -C kernel BUILD_DIR=$(BUILD_DIR)

# Install only the files the MEGA65 needs to boot/run.  Debug ELF, map and
# source-interleaved listing artifacts remain on the development filesystem.
#
# mmd -s creates parent directories as needed and returns an error if the final
# directory already exists, so tolerate that one operation.  mcopy -o then
# replaces previous installed images, making repeated "make install" useful.
install: all
	@test -f "$(SD_IMAGE)" || { \
		echo "ERROR: MEGA65 SD image not found: $(SD_IMAGE)"; \
		exit 1; \
	}
	@command -v mcopy >/dev/null 2>&1 || { \
		echo "ERROR: mtools is required. Install it with: sudo apt install mtools"; \
		exit 1; \
	}
	@mmd -i "$(SD_IMAGE)" -s "$(SD_INSTALL_DIR)" 2>/dev/null || true
	mcopy -i "$(SD_IMAGE)" -o \
		"$(BUILD_DIR)/kernel.bin" \
		"$(BUILD_DIR)/console.bin" \
		"$(BUILD_DIR)/loader.prg" \
		"$(SD_INSTALL_DIR)/"

clean:
	@for d in $(COMPONENTS); do $(MAKE) -C $$d BUILD_DIR=$(BUILD_DIR) clean; done
	rm -rf $(BUILD_DIR)
