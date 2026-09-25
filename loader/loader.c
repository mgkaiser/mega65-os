/* MEGA65 OS phase-1 transition loader.
 *
 * mega65-book.pdf is authoritative for the public Hyppo ABI.
 *
 * The loader is intentionally disposable.  It asks stock Hyppo to load the
 * raw resident-kernel image at $4000, removes compatibility-ROM write
 * protection, establishes our native RAM/IO mapping, and transfers control.
 */
#include <stdint.h>

#define KERNEL_ENTRY 0x4000u

/* Implemented in hyppo.s. Returns non-zero only if all three Hyppo calls
 * (setup transfer area, setname, loadfile) report success.
 */
extern uint8_t load_kernel_image(void);

static void copy_kernel_filename(void)
{
    static const char name[] = "kernel.bin";
    volatile uint8_t *const transfer_area = (volatile uint8_t *)0x0200u;
    uint8_t i;

    for (i = 0; i < sizeof(name); ++i)
        transfer_area[i] = (uint8_t)name[i];
}

static void disable_rom_write_protect(void)
{
    /* Hyppo $01:$02: disable compatibility-ROM write protection. */
    __asm__ volatile (
        "lda #$02\n"
        "sta $d641\n"
        "nop\n"
        : : : "a", "memory");
}

static void establish_native_ram_map(void)
{
    __asm__ volatile (
        "lda #$00\n"
        "tax\n"
        "tay\n"
        "taz\n"
        "map\n"
        "lda #$35\n"
        "sta $01\n"
        "lda #$47\n"
        "sta $d02f\n"
        "lda #$53\n"
        "sta $d02f\n"
        "eom\n"
        : : : "a", "x", "y", "z", "memory");
}

__attribute__((noreturn))
static void halt(void)
{
    for (;;)
        __asm__ volatile ("nop");
}

__attribute__((noreturn))
static void start_kernel(void)
{
    ((void (*)(void))(uintptr_t)KERNEL_ENTRY)();
    halt(); /* kernel entry must never return */
}

int main(void)
{
    copy_kernel_filename();

    /*
     * load_kernel_image() uses the inherited mapping while invoking Hyppo.
     * Once kernel.bin is resident, the stock environment is no longer needed.
     */
    if (!load_kernel_image())
        halt();

    disable_rom_write_protect();
    establish_native_ram_map();
    start_kernel();
}
