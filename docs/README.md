# MEGA65 OS — Working Design Notes

This directory is intentionally a **working pile of design notes**, not a polished specification.
The immediate goal is to preserve architectural decisions and open questions while the design is still fluid.

## Core idea

Build a new operating system for the MEGA65 that embraces the machine's unusual memory architecture instead of pretending it is a flat-memory 32-bit machine.

The system is primarily C with a very small assembly/bootstrap layer. It is preemptive, object-oriented at the kernel-resource level, supports protected processes and threads, far pointers, pageable/file-backed objects, dynamic OS components, and a console-first user environment.

## Current documents

- [architecture.md](architecture.md) — overall principles and layering
- [boot.md](boot.md) — hypervisor/bootstrap takeover
- [memory.md](memory.md) — memory hierarchy, paging, allocation
- [pointers-and-abi.md](pointers-and-abi.md) — near/far pointers and calls
- [processes-threads-scheduler.md](processes-threads-scheduler.md)
- [ipc.md](ipc.md)
- [resources-security-users.md](resources-security-users.md)
- [devices.md](devices.md)
- [console-tty-pty.md](console-tty-pty.md)
- [graphics.md](graphics.md)
- [networking.md](networking.md)
- [executable-format.md](executable-format.md)
- [linker-loader.md](linker-loader.md)
- [toolchain.md](toolchain.md)
- [roadmap.md](roadmap.md)
- [open-questions.md](open-questions.md)

These files deliberately overlap. Consolidation comes later.