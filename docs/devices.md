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

## Storage layering

    hardware
      -> controller device
      -> block driver
      -> raw block device
      -> filesystem driver
      -> mounted namespace

Keep raw devices separate from mounted filesystems.

## Hardware access

Normal applications do not know hardware register addresses.

Raw register/device mappings require capabilities. High-performance games/demos can request exclusive hardware access while the scheduler and OS remain alive.

Direct `$Dxxx` constants outside the architecture/device layer should be considered an architectural violation in native code.

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
