# Architecture

## Philosophy

The OS should be designed around what the MEGA65 actually is.

A useful mental model is a small mainframe/minicomputer-style OS implemented on an 8-bit CPU with:
- a small immediately addressable CPU space,
- considerably more physical memory,
- mapping hardware,
- DMAgic,
- fast and slower memory tiers,
- persistent storage.

The central abstraction is increasingly:

> Everything that can reasonably be represented as an addressable object should be an addressable object.

Names locate objects. Handles grant authority. Far pointers identify locations within objects. The memory manager decides how those locations become accessible to the CPU.

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

Most OS services should be dynamically loadable. Only machinery required to load/page/schedule the rest must remain resident.

## Resident Nucleus Size

The permanently resident kernel nucleus has a hard design target of **one 8 KiB extent**. Interrupt entry, BRK dispatch, the minimum scheduler/memory machinery required to make other components available, and other irreducible kernel mechanisms compete for this same budget. Policy-heavy code, filesystems, normal drivers, services, and optional facilities should remain pageable/loadable rather than growing the resident nucleus.

The build must enforce the 8 KiB resident-image limit so size pressure is visible immediately rather than becoming a late optimization project.

## Native software rule

Native applications should not poke hardware registers directly by default. Hardware belongs to OS-controlled devices. Explicit capabilities/exclusive ownership can grant near-bare-metal access to games, demos, or specialized software.

## Compatibility

Legacy Commodore/C65/MEGA65 compatibility can eventually be a personality that establishes the expected memory/ROM environment. Native ABI design is not constrained by BASIC/KERNAL conventions.

## Version Boundary: 1.x vs 2.x

MEGA65 OS 1.x uses the stock Hyppo/MEGA65 Hypervisor as its launch and platform boundary. The 1.x architecture should avoid choices that block later supervisor integration, but it does not depend on a custom Hypervisor.

MEGA65 OS 2.x is the planned point for a MEGA65-OS-specific Hypervisor/supervisor (`HICKUP.M65`). Candidate 2.x responsibilities include protected syscall entry, privilege enforcement, fault handling, stronger isolation of native code, hardware page-fault-backed virtual memory if production hardware validation supports it, and hardware virtualization for compatibility environments. The protected 16 KiB Hypervisor region remains deliberately small; normal kernel policy, VFS, drivers, and services stay outside it.

## Storage and Commodore Media

Physical floppy support is a 1.x feature. The floppy controller/drive implementation is a device driver beneath a generic block/media interface, not kernel filesystem code. FAT and Commodore filesystem support are separate filesystem modules above that interface.

The same CBM filesystem/DOS implementation should operate on both physical Commodore media and image-backed media such as `.d64`, `.d71`, and `.d81`; only the media backend differs. The CBM layer must also model DOS command-channel/status semantics rather than treating Commodore media as only a generic sector filesystem. Disk-image files may therefore be mounted as virtual media while physical 1581 media can be mounted through the real floppy backend.
