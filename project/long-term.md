# Long Term

6 work items imported from `MEGA65_OS_User_Stories.xlsx`.

| ID | Epic | Priority | Status | Estimate | Dependencies | Chapters | Work item | Acceptance criteria |
|---|---|---|---|---|---|---|---|---|
| US-054 | Compiler | Could | Backlog | M | US-051, US-052, US-053 | 16 | As a project contributor, I want to decide whether a purpose-built compiler is justified so that the MEGA65 OS architecture can be implemented incrementally. | decision compares Phase-2 limitations against VM, DMA, extent and metadata requirements |
| US-074 | Network Services | Could | Backlog | XL | US-072, US-059 | 22 | As a project contributor, I want to implement SSH remote login so that the MEGA65 OS architecture can be implemented incrementally. | secure crypto/random/key/authentication and interoperable PTY sessions work |
| US-075 | Compatibility | Could | Backlog | XL | US-046, US-038 | 23 | As a project contributor, I want to implement a legacy Commodore/C65/MEGA65 personality so that the MEGA65 OS architecture can be implemented incrementally. | legacy mappings/ROM behavior are supplied without constraining native process ABI |
| US-076 | Performance | Could | Backlog | M | US-062, US-048 | 24 | As a project contributor, I want to collect runtime heat and cross-extent profiles so that the MEGA65 OS architecture can be implemented incrementally. | bounded profile data records calls, remaps and residency behavior for export |
| US-077 | Performance | Could | Backlog | L | US-076, US-045 | 24 | As a project contributor, I want to consume profiles for extent placement so that the MEGA65 OS architecture can be implemented incrementally. | profile-informed linking measurably reduces cross-window remaps on representative workloads |
| US-112 | Security Research | Could | Backlog | L | US-110 | 22 | As an OS architect, I want to select cryptographic and secure-random strategy for ssh/tls-class services so that implementation is not built on an unresolved architectural assumption. | Available entropy sources and crypto libraries/implementations are evaluated for correctness, performance, footprint and licensing; required primitives and exclusions documented. |
