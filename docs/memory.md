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
