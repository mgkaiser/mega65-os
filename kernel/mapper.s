; MEGA65 OS bootstrap MAP backend.
;
; Design references:
;   docs/memory.md § Hardware mapping model
;   docs/architecture.md § MAP constraints
;   docs/architecture.md § Upper working set
;
; Hardware reference:
;   mega65-book.pdf: MAP instruction and EOM sequencing.
;
; This routine is intentionally TEMPORARY. It changes only the upper-half
; encoding supplied by mapper.c and explicitly supplies zero MAPLO operands.
; That is safe only while no process lower-half mapping is active. A MAP
; instruction replaces the complete selector/offset state; zero A/X does NOT
; mean "preserve MAPLO". Before process execution exists this routine must be
; replaced by a full-state emitter using the complete software MAP shadow.
;
; Pages 5-7 remain untranslated in this bootstrap state. This keeps ordinary
; near I/O at $D000-$DFFF visible and leaves the resident kernel at
; $E000-$FFFF untouched.
        .text
        .globl kmap_apply_upper
        .globl kmap_hw_y
        .globl kmap_hw_z

kmap_apply_upper:
        ; A/X encode the lower-half MAP state. Zero currently means no MAPLO
        ; selectors enabled -- it does NOT mean "leave lower mapping alone".
        lda #$00
        ldx #$00

        ; Y/Z contain the bootstrap MAPHI displacement and selector encoding
        ; prepared by mapper.c.
        ldy kmap_hw_y
        ldz kmap_hw_z

        ; MAP begins the mapping sequence; EOM terminates it. The MEGA65 book
        ; documents interrupt suppression across this sequence.
        map
        eom
        rts
