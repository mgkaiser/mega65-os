; MEGA65 OS kernel entry / vector veneers
; mega65-book.pdf is authoritative.
;
; BRK ABI:
;   BRK
;   .byte signature
;   .byte parameter0, parameter1, ...
;
; The CPU stacks PC = address after the signature byte.  The BRK veneer copies
; that PC into _brk_param_address.  C therefore sees signature at params[-1]
; and zero or more inline parameter bytes at params[0...].
;
; brk_dispatch() returns the number of parameter bytes consumed in A.  Before
; RTI, the veneer advances the stacked PC by that count, so execution resumes
; after the inline BRK payload.
;
; Phase-1 note: this veneer currently assumes the hardware stack is page $01.
; Kernel startup will make that invariant explicit before interrupts/BRK are
; enabled.  This must be revisited when the final 45GS02 stack ABI is frozen.

        .section .kernel_entry,"ax"
        .globl _kernel_start
        .globl _irq_brk_entry
        .globl _nmi_entry
        .globl _kmain
        .globl _irq_dispatch
        .globl _brk_dispatch
        .globl _nmi_dispatch
        .globl _brk_param_address

_kernel_start:
        sei
        cld

        ; TODO: establish the final kernel Base Page, hardware/soft stacks and MAP.
        ; TODO: zero .bss / initialise .data once linker symbols are frozen.

        jsr _kmain

_kernel_returned:
        sei
        bra _kernel_returned

_irq_brk_entry:
        pha
        phx
        phy
        phz

        ; After four register pushes:
        ;   SP+5 = stacked P
        ;   SP+6 = stacked PC low
        ;   SP+7 = stacked PC high
        tsx
        lda $0105,x
        and #$10
        bne _dispatch_brk

        jsr _irq_dispatch
        bra _interrupt_return

_dispatch_brk:
        ; BRK stacks PC two bytes after the opcode.  Since byte BRK+1 is our
        ; mandatory signature, the stacked PC is exactly the first parameter.
        lda $0106,x
        sta _brk_param_address
        lda $0107,x
        sta _brk_param_address+1

        ; C reads the signature at brk_param_address[-1], can inspect as many
        ; parameter bytes as its signature requires, and returns the number of
        ; parameter bytes consumed as uint8_t in A (LLVM-MOS ABI).
        jsr _brk_dispatch

        ; Skip the consumed inline parameter bytes before RTI.  The signature
        ; has already been skipped by the BRK instruction itself.
        clc
        adc $0106,x
        sta $0106,x
        bcc _interrupt_return
        inc $0107,x

_interrupt_return:
        plz
        ply
        plx
        pla
        rti

_nmi_entry:
        pha
        phx
        phy
        phz
        jsr _nmi_dispatch
        plz
        ply
        plx
        pla
        rti

        .section .vectors,"a"
        .word _nmi_entry
        .word _kernel_start
        .word _irq_brk_entry
