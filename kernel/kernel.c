/* MEGA65 OS phase-1 kernel proper.
 *
 * Machine entry and interrupt veneers live in startup.s. C begins only after
 * _kernel_start has established the kernel execution environment.
 */
#include "brk.h"

__attribute__((noreturn))
void kmain(void)
{
    for (;;) {
        __asm__ volatile ("nop");
    }
}

void irq_dispatch(void)
{
    /* Phase 1: no IRQ sources enabled yet. */
}

void nmi_dispatch(void)
{
    /* Phase 1: catch NMI safely. Device-specific handling comes later. */
}
