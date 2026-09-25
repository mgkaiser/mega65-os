MEMORY {
    zp  (rw)  : ORIGIN = 0x0002, LENGTH = 0x00fe
    ram (rwx) : ORIGIN = 0xc000, LENGTH = 0x1000
}
REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)
SECTIONS {
    .module_header : { KEEP(*(.module_header)) } > ram
    INCLUDE c.ld
}
__rc0 = 0x0002;
INCLUDE imag-regs.ld
__module_image_size = . - 0xc000;
ASSERT(ADDR(.module_header) == 0xc000, "module header must be at $C000");
ASSERT(__module_image_size <= 0x1000, "console must fit below the $D000 I/O aperture");
OUTPUT_FORMAT { TRIM(ram) }
