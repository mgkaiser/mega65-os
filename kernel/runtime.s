; Minimal LLVM-MOS compiler-runtime assembly support.
;
; Design references:
;   docs/linker-loader.md § Fixed logical execution windows
;   docs/architecture.md § Native software rule
;
; LLVM-MOS reference:
;   llvm-mos-sdk mos-platform/common/crt/call-indir.S
;
; __call_indir
; ------------
; LLVM-MOS lowers an indirect C call through a function pointer to this helper.
; The target address is placed in imaginary registers __rc18/__rc19 (RS9).
;
; JSR __call_indir pushes the caller's return address.  We JMP to the actual
; target rather than JSRing it again.  The target's RTS therefore returns
; directly to the original caller, preserving the normal C call ABI and all
; argument/return-value locations.
;
; This is exactly the mechanism our bootstrap module calls need after resolving
; an exported entry offset to a C function pointer.  It is compiler ABI glue,
; not the future MEGA65 OS object trampoline itself.
        .text
        .globl __call_indir

__call_indir:
        jmp (__rc18)
