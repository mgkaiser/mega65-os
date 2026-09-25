# Boot and Takeover

## Current direction

Do not gradually coexist with the stock operating environment. The stock environment launches a tiny transition loader, which uses stock Hyppo only long enough to preload the resident nucleus and boot-critical modules.

Phase 1 currently preloads:
- `kernel.bin` at physical/logical `$00E000`,
- `console.bin` into an 8 KiB physical extent at `$020000`.

The loader then writes a **versioned boot manifest** at logical `$0200`. Each manifest entry identifies the module kind, physical extent and extent size. This is not a permanent kernel data location: the kernel consumes the manifest before Page 0 becomes an active-thread page and seeds its physical-memory reservation table from it.

This establishes the long-term boot rule: the loader may preload modules required before the normal loader/pager is usable; later modules are demand-loaded by the kernel. The manifest ABI is versioned so adding richer module metadata does not require changing the basic handoff model.

The loader disables compatibility-ROM write protection, establishes the native RAM/I/O view, and transfers control to `$E000`.

## Initial handoff

    firmware / stock environment
      -> transition loader
          -> load kernel.bin at $E000
          -> load boot-critical modules into physical extents
          -> publish versioned boot manifest at $0200
          -> establish native RAM/I/O view
          -> jump $E000
      -> resident Page-7 nucleus
          -> consume manifest into VM reservation state
          -> initialise MAP shadow
          -> map console module into Page 6
          -> call console init/write entry points
          -> normal loader/pager later takes over

The resident kernel occupies `$E000-$FFFF`, including NMI, RESET and IRQ/BRK vectors at `$FFFA-$FFFF`.

## Initial logical map

    $0000-$1FFF   active-thread Page 0
    $2000-$BFFF   demand-paged process Pages 1-5
    $C000-$DFFF   kernel extension execution Page 6
    $E000-$FFFF   resident kernel Page 7

During early boot, Page 0 still contains loader handoff data. The kernel must consume that data before installing the first thread mapping.

## Boot-critical modules

A preloaded module is still a normal pageable kernel module; preloading changes only how its physical backing becomes resident. The kernel invokes it through the same Page-6 mapping/module ABI that demand-loaded modules will use later.

The initial console module is linked for Page 6 and is constrained to `$C000-$CFFF` while the conventional `$D000-$DFFF` I/O aperture is exposed during bring-up. This is a bootstrap constraint, not a permanent reduction of the architectural 8 KiB extension window.

## Transition loader

Keep extremely small. It may preload boot-critical images and describe them in the manifest, but it should not grow a filesystem, general executable loader, allocator or policy engine.

## Versioned Hypervisor Strategy

1.x retains stock Hyppo. 2.x may replace/extend `HICKUP.M65` with a protected supervisor; hardware page-fault VM remains a 2.x investigation.
