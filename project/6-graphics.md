# 6 - Graphics

6 work items imported from `MEGA65_OS_User_Stories.xlsx`.

| ID | Epic | Priority | Status | Estimate | Dependencies | Chapters | Work item | Acceptance criteria |
|---|---|---|---|---|---|---|---|---|
| US-064 | Graphics | Must | Backlog | L | US-028, US-032 | 20 | As a project contributor, I want to implement shared-memory graphics surfaces so that the MEGA65 OS architecture can be implemented incrementally. | application maps RW and compositor receives appropriate shared handle |
| US-065 | Graphics | Should | Backlog | L | US-038, US-064 | 20 | As a project contributor, I want to implement direct fullscreen presentation so that the MEGA65 OS architecture can be implemented incrementally. | authorized surface presents without compositor and prior state restores afterward |
| US-066 | Graphics | Must | Backlog | XL | US-064, US-035 | 20 | As a project contributor, I want to implement compositor/window server so that the MEGA65 OS architecture can be implemented incrementally. | windows provide z-order, clipping, focus and updates without raw VIC access |
| US-067 | Graphics | Should | Backlog | L | US-059, US-066 | 18,20 | As a project contributor, I want to build a PTY-backed graphical terminal so that the MEGA65 OS architecture can be implemented incrementally. | terminal window drives PTY master and unmodified shell runs on slave |
| US-113 | Graphics Decision | Must | Backlog | L | US-096 | 20 | As an OS architect, I want to define native surface formats and ownership/synchronization rules so that implementation is not built on an unresolved architectural assumption. | Pixel formats, stride/alignment, palette semantics, shared-memory permissions, update synchronization and lifetime are specified. |
| US-114 | Graphics Decision | Should | Backlog | M | US-113 | 12,20 | As an OS architect, I want to define exclusive-display acquisition and restoration protocol so that implementation is not built on an unresolved architectural assumption. | Ownership arbitration, compositor suspension, VIC state save/restore, failure recovery and user-visible transition semantics are specified. |
