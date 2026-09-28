/* MEGA65 OS resident-kernel linker layout.
 *
 * This is a freestanding LLVM-MOS "common" target image, not a stock
 * mos-mega65 BASIC-loadable PRG. The permanently resident nucleus occupies
 * exactly one 8 KiB logical extent at $E000-$FFFF.
 */

MEMORY {
    zp      (rw)  : ORIGIN = 0x0002, LENGTH = 0x00fe
    ram     (rwx) : ORIGIN = 0xe000, LENGTH = 0x1ffa
    vectors (r)   : ORIGIN = 0xfffa, LENGTH = 0x0006
}

REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)

SECTIONS {
    .kernel_entry : {
        KEEP(*(.kernel_entry))
    } > ram

    INCLUDE c.ld

    .vectors : {
        KEEP(*(.vectors))
    } > vectors
}

__rc0 = 0x0002;
INCLUDE imag-regs.ld

ASSERT(ADDR(.kernel_entry) == 0xe000,
       "kernel entry must remain at $E000");
ASSERT(ADDR(.vectors) == 0xfffa,
       "kernel vectors must begin at $FFFA");
ASSERT(SIZEOF(.vectors) == 6,
       "kernel must provide exactly NMI, RESET and IRQ/BRK vectors");
ASSERT(__rc31 < 0x0100,
       "LLVM-MOS imaginary registers overflow Base Page");

OUTPUT_FORMAT {
    FULL(ram)
    FULL(vectors)
}
