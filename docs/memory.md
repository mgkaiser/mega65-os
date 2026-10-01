# Memory System

## Hardware mapping model

The CPU sees 64 KiB. MAP supplies a shared offset for each 32 KiB half and individual 8 KiB enable bits within each half. Enabled slabs in the same half share one displacement. A later MAP replaces selector/offset state; a zero selector bit means untranslated/real addressing.

## Allocation quantum

**The physical allocator works in 8 KiB pages.** 8 KiB is also the normal residency, replacement and eviction quantum.

A new process must allocate **four contiguous 8 KiB physical pages** for its primary 32 KiB lower-half arena. This contiguity is required because MAPLO has one displacement for all four logical slots. It does not change the allocator's fundamental 8 KiB unit.

Objects outside the primary process arena may consist of ordinary 8 KiB pages and may be backed, promoted or evicted independently subject to mapping constraints.

## Logical layout

    $0000-$7FFF   active process primary arena (4 contiguous 8K pages via MAPLO)
    $8000-$9FFF   upper working-set slot 0
    $A000-$BFFF   upper working-set slot 1
    $C000-$DFFF   upper working-set slot 2; includes $D000 I/O when untranslated
    $E000-$FFFF   resident 8K nucleus

### Lower half

The process Base Page/zero page, user stack, code and near data live inside its four-page primary arena. When MAPLO is disabled, the same logical addresses expose real low memory used for the kernel Base Page, kernel stack and workspace.

Interrupt/BRK entry must preserve the user frame pushed while MAPLO still describes the process before exposing the real kernel lower half.

### Upper 24 KiB working set

Pages 4-6 are a shared cache/window for **both active-process pages and pageable kernel components**. The OS should favor frequently used kernel drivers/modules in the real $8000-$CFFF region where practical; less-common kernel pages and process data can page over upper slots as needed.

MAPHI imposes a co-residency rule: all upper slots enabled in one MAP state use the same displacement. Thus a replacement family must be physically arranged as the corresponding 8 KiB positions in a contiguous run. Roles do not have to match: one enabled slot may be driver code and another process data if their physical placement satisfies the common displacement.

Untranslated slots expose their real addresses, allowing high-priority kernel code/data to remain underneath pageable overlays.

Page 6 spans $C000-$DFFF. Mapping it overlays both $C000-$CFFF and the conventional $D000-$DFFF I/O aperture. Leave Page 6 untranslated when near I/O is required, or use mapping transitions/flat I/O where appropriate.

Page 7 normally remains untranslated and permanently exposes the resident nucleus.

## Allocation versus mapping

- **8 KiB:** physical allocation, residency and eviction unit.
- **32 KiB contiguous (4 x 8 KiB):** mandatory primary allocation for a new process.
- **Upper 8 KiB slots:** shared process/kernel working-set slots.
- **MAPHI replacement family:** enabled upper slots must share one displacement and therefore correspond to a contiguous physical layout.
- **Top 8 KiB:** resident kernel nucleus.

## MAP-aware physical allocation policy

The allocator remains an **8 KiB page allocator**, but it treats each naturally
aligned 32 KiB physical region as a **MAP neighborhood** containing four 8 KiB
slots. This is an optimization structure, not a larger allocation quantum.

Physical contiguity is unusually valuable on the MEGA65 because the four
selectors in a MAP half share one displacement. Pages that are likely to be
visible simultaneously should therefore be placed at physical offsets that
match their intended logical-slot relationship. The allocator optimizes for
**MAP compatibility**, not contiguity for its own sake.

Each 32 KiB neighborhood can be represented by a four-bit occupancy mask.
Allocation policy should preserve completely free neighborhoods when a request
can instead consume suitable holes in an already-partial neighborhood. In
other words: **fill damaged neighborhoods before damaging pristine ones.**
This reduces fragmentation of the 32 KiB runs required for new process primary
arenas.

Preferred placement is:

- Four-page / 32 KiB requests use an empty, naturally 32 KiB-aligned
  neighborhood.
- Three-page requests prefer three suitable pages within one neighborhood,
  contiguous when the requested logical-slot pattern is contiguous.
- Two-page requests prefer a compatible pair within one neighborhood. There is
  no general requirement for 16 KiB alignment; relative 8 KiB slot position is
  what matters to MAP.
- One-page requests prefer a suitable free slot in an already-partial
  neighborhood rather than consuming a slot in a pristine neighborhood.
- Pages belonging to the same mapping-affinity group receive a strong
  preference for the same neighborhood and for physical slot positions
  matching the logical slots in which they are expected to appear.

An allocation request may therefore eventually describe both a page count and
a desired four-bit **slot mask** (plus an affinity identity). For example, a
request for logical slots 0 and 2 should prefer physical positions 0 and 2 in
one neighborhood rather than merely any two contiguous pages. That allows both
pages to be exposed by one MAP displacement.

Candidate neighborhoods should be scored rather than governed by rigid
alignment rules. The preferred ordering is:

1. exact MAP-slot compatibility;
2. placement with the same mapping-affinity group;
3. consumption of an already-fragmented neighborhood;
4. useful physical contiguity;
5. avoidance of breaking a pristine 32 KiB neighborhood;
6. avoidance of awkward residual holes when otherwise equivalent.

Affinity is a placement preference, not ownership of an entire neighborhood.
Unrelated objects may share a neighborhood when doing so does not defeat a
stronger MAP requirement.

Objects larger than 32 KiB are composed of multiple MAP-sized cohorts. The
allocator should prefer whole-object physical contiguity when inexpensive
(which can also benefit DMA), but only each cohort's MAP-compatible placement
is fundamental. The resulting hierarchy is therefore:

    8 KiB    allocation / backing / residency / eviction quantum
    32 KiB   MAP-affinity neighborhood
    >32 KiB  object composed of one or more MAP neighborhoods/cohorts

The allocator should maintain cheap visibility of pristine versus partial
neighborhoods (for example free-32K and partial-32K lists plus the four-bit
occupancy mask). This makes the common placement decisions inexpensive without
requiring a conventional buddy allocator.

This policy does **not** require 32 KiB alignment for a one-page allocation or
16 KiB alignment for a two-page allocation. Such rigid alignment would reduce
placement choices and can strand otherwise useful 8 KiB slots. Alignment is
required where MAP semantics demand it; otherwise packing and slot affinity
are the optimization goals.

## Memory tiers

    CPU logical mapping
        -> first ~384K hot memory
        -> Attic RAM
        -> SD/disk/file backing

Placement policy should keep hot process pages and frequently used kernel pages in fast physical memory while preserving contiguous four-page runs for process creation.

## Far access and DMA

Ordinary near access is preferred for visible memory and I/O. Flat/far 45GS02 addressing remains the sparse out-of-map mechanism, but the Phase-1 LLVM-MOS C ABI does not yet provide the intended far-pointer lowering. DMAgic is the bulk copy/fill mechanism.

## Bootstrap reservations

The loader describes preloaded physical extents in a versioned boot manifest. Early kernel startup consumes this before the first process MAPLO mapping. Preloaded modules then participate in the same 8 KiB page accounting and residency model as later demand-loaded modules.

The mapper owns a complete software shadow of MAP state. Kernel clients request semantic mappings rather than constructing MAP registers directly.
