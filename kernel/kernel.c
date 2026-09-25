#include "brk.h"
#include "mapper.h"
#include "kmodule.h"
#include "vm_bootstrap.h"
#include "bootinfo.h"

static const char banner[] = "MEGA65 OS\nNative kernel online.\n";

__attribute__((noreturn))
void kmain(void)
{
    const struct vm_extent *console;
    if(vm_bootstrap_from_loader(BOOTINFO)!=VM_BOOT_OK)
        for(;;)__asm__ volatile("nop");

    kmap_init();
    console=vm_boot_extent(BOOT_MODULE_CONSOLE);
    if(!console) for(;;)__asm__ volatile("nop");

    if(kmodule_console_init(console->phys_addr)!=KMODULE_OK)
        for(;;)__asm__ volatile("nop");
    kmodule_console_write(console->phys_addr,banner);

    for(;;)__asm__ volatile("nop");
}
void irq_dispatch(void) {}
void nmi_dispatch(void) {}
