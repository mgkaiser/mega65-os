# Memory System

## Hierarchy

Treat memory/storage as tiers:

    CPU logical 64K mapping
        -> first ~384K fast/hot memory
        -> Attic RAM (warm)
        -> SD/disk/file backing (cold)

The first ~384K should contain the current working set rather than statically assigned programs.

## Mapping versus moving

- Short/sparse Attic access: temporarily map it.
- Heavy/repeated/sequential access: promote/copy into fast memory with DMA.
- Prefer mapping unless the expected future access cost justifies movement.
- DMAgic is treated as synchronous for scheduling/design purposes; do not assume useful CPU concurrency during DMA.

## Paging quantum

Current proposal: **8 KiB** as the fundamental mapping/residency/page quantum.

Kernel allocation should operate in pages/extents/objects, not individual C allocations.

A memory object may be arbitrary sized and consist of independently backed 8K pages. Physical contiguity is normally irrelevant.

## C heap versus OS allocator

Kernel:
- memory objects
- pages/extents
- mappings
- backing
- permissions
- ownership
- residency

libc:
- `malloc`
- `free`
- `realloc`
- suballocation and fragmentation inside mapped heap pages

Ordinary `malloc()` returns conventional 16-bit near pointers. libc asks the kernel for additional heap pages when necessary and may return wholly unused pages.

## Far allocations

Far allocations are OS memory objects rather than ordinary heap blocks.

A huge virtual allocation can reserve an object without immediately allocating backing pages. Pages materialize on first touch.

Possible API split:

    mem_create(size, flags)
    mem_resize(handle, new_size)
    mem_ptr(handle, offset)

with `far_malloc`, `far_realloc`, and `far_free` as libc conveniences.

## Resizing

Because a far pointer identifies object + offset rather than physical location, an object can grow without moving existing data or invalidating pointers into the retained range.

Growing mostly changes object metadata/page slots. New pages can remain uncommitted until touched.

Shrinking releases pages beyond the new end. Existing pointers beyond the new boundary become invalid.

## Backing policies

- Clean read-only/file-backed pages: discard and reload.
- Dirty file-backed pages: write back appropriately.
- Anonymous writable pages: Fast <-> Attic <-> swap.
- Executable code: naturally file-backed and discardable when clean.
- Device-backed pages: special semantics, not normal paging.

## Replacement policy

Go beyond strict LRU. Candidate scoring can consider:
- recency
- frequency/heat
- transfer cost
- reconstruction/reload cost
- dirty/writeback cost
- call/dependency relationships
- pinning/realtime requirements

Static dependency information plus runtime observations should help retain related hot extents.