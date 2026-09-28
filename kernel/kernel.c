#include "brk.h"
#include "mapper.h"
#include "kmodule.h"
#include "vm_bootstrap.h"
#include "bootinfo.h"

/*
 * Phase-1 resident-kernel bring-up.
 *
 * Design references:
 *   - docs/boot.md § Handoff
 *   - docs/boot.md § Boot-critical modules
 *   - docs/architecture.md § Native logical address space
 *   - docs/devices.md § Bootstrap console module
 *
 * The loader has already placed the resident kernel at $E000 and the console
 * module in an 8 KiB physical extent. kmain consumes that handoff, adopts the
 * preloaded extents into kernel bookkeeping, maps the console into the upper
 * working set, and proves the module call path by printing a banner.
 *
 * This is deliberately tiny. Policy-heavy services and drivers do not belong
 * permanently in the 8 KiB resident nucleus.
 */

static const char banner[] = "MEGA65 OS\nNative kernel online.\n";

__attribute__((noreturn))
void kmain(void)
{
    const struct vm_extent *console;

    /* Copy loader-owned boot metadata before process MAPLO can replace the
     * lower 32 KiB containing BOOTINFO. See docs/boot.md § Handoff.
     */
    if (vm_bootstrap_from_loader(BOOTINFO) != VM_BOOT_OK)
        for (;;) __asm__ volatile("nop");

    /* Establish the software MAP shadow before any temporary mapping. */
    kmap_init();

    /* The console was preloaded by the transition loader and is therefore an
     * already-resident VM extent, not a special permanent mapping.
     */
    console = vm_boot_extent(BOOT_MODULE_CONSOLE);
    if (!console)
        for (;;) __asm__ volatile("nop");

    /* Map, validate, initialize, call, and unmap the console through the same
     * module mechanism later demand-loaded kernel modules will use.
     */
    if (kmodule_console_init(console->phys_addr) != KMODULE_OK)
        for (;;) __asm__ volatile("nop");

    kmodule_console_write(console->phys_addr, banner);

    /* Scheduler/idle-loop work comes later. For bring-up, remain resident. */
    for (;;) __asm__ volatile("nop");
}

/* Interrupt policy is intentionally empty during bootstrap. startup.s owns the
 * minimum vector veneer; real dispatch belongs to the scheduler/device work.
 */
void irq_dispatch(void) {}
void nmi_dispatch(void) {}
