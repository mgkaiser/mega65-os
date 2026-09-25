# Documentation Findings

Imported from the workbook's **Documentation Findings** sheet.

| Finding | Status | Architectural consequence | Source |
|---|---|---|---|
| CPU is 45GS02, not 65816 | Resolved | Compiler, ABI, context switch and low-level code must use 45GS02 semantics. | Official MEGA65 User Guide, App. G/H |
| True physical address space is 28-bit / up to 256 MiB | Resolved | Kernel physical addressing and DMA/flat-memory primitives can address the full space. | Official MEGA65 User Guide, App. G/L |
| MAP uses eight 8 KiB blocks | Resolved | 8 KiB is a natural MAP/code-window unit, but not necessarily universal pager allocation size. | Official MEGA65 User Guide, App. G |
| MAP state cannot be read back | Resolved | Kernel must keep authoritative software shadow mapping state. | Official MEGA65 User Guide, App. G |
| MAP sequence suppresses interrupts until EOM | Resolved | Mapping primitives must be centralized, short and always terminate correctly. | Official MEGA65 User Guide, App. H |
| 32-bit Base-Page indirect flat access reaches true 28-bit space | Resolved | Sparse far data can bypass MAP; far-pointer resolution can lower to hardware flat addressing. | Official MEGA65 User Guide, App. G/H |
| Base Page is movable; stack pointer is 16-bit | Resolved mechanism | Per-thread Base Page and stack are hardware-feasible; switching cost still needs benchmarking. | Official MEGA65 User Guide, App. H |
| Normal DMA jobs are synchronous; ~20 MiB/s copy, ~40 MiB/s fill headline rates | Partially resolved | No assumed CPU/DMA overlap; benchmark setup and tier-specific crossover thresholds. | Official MEGA65 User Guide, App. L |
| Hypervisor is a protected privileged mode with SYSCALL/trap entry | Resolved API; runtime validation pending | Stock Hyppo file-loading calls needed by the transition loader are documented and implemented; hardware/emulator execution is still an implementation acceptance test. | mega65-book.pdf, App. J; GitHub loader/loader.c and loader/hyppo.s |
| Native logical map is eight role-assigned 8 KiB pages | Decided | Page 0 active thread; Pages 1-5 process demand-paged working set; Page 6 kernel-extension execution window; Page 7 $E000-$FFFF resident nucleus. | GitHub docs/architecture.md and docs/memory.md |
| Page 6 is an execution window, not driver-state storage | Decided | Drivers/extensions execute through $C000-$DFFF while persistent state may remain out of map and be accessed with flat/far addressing. | GitHub docs/architecture.md, docs/memory.md, docs/devices.md |
| Sparse I/O should prefer flat/far access; MAP/near I/O is locality optimization | Decided; benchmark crossover pending | Avoid mapping I/O for isolated register accesses; temporarily expose near $D000 I/O only when repeated accesses amortize remapping cost. | GitHub docs/memory.md and docs/devices.md |
| ADR/open-question closure workflow | Resolved | ADR template and closure rules exist and Chapter 27 questions are mapped to research/decision stories. | GitHub docs/adr-workflow.md |
