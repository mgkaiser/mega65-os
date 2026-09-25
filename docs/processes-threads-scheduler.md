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

Each thread has:
- CPU registers
- independently relocatable stack
- independently relocatable zero page
- scheduler state
- TLS
- thread-private resources

Threads in one process share the process address space and process-level resources.

Same-process context switches should often avoid changing most process mappings.

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