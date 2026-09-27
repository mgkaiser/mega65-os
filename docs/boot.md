# Boot and Takeover

## Current direction

The stock environment launches a tiny transition loader. It uses stock Hyppo only long enough to preload the resident nucleus and boot-critical modules.

Phase 1 currently preloads:
- `kernel.bin` at physical/logical `$00E000`;
- `console.bin` into an 8 KiB physical extent at `$020000`.

The loader writes a versioned boot manifest at real logical `$0200`. The kernel consumes it before any process MAPLO mapping is installed and seeds physical-memory reservations from it.

## Handoff

    firmware / stock environment
      -> transition loader
          -> load kernel.bin at $E000
          -> load console.bin at physical $020000
          -> publish boot manifest at $0200
          -> establish untranslated/native bootstrap map
          -> jump $E000
      -> resident nucleus
          -> consume manifest
          -> initialize complete MAP software shadow
          -> map console at logical $8000-$9FFF
          -> call console init/write
          -> later start normal loader/pager/process machinery

## Native split

    $0000-$7FFF   process MAPLO half (real during bootstrap/kernel)
    $8000-$9FFF   module execution slab
    $A000-$BFFF   future second module slab
    $C000-$CFFF   real kernel RAM
    $D000-$DFFF   real near I/O
    $E000-$FFFF   resident kernel

During bootstrap MAPLO is disabled, so the manifest at $0200 and kernel low-memory state are real. Later process execution maps a contiguous 32 KiB process extent over $0000-$7FFF. Kernel entry restores the real lower half only after preserving the process's hardware-pushed frame.

## Boot-critical modules

Preloading is a residency decision, not a different module type. The console uses the same versioned module header and mapping/call path intended for later demand-loaded drivers.

The console is now linked for $8000. Phase 1 maps one 8 KiB slab there while leaving $A000-$FFFF untranslated, which preserves near I/O at $D000 and the resident nucleus at $E000.

## Transition loader

Keep it small: load boot-critical images, publish their physical extents, establish the bootstrap machine state, and transfer control. General executable loading and paging belong to the kernel.

## Version boundary

1.x retains stock Hyppo. 2.x may replace/extend `HICKUP.M65` with a protected supervisor.
