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

Normal drivers and kernel extensions are not required to fit in the resident 8 KiB nucleus. Their executable extents are pageable and execute through the kernel's Page-6 window at $C000-$DFFF.

Page 6 is an execution window, not a requirement that all driver memory live in the logical 64 KiB map. Persistent driver state, buffers, descriptors, and other objects may remain elsewhere in physical memory and be reached with 45GS02 flat/far loads and stores.

This separation keeps the resident Page-7 nucleus small and makes clean driver code cheap to evict/remap.

## Hardware register access

Normal applications do not know hardware register addresses.

Drivers have two native register-access strategies:

- **Sparse I/O:** use 45GS02 flat/far loads and stores to physical I/O registers. Individual accesses are slower than near $D000 accesses, but no Page-6/I/O remapping round trip is required.
- **Dense I/O:** when many consecutive accesses justify the setup cost, temporarily expose the conventional $D000 I/O aperture and use near loads/stores, then restore the previous mapping.

The crossover should be measured. Drivers should not page/expose I/O for a single register access merely because near I/O is individually faster.

This also means a hardware driver can normally keep its entire $C000-$DFFF executable extent mapped while touching hardware sparsely through flat addressing.

Raw register/device mappings for applications require capabilities. High-performance games/demos can request exclusive hardware access while the scheduler and OS remain alive.

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

The first external driver is the console module. It is preloaded by the transition loader, registered from the boot manifest, mapped into Page 6 by the resident nucleus, validated through a versioned module header, and invoked through module entry offsets. The same module-call path is intended for later demand-loaded drivers.

Bring-up uses conventional VIC-III/IV 80-column text: H640 is enabled, screen RAM is at `$0800`, the row stride is 80 bytes, and CRAM2K exposes 2 KiB of colour RAM. The initial console can initialise the mode, clear 80x25 text/colour cells, and write simple text. Scrolling, terminal semantics and richer character conversion are later work.
