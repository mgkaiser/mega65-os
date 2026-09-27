# Toolchain and C Dialect

## Language strategy

Do not begin by writing a compiler from scratch.

First implement the architecture using standard C plus explicit runtime types/functions. Adapt an existing compiler/backend if practical. Phase 1 is locked to LLVM-MOS/Clang targeting 45GS02 (`-mcpu=mos45gs02`). Compiler extensions are deferred until the bootstrap ABI is working.

Add language/compiler extensions only where they produce measurable value.

## Concepts compiler should eventually understand

- 16-bit near pointers
- 64-bit far pointers
- far function pointers
- kernel physical pointers
- relocatable/bankable extents
- process-half Base Page/zero-page conventions
- TLS
- OS syscall ABI
- far-call trampolines
- pin/map ranges
- memory-object semantics

Possible attributes/keywords might cover:
- far
- resident
- banked/extent
- driver
- thread/TLS
- zeropage

Exact syntax is not decided.

## Optimizations

Potential future optimizations:
- turn suitable bulk loops into DMA operations
- hoist far-pointer resolution/map operations out of loops
- infer useful pin/map ranges
- cluster related functions into extents

## Hardware isolation

Except for the architecture/device-driver layer, native C code should not contain hardware register addresses.