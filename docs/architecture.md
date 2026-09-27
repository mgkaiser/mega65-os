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

## Native logical address space

    $0000-$1FFF   process page 0  ┐
    $2000-$3FFF   process page 1  │ MAPLO: four contiguous physical 8K pages
    $4000-$5FFF   process page 2  │
    $6000-$7FFF   process page 3  ┘
    $8000-$9FFF   upper working-set slot 0 / real hot kernel when untranslated
    $A000-$BFFF   upper working-set slot 1 / real hot kernel when untranslated
    $C000-$DFFF   upper working-set slot 2 / real kernel+I/O when untranslated
    $E000-$FFFF   resident kernel nucleus; normally untranslated

**8 KiB is the physical allocation, residency and eviction quantum.** A new process is the important exception to arbitrary placement: its primary lower-half arena allocates four contiguous 8 KiB pages (32 KiB total) because all four MAPLO selectors share one displacement.

The process's Base Page/zero page, user stack, code and near data live in that lower-half arena. Kernel execution uses the real/untranslated lower address space for its own Base Page, stack and workspace. Kernel entry must preserve the hardware-pushed user interrupt/BRK frame before changing MAPLO.

## Upper working set

The upper 24 KiB below the nucleus is a shared working-set/cache area for kernel code and active-process data. Frequently used kernel code may live at the real physical addresses beginning at $8000 and remain visible whenever the corresponding MAPHI selector is zero. Less-common drivers or process data may page over selected 8 KiB slots.

The replacement set is constrained by MAPHI: every enabled upper 8 KiB slot shares the same displacement. Therefore pages that are simultaneously overlaid must come from the corresponding positions in one contiguous physical run. The slots may have different roles (driver code, kernel data, process data); contiguity is a physical mapping constraint, not a type constraint.

Page 6 is the special case: its MAP slab is $C000-$DFFF, so overlaying it also overlays the conventional $D000-$DFFF I/O aperture. When Page 6 is untranslated, real $C000-$CFFF kernel content and near I/O remain visible. Code that needs Page-6 overlay and hardware I/O must use an appropriate mapping transition or flat/far I/O path.

The resident nucleus retains the hard **8 KiB $E000-$FFFF** budget.

## MAP constraints

MAP does not provide eight independent physical page registers. Each 32 KiB half has one shared translation offset plus four 8 KiB enable bits. A zero enable bit means that slab is untranslated; it does not preserve an older translated mapping. Every MAP operation establishes complete selector/offset state.

## Access strategy

Use ordinary near access for currently visible memory and I/O, MAP for executable/local working sets, flat/far access for sparse out-of-map data/I/O when supported by the ABI, and DMAgic for bulk movement.

## Native software rule

Native applications do not poke hardware registers by default. Hardware is owned by OS-controlled devices; explicit capabilities/exclusive ownership may grant lower-level access.

## Version boundary

1.x uses stock Hyppo only for launch/takeover. 2.x may introduce a MEGA65-OS-specific Hypervisor/supervisor for stronger protection, syscall trapping, virtualization and possible hardware page-fault VM after hardware validation.
