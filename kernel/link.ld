/* MEGA65 OS resident-kernel linker layout.
 *
 * The permanently resident nucleus occupies exactly one 8 KiB logical extent:
 * $E000-$FFFF.  Page 7 includes the hardware vectors at $FFFA-$FFFF.
 *
 * Base Page is deliberately a logical ABI resource, not a globally reserved
 * physical page.  The active thread owns logical Page 0 ($0000-$1FFF), and its
 * mapping carries the compiler/runtime Base-Page state with it.
 */

MEMORY {
    /* LLVM-MOS imaginary registers are offsets in the active Base Page.
     * Keep offset zero available and reserve $02-$FF for compiler temporaries.
     * This region describes logical addresses only; it does not reserve a
     * permanently fixed physical zero page.
     */
    zp      (rw)  : ORIGIN = 0x0002, LENGTH = 0x00fe

    /* Leave the final six bytes for NMI, RESET and IRQ/BRK vectors. */
    ram     (rwx) : ORIGIN = 0xe000, LENGTH = 0x1ffa
    vectors (r)   : ORIGIN = 0xfffa, LENGTH = 0x0006
}

REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)

SECTIONS {
    /* startup.s places _kernel_start here so byte zero of kernel.bin is the
     * loader's handoff target at $E000.
     */
    .kernel_entry : {
        KEEP(*(.kernel_entry))
    } > ram

    INCLUDE c.ld

    /* The normal-mode vectors are invariant because Page 7 is resident. */
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
    /* Emit the whole resident Page-7 image, including the vector tail. */
    FULL(ram)
    FULL(vectors)
}
