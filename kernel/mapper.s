; Private first-megabyte MAP backend. Pages 5-7 remain unmapped: $A000-$CFFF real RAM, $D000-$DFFF near I/O, and $E000-$FFFF resident nucleus.
        .text
        .globl _kmap_apply_upper
        .globl _kmap_hw_y
        .globl _kmap_hw_z
_kmap_apply_upper:
        lda #$00
        ldx #$00
        ldy _kmap_hw_y
        ldz _kmap_hw_z
        map
        eom
        rts
