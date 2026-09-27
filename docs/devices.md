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

Normal drivers and kernel extensions share the upper 24 KiB working set ($8000-$DFFF) with active-process data. Frequently used kernel components should preferentially occupy real memory beginning at $8000 so they are immediately visible whenever their MAPHI slot is untranslated. Less-common drivers and process data may overlay those 8 KiB slots.

All simultaneously MAPHI-enabled upper slots share one displacement, so an overlay set must occupy the corresponding positions of a contiguous physical run. This is a placement constraint; the pages in that run may serve different purposes.

Page 6 spans $C000-$DFFF. Overlaying it hides the conventional $D000 I/O aperture for the duration of that mapping. Drivers that require near I/O should execute with Page 6 untranslated or arrange an explicit transition; flat/far I/O remains the longer-term sparse-I/O path.

Persistent driver state need not live beside driver code and can reside in ordinary kernel-managed 8 KiB pages.

## Hardware register access

Normal applications do not know hardware register addresses.

During Phase 1, the bootstrap console occupies only Page 4 ($8000-$9FFF), so Page 6 remains untranslated and the conventional near $D000 I/O aperture stays visible. This avoids depending on compiler support for the planned far-pointer/flat-I/O ABI.

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
