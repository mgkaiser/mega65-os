# Pointers and ABI

## Near pointers

Ordinary C pointers remain 16-bit and refer to addresses in the CPU's currently mapped 64K logical address space.

This keeps normal C code efficient.

## Far pointers

Far pointers are 64-bit stable object addresses.

Conceptually:

    attributes | object identity | offset

Exact bit allocation is intentionally not frozen.

A far pointer means **object + offset**, never physical RAM address. It survives:
- Fast/Attic migration
- compaction
- paging
- file backing
- remapping

Attributes may describe intended semantics such as R/W/X, file-backed, shared, COW, device, pinned, etc. Pointer bits are not authoritative permission; kernel object metadata is.

## Kernel physical addresses

Physical addresses are kernel-only implementation details. Applications should not manipulate them.

## Efficient far access

Do not resolve every far byte access through a heavy trap. Runtime/compiler should map/pin useful ranges and use near pointers for tight loops.

## Far function pointers

Far function pointers are first-class and use the same object-address concept.

A far call:
1. validates executable authority,
2. identifies code object/extent,
3. makes required extent resident,
4. maps it into its canonical logical execution window,
5. establishes return mapping state,
6. calls entry offset.

Near function pointers remain ordinary cheap addresses within a currently valid mapping.

Core rule:

> A near pointer identifies an address in the current CPU mapping. A far pointer identifies an address in the OS object universe.

Code versus data is determined by type and enforced object permissions.

## Cross-process calls

Do not execute another process's far function pointer directly. Cross-process invocation uses IPC/service endpoints. Shared libraries may expose executable objects mapped into multiple processes.