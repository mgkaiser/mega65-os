# Open Questions / Things Not Yet Frozen

This list was reviewed against the official MEGA65 User Guide on
2026-09-24. Items marked **Resolved by documentation** are hardware
facts established by the guide. **Partially resolved** items have their
mechanism established but still need measurement or an OS design
decision.

## Hardware facts

-   **Partially resolved --- Hypervisor API usable by launched
    software.** Hypervisor mode, protected 16 KiB memory, SYSCALL entry
    at `$D640-$D67F`, trap state-save behavior and return mechanism are
    documented. The exact Hyppo file/bootstrap services usable by our
    transition loader still need verification.
-   **Resolved by documentation --- MAP granularity and basic
    constraints.** CPU view is eight 8 KiB blocks; lower/upper 32 KiB
    have separate offsets; offsets have 256-byte granularity; MAP state
    cannot be read back; mapping beyond 1 MiB requires the MEGA65
    extension and careful sequencing; `EOM` ends the
    interrupt-suppressed mapping sequence.
-   **Open --- exact first-384 KiB performance assumptions.** The guide
    does not establish our proposed hot-tier performance model.
-   **Partially resolved --- stack/Base Page relocation.** Movable
    256-byte Base Page and 16-bit stack pointer/high-byte control are
    documented. Context-switch costs still require measurement.
-   **Partially resolved --- DMAgic semantics/cost.** Normal jobs are
    triggered synchronously and the guide gives headline throughput
    (\~20 MiB/s copy, \~40 MiB/s fill at 40 MHz), but setup/crossover
    costs by memory tier still require measurement.
-   **Open --- IRQ source identification/acknowledgement behavior.**
    Device-specific IRQ handling still needs verification for the
    drivers we use.
-   **Resolved by documentation --- CPU identity.** Native CPU is
    45GS02, derived from CSG4510/65CE02; it is not a 65816.
-   **Resolved by documentation --- physical address width.** 45GS02
    exposes a 28-bit address space, up to 256 MiB.
-   **Resolved by documentation --- flat far-memory primitive.** 32-bit
    Base-Page indirect addressing accesses the true 28-bit space
    independently of the current 64 KiB map; documented penalty is two
    cycles for the extra pointer bytes.

## Memory

-   **Partially resolved --- 8 KiB quantum.** 8 KiB is definitely MAP
    block granularity, but the guide does not require the OS
    object/pager allocation quantum to be 8 KiB. Keep this as an OS
    design decision.
-   Open: exact near-heap logical layout.
-   Open: reserve versus commit API semantics.
-   Open: swap format/location.
-   Open: page replacement scoring.
-   Open: pinning rules and deadlock prevention.
-   **Newly clarified:** far-data policy must choose among flat-memory
    access, MAP+near access, and DMA rather than assuming MAP for sparse
    data.

## Far pointer / ABI

-   Open: exact 64-bit OS far-pointer bit layout.
-   Open: metadata in pointer versus object descriptor.
-   Open: bounds checking strategy.
-   Open: generation numbers/stale-pointer detection.
-   **Partially resolved --- far dereference implementation.** Hardware
    flat-memory addressing gives a strong sparse-access fast path after
    object resolution; MAP and DMA remain alternatives for dense/bulk
    access.
-   Open: far-call ABI and mapping-stack ABI.
-   **Resolved constraint:** ABI/compiler must target 45GS02 register,
    Base Page, stack, compound-instruction and addressing semantics; no
    65816 ABI assumptions.

## Processes/security

-   Open: exact capability representation.
-   Open: user/group/ACL model versus capability-first filesystem
    permissions.
-   Open: service namespace.
-   Open: session semantics.

## Executable format

-   Open: final name/extension (`.mex` provisional), extent directory
    layout, symbols/versioning, relocations, call-graph encoding,
    compression, signing/integrity, capability declarations, and boot
    subset.

## Linker

-   Open: canonical logical code windows, extent assignment rules,
    mapping-stack ABI, graph-coloring heuristic, runtime profile format.
-   **Clarified:** MAP is especially appropriate for code banking;
    official documentation recommends flat-memory access or DMA for most
    out-of-map data.

## IPC

-   Open: port/message limits, handle transfer, wait-set implementation,
    pipe buffering, RPC conventions.

## Console

-   Open: TTY line discipline, virtual-console representation, PTY API,
    job control.

## Networking

-   Open: TCP/IP implementation source versus custom stack, IPv6 timing,
    SSH/TLS crypto feasibility.

## GUI

-   Open: surface formats, compositor boundaries, exclusive-display
    transition protocol.

## Source

Official MEGA65 User Guide, especially Appendices G (45GS02), H
(instruction set/addressing modes) and L (DMA controller).

## Versioned decisions added 2026-09-24

- **Decided — custom Hypervisor belongs to 2.x.** MEGA65 OS 1.x uses stock Hyppo. A MEGA65-OS-specific `HICKUP.M65` supervisor is a planned 2.x architecture phase.
- **2.x validation item — hardware page faults.** The hardware documents a Hypervisor page-fault path, but production-core availability, page size/mapping semantics, fault conditions, resumability, and suitability for the OS pager must be validated before 2.x VM is designed around it.
- **Decided — physical floppy support belongs to 1.x.** It is a device-driver/media-backend feature, not kernel filesystem code.
- **Decided — FAT and CBM filesystem support belong to 1.x.**
- **Decided — common CBM filesystem/DOS implementation.** Real Commodore media and `.d64`/`.d71`/`.d81` image media use the same CBM filesystem/DOS layer; only the media backend differs.
- **Decided — CBM DOS command channel is part of the CBM storage abstraction.** The implementation must model command/status-channel semantics in addition to file/directory operations.
- **Deferred to 2.x — legacy F011 virtualization.** A custom Hypervisor may later trap legacy F011 accesses and redirect them to real or image-backed media; this is not required for native 1.x floppy access.
