# Rough Roadmap

This is deliberately provisional.

## Milestone 0 — verify hardware assumptions
- hypervisor loading/takeover facilities
- mapping granularity/capabilities
- stack and Base-Page behavior
- flat/far access to ordinary memory and physical I/O
- DMAgic behavior
- interrupt/vector behavior
- benchmark sparse far-I/O versus temporary near-I/O mapping crossover

## Milestone 1 — takeover
- launch transition loader
- use stock Hyppo to load raw resident kernel image at $E000
- abandon inherited environment
- enter assembly kernel entry, then C kernel
- place normal vectors at $FFFA-$FFFF inside the resident image
- enforce the resident nucleus as the single $E000-$FFFF 8 KiB extent

## Milestone 2 — nucleus
- memory/object manager with 8 KiB physical allocation/residency pages
- MAPLO 32K process-half machinery
- allocate four contiguous 8 KiB pages for each new process primary arena
- establish real kernel Base Page/stack transition on kernel entry
- establish $8000-$DFFF shared upper working set for hot kernel pages, pageable drivers and process data
- enforce MAPHI common-displacement/contiguous replacement-family rules
- timer IRQ
- scheduler
- threads with Base Page/stack state inside the process lower-half arena
- two preemptively scheduled C processes

## Milestone 3 — core OS
- handles/resources
- signals
- message ports
- shared memory
- wait sets
- `/dev`
- pageable driver/kernel-extension loading through upper 8 KiB working-set slots
- flat/far driver-state and sparse physical-I/O access primitives
- basic storage/VFS
- executable binder

## Milestone 4 — self-hosting console environment
- keyboard/text display
- TTY
- virtual consoles
- PTY
- shell
- pipes/redirection
- process/memory/module utilities
- debugger/monitor

## Milestone 5 — richer runtime/toolchain
- `.mex` extent format
- far pointers/function pointers
- dynamic libraries/services
- demand-loaded extents
- dependency-aware placement

## Milestone 6 — graphics
- surfaces
- display service
- compositor/window manager
- graphical terminal
- GUI libraries

## Milestone 7 — networking
- Ethernet driver
- IPv4/ICMP/UDP/TCP/DNS/DHCP
- sockets
- telnetd
- httpd
- sshd later

## Long term
- legacy compatibility personality
- more sophisticated protection support
- compiler-assisted DMA/far-memory optimization
- runtime profile-informed placement

## 1.x storage scope clarification
- generic block/media device interface
- SD storage driver after the bootstrap's minimal SD reader
- physical floppy/F011 driver
- FAT filesystem support
- CBM filesystem/DOS support including command-channel/status semantics
- common CBM filesystem implementation for real media and mounted `.d64`, `.d71`, and `.d81` images
- virtual disk-image media backends so images can be nested on ordinary filesystems such as FAT

These are 1.x deliverables and should not wait for legacy compatibility personalities.

## 2.x supervisor/hypervisor phase
After a solid 1.x system exists, investigate/implement a MEGA65-OS-specific `HICKUP.M65` supervisor. Scope includes protected syscall entry, stronger privilege enforcement, hardware fault handling, validated hardware page-fault-backed VM, and virtualization of hardware such as F011 for legacy environments. Keep normal drivers, filesystems, VFS, scheduler policy, and services outside the 16 KiB protected supervisor.
