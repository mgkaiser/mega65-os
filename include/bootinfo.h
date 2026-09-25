#ifndef MEGA65_OS_BOOTINFO_H
#define MEGA65_OS_BOOTINFO_H
#include <stdint.h>

#define BOOTINFO_ADDRESS 0x0200u
#define BOOTINFO_MAGIC 0x4d363542UL /* "M65B" little endian */
#define BOOTINFO_VERSION 1
#define BOOTINFO_MAX_MODULES 8

enum boot_module_kind {
    BOOT_MODULE_KERNEL = 1,
    BOOT_MODULE_CONSOLE = 2
};

struct boot_module {
    uint8_t kind;
    uint8_t flags;
    uint16_t reserved;
    uint32_t phys_addr;
    uint32_t extent_size;
};

struct boot_info {
    uint32_t magic;
    uint8_t version;
    uint8_t module_count;
    uint16_t size;
    struct boot_module modules[BOOTINFO_MAX_MODULES];
};

/* Loader owns this storage until handoff. Kernel copies/consumes it before
 * Page 0 is repurposed for a thread.
 */
#define BOOTINFO ((volatile struct boot_info *)(uintptr_t)BOOTINFO_ADDRESS)
#endif
