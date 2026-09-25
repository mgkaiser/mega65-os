# Toolchain and C Dialect

## Language strategy

Do not begin by writing a compiler from scratch.

First implement the architecture using standard C plus explicit runtime types/functions. Adapt an existing compiler/backend if practical. LLVM-MOS, vbcc, and cc65 have been discussed as possible starting points; actual 45GS02/MEGA65 suitability must be investigated.

Add language/compiler extensions only where they produce measurable value.

## Concepts compiler should eventually understand

- 16-bit near pointers
- 64-bit far pointers
- far function pointers
- kernel physical pointers
- relocatable/bankable extents
- per-thread zero page
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