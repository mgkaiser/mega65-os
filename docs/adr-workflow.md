# Architecture Decision and Open-Question Workflow

## Purpose

MEGA65 OS uses lightweight Architecture Decision Records (ADRs) to close design questions without losing the evidence and alternatives that led to a decision.

An open question is not considered closed merely because it was discussed. Closure requires:
1. the question or research story to identify the evidence used;
2. alternatives and consequences to be recorded;
3. the decision to be reflected in the canonical Git design documents;
4. affected implementation/user stories to be updated when the decision changes their scope or acceptance criteria.

## ADR template

```text
# ADR-NNN: Title

Status: Proposed | Accepted | Superseded | Deferred
Date:
Stories:
Affected docs:

## Question
What must be decided?

## Evidence
Hardware documentation, measurements, prototypes, implementation results, etc.

## Alternatives
The credible alternatives considered.

## Decision
The chosen architecture or policy.

## Consequences
Benefits, costs, constraints, follow-on work, and compatibility effects.

## Validation
What is already proven and what still requires measurement or implementation testing?
```

## Closure rules

Hardware facts may be closed from `mega65-book.pdf` when the acceptance criterion is documentary and the book explicitly establishes the fact. Stories whose acceptance criteria require measurement, execution on hardware/emulator, timing, or a working prototype remain open until that evidence exists.

OS-policy decisions may be closed when the decision is explicit in the canonical Git Markdown and all affected design documents are internally consistent. Implementation stories remain separate: deciding an architecture does not mean the implementation has passed its acceptance test.

## Current Chapter 27 question map

| Open question | Tracking story |
| --- | --- |
| Hypervisor API/bootstrap constraints | US-078 / US-005 |
| MAP granularity and edge cases | US-079 / US-001 |
| First-384 KiB performance assumptions | US-081 and future tier benchmark work |
| Stack/Base Page relocation constraints and cost | US-080 / US-002 |
| DMAgic setup/crossover cost | US-081 / US-003 |
| IRQ source/mask/acknowledgement behavior | US-082 / US-004 |
| 8 KiB mapping/paging policy | US-083 |
| Reserve/commit/first-touch semantics | US-084 |
| Swap format/location | US-085 |
| Pinning/deadlock rules | US-086 |
| Page replacement scoring | US-087 |
| 64-bit far-pointer layout | US-088 |
| Bounds/stale-pointer behavior | US-089 |
| Far dereference ABI | US-090 |
| Far-call/mapping-stack ABI | US-091 |
| Process/thread context and logical address layout | US-092 |
| Scheduler class/starvation semantics | US-093 |
| Capability representation/delegation | US-094 |
| Filesystem ownership/permissions | US-095 |
| Session/job-control semantics | US-096 |
| .mex header/extent layout | US-097 |
| .mex relocation/symbol rules | US-098 |
| .mex dependency/call-graph metadata | US-099 |
| BOOT_REQUIRED .mex subset | US-100 |
| Canonical execution windows/coexistence | US-101 |
| Extent-placement heuristic | US-102 |
| Message-port limits/handle transfer | US-103 |
| Wait-set semantics | US-104 |
| TTY line discipline | US-105 |
| PTY/job-control API | US-106 |
| Phase-1 compiler selection | US-107 / US-049 |
| Phase-1 calling convention | US-108 |
| Phase-2 compiler extensions | US-109 |
| TCP/IP implementation strategy | US-110 |
| IPv6 scope | US-111 |
| Crypto/random strategy | US-112 |
| Graphics surface rules | US-113 |
| Exclusive-display protocol | US-114 |

## Decisions already made outside unresolved research

The canonical design documents already record several accepted project decisions, including the 1.x/2.x Hypervisor boundary, physical floppy/FAT/CBM filesystem scope, the common CBM filesystem layer, the 8 KiB resident nucleus at `$E000-$FFFF`, Page 0 as the active-thread page, Page 6 as the pageable kernel-extension execution window, and the near/flat/MAP/DMA access hierarchy.

When one of these decisions needs a historical ADR, create it without reopening the decision unless new evidence requires reconsideration.
