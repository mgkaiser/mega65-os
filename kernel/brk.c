#include "brk.h"

/*
 * HOW TO IMPLEMENT A BRK HANDLER
 * ------------------------------
 *
 * Native code invokes the BRK service ABI like this:
 *
 *     brk
 *     .byte $42          ; mandatory one-byte signature
 *     .byte $12, $34     ; zero or more signature-defined parameters
 *
 * The 45GS02 stacks the address immediately after the signature byte.  The
 * assembly veneer in startup.s records that address in brk_param_address, so
 * brk_dispatch() can recover:
 *
 *     call.signature     -> $42
 *     call.params[0]     -> $12
 *     call.params[1]     -> $34
 *
 * A handler is an ordinary C function with this shape:
 *
 *     static uint8_t my_handler(const struct brk_call *call)
 *     {
 *         switch (call->signature) {
 *         case 0x42:
 *             uint8_t first  = brk_param_u8(call, 0);
 *             uint8_t second = brk_param_u8(call, 1);
 *             ...do the kernel operation...
 *             return 2;     // two parameter bytes were consumed
 *
 *         case 0x43:
 *             ...do a parameterless kernel operation...
 *             return 0;     // BRK + signature only
 *         }
 *
 *         return 0;
 *     }
 *
 * Register it with:
 *
 *     brk_set_handler(my_handler);
 *
 * The return value is IMPORTANT: it is the number of bytes AFTER the signature
 * that belong to this call.  startup.s adds that value to the stacked return
 * PC before RTI.  Do not include the signature byte in the count; BRK has
 * already advanced past it.
 *
 * Some signatures are reserved for special-case resident-kernel calls.  Keep
 * those handlers tiny: the design target is for the permanently resident
 * kernel nucleus, including interrupt/BRK entry plumbing, to fit in one 8 KiB
 * extent.  Larger policy or service code should live outside the resident
 * nucleus and be reached through the normal kernel/service mechanisms.
 *
 * Parameter lengths are signature-defined.  A handler may therefore consume
 * zero bytes, one byte, or a larger fixed/encoded payload.  Validate any
 * variable-length encoding before reading beyond the bytes belonging to the
 * call.
 */

volatile uint16_t brk_param_address;

static brk_handler_t current_handler;

void brk_set_handler(brk_handler_t handler)
{
    current_handler = handler;
}

uint8_t brk_dispatch(void)
{
    const uint8_t *params = (const uint8_t *)(uintptr_t)brk_param_address;
    struct brk_call call;

    call.signature = params[-1];
    call.params = params;

    if (current_handler == 0)
        return 0;

    return current_handler(&call);
}
