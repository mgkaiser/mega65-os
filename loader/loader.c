/* MEGA65 OS phase-1 transition loader.
 *
 * mega65-book.pdf is authoritative for the public Hyppo ABI.
 *
 * This loader deliberately does almost nothing: ask stock Hyppo to load the
 * kernel image, make the compatibility-ROM RAM writable, establish a native
 * RAM/IO mapping, and jump into the kernel.  Normal OS functionality belongs
 * in the kernel, not here.
 */
#include <stdint.h>

#define HYPPO_TRAP0 (*(volatile uint8_t *)0xd640u)
#define HYPPO_TRAP1 (*(volatile uint8_t *)0xd641u)

#define TRANSFER_PAGE 0x02u
#define TRANSFER_AREA ((volatile uint8_t *)0x0200u)

/*
 * kernel.prg is linked for KERNEL_ENTRY.  A PRG begins with its two-byte load
 * address, while Hyppo $00:$36 loads the complete file verbatim.  Loading the
 * file two bytes early therefore places the actual linked image at
 * KERNEL_ENTRY.
 */
#define KERNEL_ENTRY      0x4000u
#define KERNEL_FILE_LOAD  (KERNEL_ENTRY - 2u)

static void copy_kernel_filename(void)
{
    static const char name[] = "kernel.prg";
    uint8_t i;

    for (i = 0; i < sizeof(name); ++i)
        TRANSFER_AREA[i] = (uint8_t)name[i];
}

/* Hyppo calls are followed by NOP as required by the documented ABI. */
static void hyppo_setup_transfer_area(void)
{
    __asm__ volatile (
        "ldy #$02\n"
        "lda #$3a\n"
        "sta $d640\n"
        "nop\n"
        : : : "a", "y", "memory");
}

static void hyppo_set_kernel_filename(void)
{
    __asm__ volatile (
        "lda #$2e\n"
        "sta $d640\n"
        "nop\n"
        : : : "a", "memory");
}

static void hyppo_load_kernel(void)
{
    /*
     * Hyppo $00:$36 takes the 24-bit destination in X/Y/Z, low to high.
     * $003FFE is used so the two-byte PRG load-address prefix precedes the
     * linked image at $004000.
     */
    __asm__ volatile (
        "ldx #$fe\n"
        "ldy #$3f\n"
        "ldz #$00\n"
        "lda #$36\n"
        "sta $d640\n"
        "nop\n"
        : : : "a", "x", "y", "z", "memory");
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
static void start_kernel(void)
{
    ((void (*)(void))(uintptr_t)KERNEL_ENTRY)();

    /* A kernel entry point must never return to the transition loader. */
    for (;;)
        __asm__ volatile ("nop");
}

int main(void)
{
    copy_kernel_filename();

    /*
     * Keep the inherited mapping intact while using Hyppo: the transfer-area
     * call records the caller's current mapping.  Once the file is resident,
     * we no longer need the stock environment.
     */
    hyppo_setup_transfer_area();
    hyppo_set_kernel_filename();
    hyppo_load_kernel();

    disable_rom_write_protect();
    establish_native_ram_map();
    start_kernel();
}
