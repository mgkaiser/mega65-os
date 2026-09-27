# Processes, Threads, Scheduler, Interrupts

## Process

A process is a protection/resource/address-space container.

It owns/references:
- memory objects
- handles
- devices
- IPC endpoints
- timers
- capabilities
- threads

Creating a process automatically creates an initial/main thread.

## Thread

Scheduler schedules threads, not processes.

Each thread has CPU register/scheduler state and thread-private resources. The exact placement of per-thread user stack, Base Page and TLS inside a process's 32 KiB MAPLO extent remains an ABI design question; do not model them as independently MAP-able 8 KiB pages.

A running process normally owns a contiguous 32 KiB physical execution extent mapped at $0000-$7FFF by MAPLO. User Base Page/zero page and user stack are therefore part of that mapped lower half.

Kernel execution uses the real/untranslated lower half, including a real kernel Base Page and kernel stack. Interrupt/BRK entry must preserve the user frame pushed under the process mapping before MAPLO is disabled. The exact transition sequence remains to be implemented and validated.

Threads in one process share process-level resources and address-space objects.

## Preemption

Preemptive scheduling is fundamental.

Proposed classes:
- REALTIME
- EXCLUSIVE
- NORMAL
- BACKGROUND

An EXCLUSIVE game/demo gets nearly all CPU remaining after critical realtime work without rebooting out of the OS.

## Interrupt architecture

Keep true interrupt execution tiny.

Hardware IRQ path:

    IRQ
      -> save minimal context
      -> identify/ack/mask source
      -> wake associated service/driver thread
      -> scheduler
      -> selected thread
      -> RTI/restore

Substantive driver logic executes as scheduled thread code, not interrupt-context code.

Drivers register/claim IRQ resources rather than patching vectors.

## Signals

Support Unix-like process-control concepts where useful:
- TERM
- INT
- HUP
- ALRM
- CHLD
- user signals
- uncatchable KILL

Forced kill executes no target cleanup code. Kernel stops all threads and deterministically reclaims process-owned resources.

Normal exit and forced kill should converge on the same kernel resource-reclamation machinery after any graceful user cleanup opportunity.