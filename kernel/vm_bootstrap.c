#include "vm_bootstrap.h"
static struct vm_extent extents[VM_BOOT_EXTENTS];
static uint8_t extent_count;

enum vm_boot_result vm_bootstrap_from_loader(const volatile struct boot_info *b)
{
    uint8_t i;
    if(b->magic!=BOOTINFO_MAGIC || b->version!=BOOTINFO_VERSION ||
       b->module_count>VM_BOOT_EXTENTS || b->size!=sizeof(struct boot_info))
        return VM_BOOT_BAD_MANIFEST;
    extent_count=0;
    for(i=0;i<b->module_count;i++){
        if(!b->modules[i].kind) continue;
        extents[extent_count].phys_addr=b->modules[i].phys_addr;
        extents[extent_count].size=b->modules[i].extent_size;
        extents[extent_count].owner=b->modules[i].kind;
        extents[extent_count].flags=b->modules[i].flags;
        extent_count++;
    }
    return VM_BOOT_OK;
}
const struct vm_extent *vm_boot_extent(uint8_t owner)
{
    uint8_t i;
    for(i=0;i<extent_count;i++) if(extents[i].owner==owner) return &extents[i];
    return 0;
}
