# 7 - Networking

8 work items imported from `MEGA65_OS_User_Stories.xlsx`.

| ID | Epic | Priority | Status | Estimate | Dependencies | Chapters | Work item | Acceptance criteria |
|---|---|---|---|---|---|---|---|---|
| US-068 | Networking | Must | Backlog | L | US-035, US-022 | 21 | As a project contributor, I want to implement Ethernet /dev/net0 driver so that the MEGA65 OS architecture can be implemented incrementally. | frame send/receive works and IRQ processing occurs in scheduled context |
| US-069 | Networking | Must | Backlog | XL | US-068 | 21 | As a project contributor, I want to implement IPv4/ARP/ICMP/UDP/TCP so that the MEGA65 OS architecture can be implemented incrementally. | LAN tests demonstrate addressing, ping, UDP and reliable TCP streams |
| US-070 | Networking | Must | Backlog | L | US-069 | 21 | As a project contributor, I want to implement DHCP and DNS so that the MEGA65 OS architecture can be implemented incrementally. | lease/config acquisition and DNS resolution handle success, timeout and error cases |
| US-071 | Networking | Must | Backlog | XL | US-069, US-030 | 21 | As a project contributor, I want to implement BSD-like sockets with wait-set readiness so that the MEGA65 OS architecture can be implemented incrementally. | core socket calls work and readiness integrates with wait sets |
| US-072 | Network Services | Should | Backlog | L | US-059, US-071, US-034 | 22 | As a project contributor, I want to implement telnetd using PTY and sessions so that the MEGA65 OS architecture can be implemented incrementally. | incoming TCP login creates session/PTTY/shell and cleans up on disconnect |
| US-073 | Network Services | Could | Backlog | M | US-071, US-040 | 22 | As a project contributor, I want to implement a small static HTTP server so that the MEGA65 OS architecture can be implemented incrementally. | HTTP service reads files through VFS and serves bounded concurrent TCP requests |
| US-110 | Networking Decision | Must | Backlog | L | US-096 | 21 | As an OS architect, I want to select the initial tcp/ip implementation strategy so that implementation is not built on an unresolved architectural assumption. | Port/adapt/build-new options are compared for license, memory footprint, 8-bit suitability, socket integration and maintainability; decision and adaptation plan recorded. |
| US-111 | Networking Decision | Could | Backlog | S | US-110 | 21 | As an OS architect, I want to decide ipv6 timing and scope so that implementation is not built on an unresolved architectural assumption. | Cost/benefit and memory/code impact are assessed; initial release scope and later compatibility requirements are documented. |
