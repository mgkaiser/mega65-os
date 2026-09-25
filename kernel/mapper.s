; Private first-megabyte MAP backend. Page 7 remains unmapped/resident.
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
