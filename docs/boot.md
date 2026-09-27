# Boot and Takeover

## Current direction

The stock environment launches a tiny transition loader. It uses stock Hyppo only long enough to preload the resident nucleus and boot-critical modules.

Phase 1 currently preloads:
- `kernel.bin` at physical/logical `$00E000`;
- `console.bin` into one 8 KiB physical page at `$020000`.

The loader writes a versioned boot manifest at real logical `$0200`. The kernel consumes it before any process MAPLO mapping is installed and seeds 8 KiB physical-page reservations from it.

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
          -> overlay console at logical $8000-$9FFF
          -> call console init/write
          -> later start allocator/pager/process machinery

## Native split during boot

    $0000-$7FFF   real kernel/bootstrap low memory
    $8000-$9FFF   console overlay while selected
    $A000-$BFFF   real upper memory
    $C000-$DFFF   real kernel/I/O
    $E000-$FFFF   resident nucleus

Later, each process receives four contiguous 8 KiB pages mapped over $0000-$7FFF. Pages $8000-$DFFF become the shared upper working set for process data and pageable kernel components, with hot real kernel content visible whenever a slot is untranslated.

## Boot-critical modules

Preloading is a residency decision, not a different module type. The bootstrap console is an ordinary 8 KiB allocation loaded at physical $020000 and overlaid at $8000-$9FFF. The same mechanism evolves into upper-working-set paging.

## Transition loader

Keep it small: load boot-critical images, publish their physical extents, establish bootstrap state, and transfer control. General allocation, executable loading and paging belong to the kernel.

## Version boundary

1.x retains stock Hyppo. 2.x may replace/extend `HICKUP.M65` with a protected supervisor.
