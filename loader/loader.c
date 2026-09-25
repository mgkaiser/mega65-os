/* MEGA65 OS phase-1 transition loader. */
#include <stdint.h>
#include "bootinfo.h"

#define KERNEL_ENTRY 0xe000u
#define KERNEL_PHYS  0x0000e000UL
#define CONSOLE_PHYS 0x00020000UL
#define EXTENT_SIZE  0x00002000UL

extern uint8_t load_kernel_image(void);
extern uint8_t load_console_image(void);

static void set_filename(const char *name)
{
    volatile uint8_t *p=(volatile uint8_t *)0x0200u;
    do { *p++=(uint8_t)*name; } while (*name++);
}

static void disable_rom_write_protect(void)
{
    __asm__ volatile ("lda #$02\nsta $d641\nnop\n" : : : "a","memory");
}

static void establish_native_ram_map(void)
{
    __asm__ volatile (
        "lda #$00\ntax\ntay\ntaz\nmap\n"
        "lda #$35\nsta $01\n"
        "lda #$47\nsta $d02f\nlda #$53\nsta $d02f\neom\n"
        : : : "a","x","y","z","memory");
}

static void build_boot_info(void)
{
    volatile struct boot_info *b=BOOTINFO;
    uint8_t i;
    b->magic=BOOTINFO_MAGIC;
    b->version=BOOTINFO_VERSION;
    b->module_count=2;
    b->size=(uint16_t)(sizeof(struct boot_info));
    for(i=0;i<BOOTINFO_MAX_MODULES;i++) {
        b->modules[i].kind=0; b->modules[i].flags=0; b->modules[i].reserved=0;
        b->modules[i].phys_addr=0; b->modules[i].extent_size=0;
    }
    b->modules[0].kind=BOOT_MODULE_KERNEL;
    b->modules[0].phys_addr=KERNEL_PHYS;
    b->modules[0].extent_size=EXTENT_SIZE;
    b->modules[1].kind=BOOT_MODULE_CONSOLE;
    b->modules[1].phys_addr=CONSOLE_PHYS;
    b->modules[1].extent_size=EXTENT_SIZE;
}

__attribute__((noreturn)) static void halt(void){for(;;)__asm__ volatile("nop");}
__attribute__((noreturn)) static void start_kernel(void)
{
    ((void(*)(void))(uintptr_t)KERNEL_ENTRY)(); halt();
}

int main(void)
{
    set_filename("kernel.bin");
    if(!load_kernel_image()) halt();

    set_filename("console.bin");
    if(!load_console_image()) halt();

    /* The transfer area is no longer needed; turn it into the versioned
     * loader->kernel manifest before Page 0 changes ownership.
     */
    build_boot_info();
    disable_rom_write_protect();
    establish_native_ram_map();
    start_kernel();
}
