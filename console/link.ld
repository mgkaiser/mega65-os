MEMORY {
    zp  (rw)  : ORIGIN = 0x0002, LENGTH = 0x00fe
    ram (rwx) : ORIGIN = 0x8000, LENGTH = 0x2000
}
REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)
SECTIONS {
    .module_header : { KEEP(*(.module_header)) } > ram
    INCLUDE c.ld
}
__rc0 = 0x0002;
INCLUDE imag-regs.ld
__module_image_size = . - 0x8000;
ASSERT(ADDR(.module_header) == 0x8000, "module header must be at $8000");
ASSERT(__module_image_size <= 0x2000, "console must fit the 8 KiB upper working-set page");
OUTPUT_FORMAT { TRIM(ram) }
