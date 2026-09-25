# Architecture Decisions and Research

Imported from the workbook's **Architecture Decisions** sheet. The user-story phase files in this directory carry the implementation-facing work items; this file preserves the research/decision resolution and evidence columns.

| ID | Area | Phase | Question / decision | Priority | Depends on | Chapters | Status | Resolution | Evidence / remaining work |
|---|---|---|---|---|---|---|---|---|---|
| US-078 | Hardware | 0 - Hardware Validation | verify the exact hypervisor api and bootstrap constraints | Must |  | 2,4 | In Progress | Partially resolved; implementation exists | mega65-book documents transfer-area, setname/loadfile and ROM-write-protection calls; transition-loader code now implements the Hyppo load path. Hardware/emulator execution and failure-path validation remain. |
| US-079 | Hardware | 0 - Hardware Validation | determine the exact cpu mapping granularity and edge cases | Must |  | 2,5 | Done | Resolved by documentation | mega65-book establishes eight 8 KiB MAP blocks, separate lower/upper offsets, 256-byte offset granularity, MAP/EOM sequencing and related constraints. |
| US-080 | Hardware | 0 - Hardware Validation | measure stack and zero-page relocation cost and constraints | Must |  | 2,7,8 | Ready | Mechanism resolved; measurement remains | mega65-book documents movable Base Page and 16-bit stack facilities. The OS now has a Page-0 active-thread design, but repeated context-switch measurement is still required by acceptance criteria. |
| US-081 | Hardware | 0 - Hardware Validation | characterize dmagic setup and transfer costs across memory tiers | Must |  | 2,5,24 | Ready | Mechanism and headline performance resolved; benchmark remains | mega65-book documents full-space DMA access, synchronous normal jobs, about 20 MiB/s copy and 40 MiB/s fill. Tier/size setup and crossover benchmarks remain. |
| US-082 | Hardware | 0 - Hardware Validation | verify interrupt-source, masking, acknowledgement and nesting behavior | Must |  | 2,8 | Ready | Partially resolved by documentation | mega65-book documents interrupt flags/masks and device-specific acknowledgement behavior (including VIC $D019 behavior and register-cleared events). Representative OS timer/device IRQ validation and nesting tests remain. |
| US-083 | Memory Decision | 2 - Kernel Nucleus | confirm 8 kib as the kernel page and mapping quantum | Must | US-079 | 5,6,16 | Ready | Partially resolved | 8 KiB is definitively MAP block granularity, but the guide does not require it as the universal OS allocation/paging quantum. Retain as an OS policy decision. |
| US-084 | Memory Decision | 2 - Kernel Nucleus | define reserve, commit and first-touch semantics for memory objects | Must | US-084 | 5 | Backlog | | |
| US-085 | Memory Decision | 2 - Kernel Nucleus | define swap backing and on-storage format | Should | US-085 | 5 | Backlog | | |
| US-086 | Memory Decision | 2 - Kernel Nucleus | define pinning rules and pager deadlock prevention | Must | US-084 | 5,15 | Backlog | | |
| US-087 | Memory Decision | 2 - Kernel Nucleus | define the initial page-replacement scoring model | Should | US-084 | 5,24 | Backlog | | |
| US-088 | ABI Decision | 2 - Kernel Nucleus | select the 64-bit far-pointer bit layout | Must | US-084 | 6 | Backlog | | |
| US-089 | ABI Decision | 2 - Kernel Nucleus | define far-pointer bounds and stale-pointer behavior | Must | US-088 | 6 | Backlog | | |
| US-090 | ABI Decision | 2 - Kernel Nucleus | define the far dereference map/pin/unmap abi | Must | US-088, US-089 | 6,16,17 | Ready | Partially resolved | 45GS02 32-bit Base-Page indirect flat-memory addressing provides a direct sparse far-data path after object resolution. Exact OS map/pin/unmap ABI remains to be designed. |
| US-091 | ABI Decision | 5 - Native Toolchain | define the far-call and mapping-stack abi | Must | US-088 | 6,15,16 | Backlog | | |
| US-092 | Process Decision | 2 - Kernel Nucleus | freeze process/thread context and initial address-space layout | Must | US-079, US-080 | 6,7,8 | Ready | Partially resolved | Hardware confirms movable Base Page and 16-bit stack pointer; process/thread logical layout remains an OS design choice. |
| US-093 | Scheduler Decision | 2 - Kernel Nucleus | define scheduling-class semantics and starvation rules | Should | US-081 | 8 | Backlog | | |
| US-094 | Security Decision | 3 - Core OS | choose capability representation and delegation rules | Must | US-090 | 9,11 | Backlog | | |
| US-095 | Security Decision | 3 - Core OS | define filesystem ownership and permission semantics | Should | US-096 | 11,13 | Backlog | | |
| US-096 | Session Decision | 3 - Core OS | define sessions, login and process-group/job-control relationships | Should | US-097 | 7,11,18,19 | Backlog | | |
| US-097 | Executable Decision | 3 - Core OS | freeze the .mex container header and extent-directory binary layout | Must | US-042 | 14 | Backlog | | |
| US-098 | Executable Decision | 5 - Native Toolchain | define .mex relocation types and symbol binding rules | Must | US-098 | 14,15 | Backlog | | |
| US-099 | Executable Decision | 5 - Native Toolchain | define .mex dependency, call-graph and placement metadata encoding | Should | US-098 | 14,15,16 | Backlog | | |
| US-100 | Executable Decision | 3 - Core OS | define the boot_required .mex subset | Must | US-098 | 4,14 | Backlog | | |
| US-101 | Linker Decision | 5 - Native Toolchain | select canonical logical execution windows and coexistence rules | Must | US-091, US-098 | 6,15 | Backlog | | |
| US-102 | Linker Decision | 5 - Native Toolchain | define initial weighted-graph extent placement heuristic | Should | US-101 | 15,24 | Backlog | | |
| US-103 | IPC Decision | 3 - Core OS | define message-port payload and handle-transfer limits | Must | US-090 | 10 | Backlog | | |
| US-104 | IPC Decision | 3 - Core OS | define wait-set readiness and wake semantics | Must | US-090 | 9,10 | Backlog | | |
| US-105 | TTY Decision | 4 - Console OS | define tty line discipline and control-character semantics | Must | US-093, US-095 | 18 | Backlog | | |
| US-106 | TTY Decision | 4 - Console OS | define pty and job-control apis | Must | US-105 | 18,19 | Backlog | | |
| US-107 | Compiler Decision | 1 - Takeover | select the phase-1 full-source c compiler | Must | US-079 | 16 | Done | Decided | Phase-1 reference compiler is LLVM-MOS/Clang targeting 45GS02 with -mcpu=mos45gs02. Current kernel Makefile uses that target; later compiler work may add OS-specific ABI/address-space optimizations. |
| US-108 | Compiler Decision | 1 - Takeover | freeze the phase-1 compiler/runtime calling convention | Must | US-107 | 6,16,17 | Backlog | | |
| US-109 | Compiler Decision | 5 - Native Toolchain | define phase-2 compiler extension syntax and semantics | Should | US-088, US-107 | 16 | Backlog | | |
| US-110 | Networking Decision | 7 - Networking | select the initial tcp/ip implementation strategy | Must | US-096 | 21 | Backlog | | |
| US-111 | Networking Decision | 7 - Networking | decide ipv6 timing and scope | Could | US-110 | 21 | Backlog | | |
| US-112 | Security Research | Long Term | select cryptographic and secure-random strategy for ssh/tls-class services | Could | US-110 | 22 | Backlog | | |
| US-113 | Graphics Decision | 6 - Graphics | define native surface formats and ownership/synchronization rules | Must | US-096 | 20 | Backlog | | |
| US-114 | Graphics Decision | 6 - Graphics | define exclusive-display acquisition and restoration protocol | Should | US-113 | 12,20 | Backlog | | |
| US-115 | Architecture Governance | 0 - Hardware Validation | establish adr format and open-question closure workflow | Must |  | 27 | Done | Workflow established | docs/adr-workflow.md defines the ADR template, evidence/closure rules, and maps Chapter 27 open questions to research/decision stories. Closure requires canonical design-document updates. |
| DOC-001 | Hardware | 0 - Hardware Validation | Confirm native CPU identity | Must |  | 2 | Done | Resolved by documentation | Native CPU is 45GS02 derived from CSG4510/65CE02; not 65816. |
| DOC-002 | Hardware | 0 - Hardware Validation | Confirm physical address width | Must |  | 2,5 | Done | Resolved by documentation | 45GS02 true address space is 28-bit, supporting up to 256 MiB. |
| DOC-003 | ABI Decision | 2 - Kernel Nucleus | Confirm hardware flat-memory primitive | Must |  | 5,6,16 | Done | Resolved by documentation | 32-bit Base-Page indirect addressing accesses the true 28-bit space independently of current banking; documented extra pointer-fetch penalty is two cycles. |
| DOC-004 | Memory Decision | 2 - Kernel Nucleus | Choose far-data mechanism using documented hardware primitives | Must | US-079, US-081 | 5,6,16 | Ready | Architecture clarified | Default candidates are flat-memory for sparse access, MAP+near for hot compact regions/code, DMA for bulk movement. Thresholds still require measurement. |
| DOC-005 | Scheduler Decision | 2 - Kernel Nucleus | Account for MAP/EOM interrupt semantics | Must | US-079 | 8 | Ready | Resolved hardware constraint | MAP mapping sequences suppress interrupts until EOM; kernel mapping primitive must centralize and bound this critical sequence. |
