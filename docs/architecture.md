# Architecture

## Philosophy

MEGA65 OS is designed around the machine's actual mapping hardware rather than pretending MAP is an eight-entry MMU. The CPU has a 64 KiB logical view and a much larger physical space; MAP supplies one translation offset for the lower 32 KiB and one for the upper 32 KiB, with individual 8 KiB enable bits inside each half.

The central abstraction remains: addressable objects are identified independently of their current physical backing. Names locate objects, handles grant authority, and the memory manager decides what is resident and what is mapped.

## Major layers

1. Hypervisor transition/bootstrap
2. Tiny resident kernel nucleus
3. Memory/object manager
4. Scheduler and interrupt machinery
5. Device namespace and drivers
6. IPC and services
7. VFS/filesystems
8. Console/TTY/PTY/shell
9. Optional networking
10. Optional graphics/window system/GUI

## Split logical address space

The native 1.x address-space model is:

    $0000-$7FFF   process half-space (MAPLO)
    $8000-$9FFF   pageable kernel/driver execution slab (MAPHI Page 4)
    $A000-$BFFF   reserved for expansion of kernel execution window (MAPHI Page 5)
    $C000-$CFFF   real kernel RAM/workspace
    $D000-$DFFF   conventional near I/O aperture
    $E000-$FFFF   real resident kernel nucleus and vectors

The lower 32 KiB is the normal process execution mapping. A process receives a contiguous 32 KiB physical mapping extent so one MAPLO offset can translate the whole lower half. Its logical zero/Base Page and user stack therefore live at the conventional low addresses inside that mapped extent.

The kernel uses the **real/untranslated lower address space** for its own Base Page, stack and low-memory workspace. Kernel entry must safely preserve the hardware-pushed user interrupt/BRK frame before switching MAPLO back to real lower memory; the exact entry veneer remains implementation work.

The upper half deliberately leaves Pages 6 and 7 untranslated. This preserves real RAM at $C000-$CFFF, the conventional near I/O aperture at $D000-$DFFF, and the resident nucleus at $E000-$FFFF. Pageable kernel modules execute initially at $8000-$9FFF. Page 5 can later join Page 4 to form a contiguous 16 KiB extension window when a module's physical placement shares the same MAPHI displacement.

The resident nucleus retains the hard **8 KiB $E000-$FFFF** budget.

## MAP constraints

MAP does not provide eight independent physical page registers. Each 32 KiB half has one shared translation offset plus four 8 KiB enable bits. A zero enable bit means that logical slab is untranslated; it does not preserve an older mapping. Every MAP operation therefore establishes the complete mapping state represented by its registers.

This makes **32 KiB the normal process mapping extent**, but not necessarily the smallest allocation object. The memory manager may account for smaller extents/objects; it simply cannot expose arbitrary unrelated 8 KiB physical pages simultaneously in one 32 KiB logical half using MAP alone.

## Access strategy

Use ordinary near access for currently mapped data and near I/O. Use MAP for executable/local working sets and DMAgic for bulk movement. The 45GS02 flat-memory mechanism remains architecturally useful for sparse far data/I/O, but Phase-1 C compiler support for the intended far-pointer ABI is not yet implemented; assembly veneers may bridge that gap when needed.

## Native software rule

Native applications do not poke hardware registers by default. Hardware is owned by OS-controlled devices; explicit capabilities/exclusive ownership may grant lower-level access.

## Version boundary

1.x uses stock Hyppo only for launch/takeover. 2.x may introduce a MEGA65-OS-specific Hypervisor/supervisor for stronger protection, syscall trapping, virtualization and possible hardware page-fault VM after hardware validation.
