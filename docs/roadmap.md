# Rough Roadmap

This is deliberately provisional.

## Milestone 0 — verify hardware assumptions
- hypervisor loading/takeover facilities
- mapping granularity/capabilities
- stack and zero-page relocation details
- DMAgic behavior
- interrupt/vector behavior

## Milestone 1 — takeover
- launch transition loader
- use stock Hyppo to load raw resident kernel image at $4000
- abandon inherited environment
- enter assembly kernel entry, then C kernel
- enforce the resident nucleus as a single 8 KiB extent

## Milestone 2 — nucleus
- memory/object manager
- 8K page machinery
- timer IRQ
- scheduler
- threads with separate stack/zero page
- two preemptively scheduled C processes

## Milestone 3 — core OS
- handles/resources
- signals
- message ports
- shared memory
- wait sets
- `/dev`
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
