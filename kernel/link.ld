/* MEGA65 OS resident-kernel linker layout.
 *
 * The permanently resident nucleus occupies exactly one 8 KiB logical extent:
 * $4000-$5FFF.  Keep this file small and boring; pageable services do not
 * belong in this image.
 */

MEMORY {
    zp  (rw)  : ORIGIN = 0x0002, LENGTH = 0x00fe
    ram (rwx) : ORIGIN = 0x4000, LENGTH = 0x2000
}

REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)

SECTIONS {
    /* startup.s places _kernel_start here so byte zero of kernel.bin is the
     * loader's handoff target at $4000.
     */
    .kernel_entry : {
        KEEP(*(.kernel_entry))
    } > ram

    INCLUDE c.ld
}

__rc0 = 0x0002;
INCLUDE imag-regs.ld

ASSERT(ADDR(.kernel_entry) == 0x4000,
       "kernel entry must remain at $4000");
ASSERT(__rc31 < 0x0100,
       "LLVM-MOS imaginary registers overflow Base Page");

OUTPUT_FORMAT {
    TRIM(ram)
}
