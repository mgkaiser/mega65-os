# MEGA65 OS source tree

`Makefile` is the master build. Each independently built OS component lives in
its own subdirectory and owns its own Makefile.

Current components:

- `loader/` — phase-1 transition loader using stock Hyppo to load and hand off
  to the native kernel.
- `kernel/` — freestanding resident-kernel nucleus and early BRK/kernel-entry
  plumbing.
- `docs/` — canonical architecture and design documents.
- `project/` — repository copy of the project backlog and architecture tracking
  data.

Toolchain: **LLVM-MOS/Clang**, using `mos-mega65-clang` and the 45GS02 target.

See **[BUILDING.md](BUILDING.md)** for compiler installation, toolchain links,
build commands, outputs, and current bring-up caveats.

## Bring-up status

The MEGA65 book (`mega65-book.pdf`) is the authoritative source for hardware
and Hyppo behaviour.

The architecture now assigns the resident kernel nucleus to the single 8 KiB
logical page at `$E000-$FFFF`. Some current loader/linker code still reflects
the earlier `$4000` bring-up location and must be migrated before the current
binaries should be treated as bootable release images.
