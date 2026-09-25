# MEGA65 OS source tree

`Makefile` is the master build. Each independently built OS component lives in
its own subdirectory and owns its own Makefile.

Current components:

- `loader/` — phase-1 transition loader. It reclaims the compatibility-ROM RAM
  using the Hypervisor and will load the kernel using Hyppo file services.
- `kernel/` — initial freestanding kernel stub; intentionally does nothing.

Toolchain: LLVM-MOS, using `mos-mega65-clang`.

## Bring-up status

The MEGA65 book (`mega65-book.pdf`) is the authoritative source for hardware
and Hyppo behaviour. The loader is currently a bring-up skeleton; kernel loading
and final memory/linker details must follow the documented Hyppo ABI rather than
guessed conventions.
