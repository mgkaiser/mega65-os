; Stock-Hyppo calls used by the phase-1 transition loader.
; mega65-book.pdf is authoritative for the public ABI.
;
; The caller has already copied "kernel.bin\0" to $0200.
;
; $00:$3A setup_transfer_area: Y = page number ($02)
; $00:$2E setname: uses the configured transfer area
; $00:$36 loadfile: X/Y/Z = 24-bit destination, low/mid/high
;
; Every Hypervisor call is followed by NOP as required by the ABI.  Carry set
; means success; carry clear means failure.
;
; uint8_t load_kernel_image(void)
; Returns 1 in A on success, 0 in A on failure.

        .text
        .globl _load_kernel_image

_load_kernel_image:
        ldy #$02
        lda #$3a
        sta $d640
        nop
        bcc _load_failed

        lda #$2e
        sta $d640
        nop
        bcc _load_failed

        ; Load the raw resident image directly at its linked address $004000.
        ldx #$00
        ldy #$40
        ldz #$00
        lda #$36
        sta $d640
        nop
        bcc _load_failed

        lda #$01
        rts

_load_failed:
        lda #$00
        rts
