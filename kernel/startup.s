; MEGA65 OS kernel entry / vector veneers
; mega65-book.pdf is authoritative.
;
; Normal-mode vectors:
;   NMI   $FFFA/$FFFB
;   RESET $FFFC/$FFFD (Hyppo emulates reset-vector handoff when leaving
;                      Hypervisor mode; hardware reset itself starts at $8100
;                      in protected Hypervisor memory.)
;   IRQ/BRK $FFFE/$FFFF
;
; BRK and IRQ share the IRQ vector. irq_brk_entry inspects the stacked B flag
; and dispatches to separate C handlers.

        .text
        .globl _kernel_start
        .globl _irq_brk_entry
        .globl _nmi_entry
        .globl _kmain
        .globl _irq_dispatch
        .globl _brk_dispatch
        .globl _nmi_dispatch

_kernel_start:
        sei
        cld

        ; Phase-1 loader has already established native writable RAM.
        ; TODO: establish the final kernel Base Page, 16-bit stack and MAP here.
        ; TODO: zero .bss / initialise .data once linker symbols are frozen.
        ;
        ; The vector words themselves are emitted in .vectors below. The final
        ; kernel link must place that section at $FFFA in the kernel's normal
        ; CPU mapping.

        jsr _kmain

_kernel_returned:
        sei
        bra _kernel_returned

_irq_brk_entry:
        ; CPU has already stacked PC and P. Save the general registers before
        ; entering C. Exact full kernel context frame will be frozen with ABI.
        pha
        phx
        phy
        phz

        ; Before the four pushes above, stacked P was at SP+1. It is now SP+5.
        ; Use TSX and inspect the saved status B bit (bit 4).
        tsx
        lda $0105,x
        and #$10
        bne _dispatch_brk

        jsr _irq_dispatch
        bra _interrupt_return

_dispatch_brk:
        jsr _brk_dispatch

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
