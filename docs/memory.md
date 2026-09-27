# Memory System

## Hardware mapping model

The CPU sees 64 KiB. MAP supplies a shared offset for each 32 KiB half and individual 8 KiB enable bits within each half. Enabled slabs in the same half therefore share one displacement. A later MAP replaces the selector/offset state; a zero selector bit means untranslated/real addressing.

## Native logical layout

    $0000-$7FFF   process half-space, translated by MAPLO
    $8000-$9FFF   kernel extension/driver execution slab
    $A000-$BFFF   second extension slab / future 16K window
    $C000-$CFFF   real kernel RAM
    $D000-$DFFF   real conventional I/O aperture
    $E000-$FFFF   real resident 8K nucleus

### Process half-space

The normal process mapping extent is **32 KiB contiguous physical memory**. One MAPLO displacement maps it at $0000-$7FFF. The process's Base Page/zero page, user stack, code and near data all live inside that translated lower half.

This is a mapping constraint, not a declaration that every allocation must consume 32 KiB. Objects and backing storage may use smaller units where useful, but arbitrary unrelated 8 KiB frames cannot be simultaneously presented as independent pages inside one MAP half.

### Kernel low memory

When MAPLO is disabled, $0000-$7FFF refers to real lower memory. The kernel uses real Base Page/zero page, a real kernel stack, and real low-memory workspace. Switching from a process mapping into this kernel environment requires an entry veneer that first preserves the user interrupt/BRK frame that hardware pushed while the process MAPLO mapping was active.

### Kernel upper memory

MAPHI is used only for selected kernel extension slabs. Phase 1 maps Page 4 ($8000-$9FFF) for a module while Pages 5-7 remain untranslated. This leaves $D000-$DFFF available as near I/O and $E000-$FFFF permanently resident.

Page 5 may later be enabled together with Page 4 for a 16 KiB contiguous module window. Because both share MAPHI, their physical backing must have the same logical-to-physical displacement.

## Allocation, residency and mapping

Keep these concepts separate:

- **Allocation/object size:** may be smaller than 32 KiB.
- **Backing/residency unit:** policy choice; 8 KiB remains a useful natural slab because MAP enable bits have 8 KiB granularity.
- **Process mapping extent:** normally 32 KiB contiguous because MAPLO has one shared offset.
- **Kernel module execution window:** initially 8 KiB at $8000; expandable to 16 KiB at $8000-$BFFF when contiguous.

## Memory tiers

    CPU logical mapping
        -> first ~384K hot memory
        -> Attic RAM
        -> SD/disk/file backing

The hot tier should contain active mapping extents and frequently used objects rather than statically assigned programs.

## Far access and DMA

Ordinary near access is preferred for mapped memory and near I/O. Flat/far 45GS02 addressing remains the sparse out-of-map mechanism, but the Phase-1 LLVM-MOS C ABI does not yet provide the OS's intended far-pointer lowering. DMAgic is the bulk copy/fill mechanism.

## Bootstrap reservations

The loader describes preloaded physical extents in a versioned boot manifest. Early kernel startup consumes this into physical-memory reservation state before the lower half is ever reassigned to a process. Preloaded modules then participate in the same residency/accounting model as later demand-loaded modules.

The mapper owns a complete software shadow of MAP state. Kernel clients request semantic windows rather than constructing MAP registers directly.
