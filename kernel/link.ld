/* MEGA65 OS resident-kernel linker layout.
 *
 * The permanently resident nucleus occupies exactly one 8 KiB logical extent:
 * $E000-$FFFF. Page 7 includes the hardware vectors at $FFFA-$FFFF.
 *
 * LLVM-MOS' platform linker support owns the standard zero-page region and
 * imaginary-register definitions. Do not redefine "zp" here. Base Page
 * relocation is an OS/runtime concern and will be finalized with the process
 * ABI and interrupt-entry design.
 */

MEMORY {
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

ASSERT(ADDR(.kernel_entry) == 0xe000,
       "kernel entry must remain at $E000");
ASSERT(ADDR(.vectors) == 0xfffa,
       "kernel vectors must begin at $FFFA");
ASSERT(SIZEOF(.vectors) == 6,
       "kernel must provide exactly NMI, RESET and IRQ/BRK vectors");

OUTPUT_FORMAT {
    /* Emit the whole resident Page-7 image, including the vector tail. */
    FULL(ram)
    FULL(vectors)
}
