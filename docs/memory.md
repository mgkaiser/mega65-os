# Memory System

## Hierarchy

Treat memory/storage as tiers:

    CPU logical 64K mapping
        -> first ~384K fast/hot memory
        -> Attic RAM (warm)
        -> SD/disk/file backing (cold)

The first ~384K should contain the current working set rather than statically assigned programs.

## Eight-page logical view

The CPU's 64 KiB logical view is treated as eight 8 KiB pages:

    $0000-$1FFF   Page 0   active thread
    $2000-$3FFF   Page 1   demand-paged process working set
    $4000-$5FFF   Page 2   demand-paged process working set
    $6000-$7FFF   Page 3   demand-paged process working set
    $8000-$9FFF   Page 4   demand-paged process working set
    $A000-$BFFF   Page 5   demand-paged process working set
    $C000-$DFFF   Page 6   kernel extension/driver execution window
    $E000-$FFFF   Page 7   resident kernel nucleus

Page 0 belongs to the active thread. The default ABI may use $0000 as Base Page, $0100 as the initial stack area, and $0200 onward for thread/process code and data. These addresses are logical conventions, not permanently reserved physical memory. Paging the Page-0 backing extent switches the low-address execution environment with the thread.

Pages 1-5 are the ordinary demand-paged process working set.

Page 6 is reserved as the kernel's pageable executable-extension window. Driver and extension state need not be mapped there: flat/far addressing can reach physical memory outside the current 64 KiB view.

Page 7 is invariant and contains the resident 8 KiB kernel nucleus and normal-mode vectors.

## Mapping versus direct far access versus moving

Use the cheapest mechanism appropriate to locality:

- Already mapped data: ordinary near access.
- Short/sparse out-of-map access: 45GS02 flat/far addressing.
- Repeated access to a compact region, or code that must execute: MAP it.
- Heavy/bulk copy, fill, or promotion: use DMAgic.

MAP should therefore be viewed primarily as an execution/locality mechanism. Out-of-map data does not need to be mapped merely to be readable or writable.

DMAgic is treated as synchronous for scheduling/design purposes; do not assume useful CPU concurrency during DMA.

## Paging quantum

Current proposal: **8 KiB** as the fundamental mapping/residency/page quantum.

Kernel allocation should operate in pages/extents/objects, not individual C allocations.

A memory object may be arbitrary sized and consist of independently backed 8K pages. Physical contiguity is normally irrelevant.

## Kernel extensions

Code that does not fit in the resident Page-7 nucleus can be packaged as pageable kernel extensions and drivers. When such code must execute, the nucleus maps the required executable extent into Page 6 ($C000-$DFFF) and transfers control to it.

The extension's persistent state, buffers, kernel objects, and other data may remain elsewhere in physical memory and be accessed with flat/far operations. Evicting a clean Page-6 code mapping therefore need not imply copying driver state out of the window.

Nested extension calls require the kernel to preserve/restore Page-6 mapping state.

## I/O access policy

Device I/O follows the same locality rule as ordinary memory.

Sparse register access should normally use flat/far loads and stores to the physical I/O address. Although this costs more per register access than a near access through the conventional $D000 I/O aperture, it avoids the fixed cost of changing the mapping/I/O personality and then restoring it.

For a dense I/O sequence, temporarily exposing the conventional $D000 aperture and using near loads/stores may be faster. The OS should benchmark the crossover and permit drivers to choose an I/O-intensive near-access section where justified.

Consequently, Page 6 should not be designed as though its upper half is permanently lost to I/O. A driver may retain the full $C000-$DFFF executable window while performing sparse hardware access through flat addressing.

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

## Bootstrap reservations and mapping API

The transition loader describes every preloaded physical extent in a versioned boot manifest. Early kernel startup copies those extents into the VM reservation table before Page 0 is reassigned. This is the seed of the normal physical allocator/pager state: preloaded code is not special memory once registered.

Kernel clients do not manipulate MAP encodings. They acquire a semantic mapping window with a physical address and receive an opaque restore token. The current implementation supports the Page-6 extension window and nested LIFO mappings; later backing objects, replacement and >1 MiB MAP sequencing can be added behind the same interface.

The mapper owns a software shadow because normal code cannot rely on reading MAP state directly. Page 7 remains resident while Page 6 is remapped.
