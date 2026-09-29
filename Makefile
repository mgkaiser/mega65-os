COMPONENTS := kernel console loader
BUILD_DIR := $(CURDIR)/build

# xemu-lgb MEGA65 SD-card image on the Windows host.  WSL exposes C: below
# /mnt/c, so no Windows-side helper is required.
SD_IMAGE ?= /mnt/c/Users/mgkai/AppData/Roaming/xemu-lgb/mega65/mega65.img
# The FAT32 partition starts at sector 2048 in xemu's 4 GiB disk image.
# mtools accepts IMAGE@@BYTE_OFFSET, so 2048 * 512 = 1,048,576 bytes.
SD_PARTITION_START_SECTOR ?= 2048
SD_SECTOR_SIZE ?= 512
SD_OFFSET := $(shell expr $(SD_PARTITION_START_SECTOR) \* $(SD_SECTOR_SIZE))
SD_MTOOLS_IMAGE := $(SD_IMAGE)@@$(SD_OFFSET)
# Keep SD-visible names deliberately boring: uppercase DOS 8.3-compatible
# names avoid depending on VFAT long-name/case handling in MEGA65 firmware.
SD_INSTALL_DIR ?= ::/MEGA65OS

# mtools operates directly on the FAT32 partition inside the disk image.  This is
# preferable to mounting the image: install stays unprivileged and does not
# leave a loop/mount behind.  Set MTOOLS_SKIP_CHECK=1 for MEGA65/xemu images
# whose geometry metadata mtools considers unusual. The @@ byte offset skips the MBR.
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
# The install directory and installed filenames are explicitly uppercase and
# DOS 8.3-compatible.  Do not rely on mtools to translate the lowercase Unix
# build artifact names into names the MEGA65 firmware will see consistently.
#
# Repeated "make install" runs overwrite the three runtime files.
install: all
	@test -f "$(SD_IMAGE)" || { \
		echo "ERROR: MEGA65 SD image not found: $(SD_IMAGE)"; \
		exit 1; \
	}
	@command -v mcopy >/dev/null 2>&1 || { \
		echo "ERROR: mtools is required. Install it with: sudo apt install mtools"; \
		exit 1; \
	}
	@# mmd does not reliably accept a trailing slash in the destination name.
	@# Test first so a genuine mmd failure is not hidden by "|| true".
	@if ! mdir -i "$(SD_MTOOLS_IMAGE)" "$(SD_INSTALL_DIR)" >/dev/null 2>&1; then \
		echo "Creating $(SD_INSTALL_DIR) on MEGA65 SD image"; \
		mmd -i "$(SD_MTOOLS_IMAGE)" "$(SD_INSTALL_DIR)"; \
	fi
	mcopy -i "$(SD_MTOOLS_IMAGE)" -o \
		"$(BUILD_DIR)/kernel.bin" \
		"$(SD_INSTALL_DIR)/KERNEL.BIN"
	mcopy -i "$(SD_MTOOLS_IMAGE)" -o \
		"$(BUILD_DIR)/console.bin" \
		"$(SD_INSTALL_DIR)/CONSOLE.BIN"
	mcopy -i "$(SD_MTOOLS_IMAGE)" -o \
		"$(BUILD_DIR)/loader.prg" \
		"$(SD_INSTALL_DIR)/LOADER.PRG"

clean:
	@for d in $(COMPONENTS); do $(MAKE) -C $$d BUILD_DIR=$(BUILD_DIR) clean; done
	rm -rf $(BUILD_DIR)
