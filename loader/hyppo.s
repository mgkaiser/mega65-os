; Stock-Hyppo phase-1 file loading.
        .text
        .globl _load_kernel_image
        .globl _load_console_image

; Filename must already be NUL-terminated at transfer area $0200.
_prepare_name:
        ldy #$02
        lda #$3a
        sta $d640
        nop
        bcc _load_failed
        lda #$2e
        sta $d640
        nop
        bcc _load_failed
        rts

_load_kernel_image:
        jsr _prepare_name
        bcc _load_failed
        ; $00E000
        ldx #$00
        ldy #$e0
        ldz #$00
        bra _load

_load_console_image:
        jsr _prepare_name
        bcc _load_failed
        ; physical $020000, 8 KiB-aligned boot-module extent
        ldx #$00
        ldy #$00
        ldz #$02
_load:
        lda #$36
        sta $d640
        nop
        bcc _load_failed
        lda #$01
        rts
_load_failed:
        lda #$00
        rts
