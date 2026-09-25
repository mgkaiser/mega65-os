# 1 - Takeover

7 work items imported from `MEGA65_OS_User_Stories.xlsx`.

| ID | Epic | Priority | Status | Estimate | Dependencies | Chapters | Work item | Acceptance criteria |
|---|---|---|---|---|---|---|---|---|
| US-006 | Boot | Must | In Progress | S | US-005 | 4 | As a project contributor, I want to build the tiny transition loader so that the MEGA65 OS architecture can be implemented incrementally. | stock environment is abandoned after hypervisor bootstrap load and never regains control |
| US-007 | Boot | Must | In Progress | M | US-001, US-006 | 4 | As a project contributor, I want to establish MEGA65 OS vectors and mappings so that the MEGA65 OS architecture can be implemented incrementally. | no stock ROM/KERNAL vector or memory assumption is used after bootstrap entry |
| US-008 | Boot | Must | Backlog | M | US-006 | 4,14 | As a project contributor, I want to define and load a minimal boot container so that the MEGA65 OS architecture can be implemented incrementally. | bootstrap locates and loads manifest plus BOOT_REQUIRED payloads without a full filesystem |
| US-009 | Boot | Should | Backlog | S | US-008 | 4 | As a project contributor, I want to reclaim bootstrap memory so that the MEGA65 OS architecture can be implemented incrementally. | temporary bootstrap pages are returned after normal loader/pager activation |
| US-049 | Compiler | Must | In Progress | L | US-006 | 16 | As a project contributor, I want to select and use an unmodified full-source C compiler so that the MEGA65 OS architecture can be implemented incrementally. | initial C kernel builds with small assembly support; compiler can legally/practically be modified later |
| US-107 | Compiler Decision | Must | Done | L | US-079 | 16 | As an OS architect, I want to select the phase-1 full-source c compiler so that implementation is not built on an unresolved architectural assumption. | LLVM-MOS, vbcc, cc65 and credible alternatives are compared on license/source, 45GS02 path, ABI control, ZP control, optimizer/backend modifiability and object/linker integration; selection recorded. |
| US-108 | Compiler Decision | Must | Backlog | L | US-107 | 6,16,17 | As an OS architect, I want to freeze the phase-1 compiler/runtime calling convention so that implementation is not built on an unresolved architectural assumption. | Argument/return placement, register preservation, stack use, struct returns, variadics, interrupts and C/assembly interop are specified and validated with tests. |
