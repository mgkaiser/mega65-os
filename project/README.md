# MEGA65 OS Project Backlog

This directory is the repository-native project plan imported from `MEGA65_OS_User_Stories.xlsx`.

The goal is to make the work discoverable to contributors without requiring access to the spreadsheet. Each work item preserves its stable `US-nnn` identifier, phase, epic, priority, current status, estimate, dependencies, design-chapter references, user story and acceptance criteria.

## Phase backlogs

- [0 - Hardware Validation](0-hardware-validation.md)
- [1 - Takeover](1-takeover.md)
- [2 - Kernel Nucleus](2-kernel-nucleus.md)
- [3 - Core OS](3-core-os.md)
- [4 - Console OS](4-console-os.md)
- [5 - Native Toolchain](5-native-toolchain.md)
- [6 - Graphics](6-graphics.md)
- [7 - Networking](7-networking.md)
- [Long Term](long-term.md)

## Supporting project data

- [Roadmap](roadmap.md)
- [Architecture decisions and research](architecture-decisions.md)
- [Documentation findings](documentation-findings.md)
- [ADR workflow](../docs/adr-workflow.md)
- [Canonical design documentation](../docs/README.md)

## Contributor workflow

Choose a work item whose status and dependencies make it actionable. Use its stable ID in branches, commits and pull requests, for example `US-010`. Read the referenced design chapters before implementation and treat the listed acceptance criteria as the minimum definition of done.

If implementation exposes a design ambiguity, do not silently invent an ABI or policy. Check the architecture-decision tracker and `docs/open-questions.md`; where necessary, resolve the corresponding decision/research story and update the canonical design docs.

A story marked **Done** records completed research/design work and should not be reimplemented unless a later ADR supersedes it. **Ready** means prerequisite research is sufficiently resolved to begin the stated validation/design work. **In Progress** reflects work already started in the repository. **Backlog** is not yet active.

## GitHub Issues / Projects

These Markdown files are the durable repository copy of the project data. GitHub Issues can use the same `US-nnn` IDs for assignable implementation work, while a GitHub Projects board can be a view over those issues. The stable IDs and this repository data remain the cross-reference back to the architecture and spreadsheet import.
