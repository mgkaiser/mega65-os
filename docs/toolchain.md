# Toolchain and C Dialect

## Language strategy

Do not begin by writing a compiler from scratch.

First implement the architecture using standard C plus explicit runtime types/functions. Adapt an existing compiler/backend if practical. Phase 1 is locked to LLVM-MOS/Clang targeting 45GS02 (`-mcpu=mos45gs02`). Compiler extensions are deferred until the bootstrap ABI is working.

Add language/compiler extensions only where they produce measurable value.

## Resident kernel language policy

The Phase-1 compiler choice does not imply that the resident nucleus should be predominantly C. The $E000-$FFFF nucleus has only 8 KiB, so implementation language is selected per mechanism using measured linked size.

Use 45GS02 assembly freely where it materially reduces resident size or avoids unnecessary C ABI/compiler scaffolding. Interrupt and BRK veneers, MAP manipulation, Base Page/stack transitions, context switching, flat-memory access, compiler-runtime helpers and eventual object-call trampolines are natural assembly candidates. Keep C where its generated code is comparably compact and its clarity is useful.

Prefer moving larger policy code out of the permanent nucleus into pageable/loadable objects over hand-optimizing policy code merely to keep it resident.

### Build inspection artifacts

Normal builds keep runnable images in `build/`, exact-link ELF companions in `build/debug/`, linker maps in `build/map/`, and source-interleaved disassembly in `build/lst/`. The listings are generated from the final linked ELF rather than from a separate compiler-only assembly pass, so optimization decisions can be based on the code and addresses that actually reached the image. Raw flat-image length is not sufficient to measure resident usage when the linker pads an extent.

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