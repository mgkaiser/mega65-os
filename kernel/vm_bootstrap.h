#ifndef MEGA65_OS_VM_BOOTSTRAP_H
#define MEGA65_OS_VM_BOOTSTRAP_H
#include <stdint.h>
#include "bootinfo.h"

#define VM_BOOT_EXTENTS BOOTINFO_MAX_MODULES
struct vm_extent {
    uint32_t phys_addr;
    uint32_t size;
    uint8_t owner;
    uint8_t flags;
};
enum vm_boot_result { VM_BOOT_OK=0, VM_BOOT_BAD_MANIFEST, VM_BOOT_FULL };
enum vm_boot_result vm_bootstrap_from_loader(const volatile struct boot_info *info);
const struct vm_extent *vm_boot_extent(uint8_t owner);
#endif
