; Minimal MAP backend for the kernel extension window.
;
; void kmap_apply_upper(uint16_t offset_pages, uint8_t enable)
;
; This routine is intentionally private to mapper.c.  The stable C interface
; deals in semantic windows and physical addresses, not MAP encodings.
;
; LLVM-MOS C ABI details are compiler-owned; this veneer is kept small so the
; exact argument lowering can be validated during the first real toolchain
; build.  The implementation below is a placeholder until that ABI validation
; is complete and MUST NOT be treated as a tested hardware mapper.
;
; The eventual sequence is:
;   Y = low 8 bits of 12-bit upper-half offset
;   Z low nibble = high 4 bits of offset
;   Z high nibble = enable mask for Pages 4..7
;   A = X = 0 (lower-half mapping disabled)
;   MAP
;   EOM
;
; Keeping this hardware encoding behind one symbol is what allows the public
; mapper API to remain stable as the full 28-bit mapper is implemented.

        .text
        .globl _kmap_apply_upper

_kmap_apply_upper:
        ; TODO(US-001/US-002): implement after LLVM-MOS argument ABI is
        ; build-validated.  Do not guess where the C arguments arrive.
        rts
