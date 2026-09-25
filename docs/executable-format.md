# Native Executable / Module Container

Working extension: `.mex` (placeholder).

This is not merely a flat executable. It is an extent/object container consumed by both development tools and the runtime binder.

## Possible contents

    header
    extent directory
    imports
    exports
    relocations
    dependency metadata
    call graph / placement hints
    capability requirements
    payload extents

Example extent classes:
- CODE
- RODATA
- DATA
- BSS
- TLS
- ZEROPAGE
- resource/data objects

## Extents

An extent describes logical expectations and backing, not fixed physical RAM placement.

Metadata can include:
- type
- file offset/stored length
- virtual length
- alignment
- canonical logical execution/mapping window
- permissions
- shareability
- discardability
- backing policy
- residency hints
- dependencies
- boot requirements
- compression

Two code extents may deliberately be linked for the same logical CPU address and act as automatic overlays.

## Initialization/backing classes

Possible attributes:
- LOAD
- ZERO
- FILE_BACKED
- DISCARDABLE
- SHARED
- RESIDENT
- BOOT_REQUIRED
- DEVICE
- TLS
- ZEROPAGE
- STACK

## Container roles

Same basic format should be usable for:
- executable
- shared library
- driver
- filesystem module
- service
- kernel module
- boot image

Do not create unrelated formats unless there is a compelling reason.

## Linker metadata is runtime data

The format must preserve enough linker knowledge for the OS binder:
- imports/exports
- relocations
- near versus far relocation/call types
- dependencies/version requirements
- call graph/weights
- preferred logical windows
- coexistence information
- residency hints
- capability declarations

This is not disposable debug information. The OS actively uses it to decide how a program exists in memory.