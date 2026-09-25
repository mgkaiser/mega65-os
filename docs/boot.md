# Boot and Takeover

## Current direction

Do not gradually coexist with the stock operating environment.

The stock environment merely launches a tiny transition loader. That loader uses the MEGA65 hypervisor facilities to cross into our own environment as early as practical.

The phase-1 implementation uses stock Hyppo's transfer-area, setname, and loadfile calls to load a raw `kernel.bin` at **$4000**, then disables compatibility-ROM write protection, establishes the native RAM/IO mapping, and transfers control directly to the kernel entry at $4000. Hyppo load failures halt the transition loader rather than jumping into an invalid image.

Conceptually:

    RESET
      -> MEGA65 firmware/stock environment
      -> tiny transition loader
      -> hypervisor loads MEGA65 OS bootstrap
      -> inherited mappings/vectors/environment are abandoned
      -> bootstrap establishes our machine state
      -> bootstrap loads kernel nucleus
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

## Bootstrap

Bootstrap responsibilities:
- establish our memory map,
- install our vectors,
- initialize enough DMA/storage machinery,
- provide primitive allocation,
- understand the boot container sufficiently to load required components,
- instantiate the kernel nucleus and normal loader/binder.

Once normal kernel facilities exist, bootstrap memory should be reclaimable.

## Boot image

Prefer the same native container format used elsewhere, with a minimal subset understood by bootstrap. Mark extents such as BOOT_REQUIRED rather than inventing a completely unrelated boot executable format.

Open item: verify exact hypervisor facilities and restrictions before freezing the boot sequence.

## Versioned Hypervisor Strategy

For 1.x, retain the stock Hyppo/MEGA65 Hypervisor and use it only as required to launch/take over the machine. Do not make the 1.x kernel dependent on a custom `HICKUP.M65`.

For 2.x, plan an optional MEGA65 OS supervisor implemented by replacing/extending `HICKUP.M65`. The official platform permits a modified 16 KiB Hypervisor program and provides protected Hypervisor memory plus hardware trap/SYSCALL entry. The 2.x supervisor is intended to remain a small protection/virtualization monitor, not to absorb normal kernel services.

Hardware page-fault support is a 2.x investigation. If validated on the production core with suitable semantics, demand paging may use genuine Hypervisor page-fault traps. This is not a 1.x dependency.
