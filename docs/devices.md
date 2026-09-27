# Device Model

## `/dev`

Use a Unix-ish OS-controlled device namespace as the primitive hardware layer.

Possible nodes:
- `/dev/vic`
- `/dev/dmagic`
- `/dev/audio0`
- `/dev/keyboard`
- `/dev/mouse`
- `/dev/serial0`
- `/dev/rtc`
- `/dev/fdc0`
- `/dev/fd0`
- `/dev/sd0`
- `/dev/sd0p1`

Opening a device may dynamically load its driver.

## Operations

Device handles should support applicable operations such as:
- open/close
- read/write
- seek
- ioctl
- map
- blocking/wakeup/readiness

## Driver execution and state

Normal drivers and kernel extensions execute through the upper-half kernel module window. Phase 1 maps one 8 KiB slab at $8000-$9FFF. Page 5 ($A000-$BFFF) is reserved so the execution window can later grow to 16 KiB for contiguous module extents sharing the MAPHI displacement.

Pages 6 and 7 remain untranslated. This deliberately preserves real kernel RAM at $C000-$CFFF, conventional near I/O at $D000-$DFFF, and the resident nucleus at $E000-$FFFF.

Persistent driver state need not live in the execution slab. It may reside in kernel-managed objects elsewhere in physical memory.

## Hardware register access

Normal applications do not know hardware register addresses.

During Phase 1, drivers can use the conventional near $D000 I/O aperture because the module window no longer overlaps it. This avoids depending on compiler support for the planned far-pointer/flat-I/O ABI.

The hardware's flat-memory mechanism remains useful for sparse far I/O and data, and assembly veneers can expose it before compiler lowering exists. Later drivers may choose near or flat I/O based on locality and measured cost.

Direct `$Dxxx` constants outside the architecture/device layer should be considered an architectural violation in native application code.

## Storage layering

    hardware
      -> controller device
      -> block driver
      -> raw block device
      -> filesystem driver
      -> mounted namespace

Keep raw devices separate from mounted filesystems.

## 1.x Floppy and Commodore Media

Physical floppy support belongs in 1.x and is implemented as a driver, not as kernel filesystem logic. The storage stack separates media access from filesystem semantics:

    physical floppy/F011 backend ─┐
    D64 image backend             ├─> CBM media interface -> CBM FS/DOS -> VFS
    D71 image backend             │
    D81 image backend             ┘

The CBM filesystem/DOS layer is shared by real media and image-backed virtual media. It understands Commodore directory/BAM/file semantics and exposes DOS command-channel/status behavior. A physical 1581 disk and a mounted `.d81` therefore use the same upper filesystem implementation while differing only in the media backend.

FAT is a separate filesystem module and is also a 1.x requirement, particularly for SD/removable-media interoperability. A CBM image stored on FAT may be opened as a file, wrapped by an image-media backend, and mounted through the CBM filesystem module.

## 2.x Hardware Virtualization

Legacy hardware virtualization is deferred to 2.x. A custom MEGA65 OS Hypervisor may trap virtualized F011 accesses and redirect legacy software to real or image-backed media. This is distinct from 1.x native floppy and CBM filesystem support.

## Bootstrap console module

The first external driver is the console module. It is preloaded by the transition loader, registered from the boot manifest, mapped at $8000-$9FFF by the resident nucleus, validated through a versioned module header, and invoked through module entry offsets. The same module-call path is intended for later demand-loaded drivers.

Bring-up uses conventional VIC-III/IV 80-column text: H640 is enabled, screen RAM is at `$0800`, the row stride is 80 bytes, and CRAM2K exposes 2 KiB of colour RAM. The initial console can initialise the mode, clear 80x25 text/colour cells, and write simple text. Scrolling, terminal semantics and richer character conversion are later work.
