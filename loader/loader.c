/* MEGA65 OS phase-1 transition loader.
 * Initial skeleton. mega65-book.pdf is authoritative for hardware/Hyppo ABI.
 */
#include <stdint.h>

#define HTRAP01 (*(volatile uint8_t *)0xd641u)

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
        "eom\n");
}

static void disable_rom_write_protect(void)
{
    HTRAP01 = 0x02u;
    __asm__ volatile ("nop");
}

__attribute__((noreturn))
static void halt(void)
{
    for (;;)
        __asm__ volatile ("nop");
}

int main(void)
{
    establish_native_ram_map();
    disable_rom_write_protect();

    /* Next implementation step: use the documented Hyppo file-load ABI
       from mega65-book.pdf to load the kernel, then transfer control. */
    halt();
}
