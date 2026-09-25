# Boot and Takeover

## Current direction

Do not gradually coexist with the stock operating environment.

The stock environment merely launches a tiny transition loader. That loader uses the MEGA65 hypervisor facilities to cross into our own environment as early as practical.

The phase-1 implementation direction is to use stock Hyppo's transfer-area, setname, and loadfile calls to load a raw `kernel.bin` at **$E000**, then disable compatibility-ROM write protection, establish the native RAM/IO mapping, and transfer control directly to the kernel entry at $E000. Hyppo load failures halt the transition loader rather than jumping into an invalid image.

The resident kernel occupies the top logical 8 KiB page, **$E000-$FFFF**. This naturally includes the normal NMI, RESET, and IRQ/BRK vectors at $FFFA-$FFFF. The linker should place and enforce these vectors inside the resident image and reject a nucleus that exceeds the single-page budget.

Conceptually:

    RESET
      -> MEGA65 firmware/stock environment
      -> tiny transition loader
      -> Hyppo loads resident kernel at $E000
      -> inherited mappings/environment are abandoned
      -> bootstrap establishes our machine state
      -> Page 7 remains the resident nucleus
      -> normal OS loader/binder takes over
      -> init/session/console

Once the bootstrap receives control, no stock OS code should be required again.

## Transition loader

Keep extremely small. Responsibilities only:
- enter controlled machine state,
- mask/disable normal interrupt activity as required,
- invoke hypervisor loading facilities,
- transfer control to the resident kernel/bootstrap entry.

It should not contain a filesystem, general executable loader, or normal OS functionality.

## Initial logical map

The intended native 1.x view after takeover is:

    $0000-$1FFF   active-thread Page 0
    $2000-$BFFF   demand-paged process Pages 1-5
    $C000-$DFFF   pageable kernel extension Page 6
    $E000-$FFFF   resident kernel Page 7

Page 0 can conventionally contain Base Page at $0000, initial stack at $0100, and code/data from $0200. These are logical addresses backed by the active thread's mapped page rather than globally reserved physical RAM.

Page 6 is used to execute drivers and kernel extensions that do not fit in the nucleus. Their persistent data can remain out of map and be accessed through 45GS02 flat/far addressing.

## Bootstrap

Bootstrap responsibilities:
- establish our memory map,
- install/confirm our vectors,
- initialize enough DMA/storage machinery,
- provide primitive allocation,
- understand the boot container sufficiently to load required components,
- instantiate the kernel nucleus and normal loader/binder.

Once normal kernel facilities exist, bootstrap memory should be reclaimable except for machinery deliberately retained in the Page-7 nucleus.

## Boot image

Prefer the same native container format used elsewhere, with a minimal subset understood by bootstrap. Mark extents such as BOOT_REQUIRED rather than inventing a completely unrelated boot executable format.

Open item: verify exact hypervisor facilities and restrictions before freezing the boot sequence.

## Versioned Hypervisor Strategy

For 1.x, retain the stock Hyppo/MEGA65 Hypervisor and use it only as required to launch/take over the machine. Do not make the 1.x kernel dependent on a custom `HICKUP.M65`.

For 2.x, plan an optional MEGA65 OS supervisor implemented by replacing/extending `HICKUP.M65`. The official platform permits a modified 16 KiB Hypervisor program and provides protected Hypervisor memory plus hardware trap/SYSCALL entry. The 2.x supervisor is intended to remain a small protection/virtualization monitor, not to absorb normal kernel services.

Hardware page-fault support is a 2.x investigation. If validated on the production core with suitable semantics, demand paging may use genuine Hypervisor page-fault traps. This is not a 1.x dependency.
