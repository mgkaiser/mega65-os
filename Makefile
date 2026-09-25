COMPONENTS := kernel console loader
.PHONY: all clean $(COMPONENTS)
all: $(COMPONENTS)
loader: kernel console
	$(MAKE) -C loader
console:
	$(MAKE) -C console
kernel:
	$(MAKE) -C kernel
clean:
	@for d in $(COMPONENTS); do $(MAKE) -C $$d clean; done
