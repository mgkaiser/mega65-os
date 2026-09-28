COMPONENTS := kernel console loader
BUILD_DIR := $(CURDIR)/build

.PHONY: all clean $(COMPONENTS)

all: $(COMPONENTS)

loader: kernel console
	$(MAKE) -C loader BUILD_DIR=$(BUILD_DIR)

console:
	$(MAKE) -C console BUILD_DIR=$(BUILD_DIR)

kernel:
	$(MAKE) -C kernel BUILD_DIR=$(BUILD_DIR)

clean:
	@for d in $(COMPONENTS); do $(MAKE) -C $$d BUILD_DIR=$(BUILD_DIR) clean; done
	rm -rf $(BUILD_DIR)
