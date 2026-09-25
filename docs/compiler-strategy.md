# C Compiler Strategy

## Goal

Implement as much of MEGA65 OS as practical in C. Assembly should be confined to machine-ABI necessities: initial entry/bootstrap, interrupt/context entry/exit, mapping primitives, unusual instructions unavailable to C, and measured low-level hot paths.

The compiler should evolve with the OS instead of requiring the final language implementation before the kernel exists.

## Three phases

### Phase 1 — unmodified open-source C compiler

Bring up the OS with an **unmodified C compiler for which we have complete source code**.

Requirements:
- complete source under a license permitting modification/redistribution;
- usable 65xx/45GS02 generation, or a credible path to it;
- ABI we can wrap with a small assembly runtime;
- output that our linker/postprocessor can turn into `.mex`;
- no compiler changes required for initial kernel bring-up.

Unusual OS concepts are initially explicit runtime APIs and types: memory-object handles, far-pointer descriptors, map/pin/unmap calls, and far-call helpers.

The first kernel therefore depends on an upstream compiler, small assembly runtime, our libc/runtime, and our linker/binder tooling—not on compiler development.

### Phase 2 — modify the compiler

Fork/adapt the chosen compiler after the ABI and memory model work.

Teach it:
- 16-bit near pointers;
- 64-bit far object pointers;
- far function pointers;
- kernel-only physical pointers;
- per-thread relocatable zero page;
- per-thread stack/TLS;
- code/data extents;
- canonical logical execution windows;
- OS syscall/calling conventions;
- mapping and pinning semantics;
- resident/boot/driver annotations.

Syntax is not frozen. Semantics come first.

#### Virtual-memory-aware optimization

A far pointer is not merely a wide integer pointer. It denotes an OS-managed object whose pages may require mapping/promotion.

For dense loops, the compiler can hoist resolution out of the inner loop: map/pin a useful range, use ordinary 16-bit addressing, advance/remap at an 8K boundary, then unpin.

Optimization can consider access density, sequential versus sparse access, 8K boundaries, mapping cost, DMA promotion cost, object lifetime, and read/write behavior.

#### DMA-aware optimization

Recognize profitable copies/fills and related loops and lower them to DMAgic operations.

DMA is treated as synchronous, so profitability compares CPU cost against DMA setup + transfer cost and does not assume useful CPU/DMA overlap.

#### Extent-aware generation

Keep strongly connected functions together so local calls remain near calls. Cross-extent calls use the far-call/binder trampoline.

Emit metadata useful to the linker: call graph and estimated weights, code/data references, sizes, hot/cold hints, address-taken functions, far-call requirements, zero-page/TLS requirements.

### Phase 3 — optional purpose-built compiler

Only consider a from-scratch compiler after Phase 2 demonstrates where the inherited compiler architecture fights the OS.

A custom compiler is worthwhile if it materially improves:
- far-pointer semantics/optimization;
- automatic near/far handling;
- object bounds/protection checks;
- 8K-page-aware loops;
- mapping-lifetime analysis;
- DMA generation;
- extent construction;
- call-graph-driven placement;
- per-thread zero-page allocation;
- MEGA65-specific instruction selection;
- whole-program metadata exported to `.mex`;
- compiler/linker/binder/pager/profile cooperation.

Its distinguishing feature would be a **native model of the OS object/virtual-memory system**, not merely better 45GS02 instruction generation.

## Compiler / linker / binder split

**Compiler knows:** C semantics, pointer kinds, access patterns, function/data relationships, optimization opportunities, source attributes.

**Development linker knows:** whole-program symbol/call/dependency graph, extent composition, canonical logical windows, imports/exports/relocations, static placement hints.

**OS binder knows:** current Fast/Attic pressure, available memory, loaded/shared modules, process mappings, runtime profiles, and current resources.

Compiler/linker describe possibilities and costs. Binder makes current physical-placement decisions.

## Stable runtime contract first

Even later compiler phases should lower special language features into stable ABI/runtime primitives where practical. That provides a reference implementation, easier debugging, older-compiler compatibility, ABI stability, and direct optimized-vs-unoptimized testing.

## Cross compilation and self-hosting

Initial development should be cross-compiled on a modern host. Eventual self-hosting is interesting but should not constrain early architecture.

Large portions of the OS should remain ordinary C even after specialized compiler support exists.

## Candidate Phase-1 compiler investigation

Candidates already discussed:
- LLVM-MOS
- vbcc
- cc65

Selection requires investigation of:
- source availability and license;
- 45GS02/MEGA65 support;
- generated-code quality;
- backend maintainability;
- optimizer extensibility;
- object/linker architecture;
- multiple address-space/pointer support;
- zero-page control;
- calling-convention control.

Do not choose solely on today's benchmark performance. Phase 2 requires a compiler codebase we can realistically teach our memory model.

## Principle

> First express the OS memory model in C APIs. Then teach the compiler what those APIs mean. Only then decide whether the compiler itself should be replaced.

The OS should drive the language/toolchain design rather than being constrained by the first compiler.