COMPONENTS := kernel loader
.PHONY: all clean $(COMPONENTS)
all: $(COMPONENTS)
loader: kernel
	$(MAKE) -C loader
kernel:
	$(MAKE) -C kernel
clean:
	@for d in $(COMPONENTS); do $(MAKE) -C $$d clean; done
