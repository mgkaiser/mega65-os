#ifndef MEGA65_OS_BRK_H
#define MEGA65_OS_BRK_H

#include <stdint.h>

/*
 * Inline BRK format:
 *
 *     BRK
 *     .byte signature
 *     .byte parameter0, parameter1, ...
 *
 * The signature is always present.  Parameters are signature-defined and may
 * be zero length.  A handler returns the number of parameter bytes consumed;
 * the assembly veneer advances the interrupted PC by that amount before RTI.
 */
struct brk_call {
    uint8_t signature;
    const uint8_t *params;
};

typedef uint8_t (*brk_handler_t)(const struct brk_call *call);

extern volatile uint16_t brk_param_address;

void brk_set_handler(brk_handler_t handler);
uint8_t brk_dispatch(void);

static inline uint8_t brk_param_u8(const struct brk_call *call, uint8_t index)
{
    return call->params[index];
}

#endif
