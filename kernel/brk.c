#include "brk.h"

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
