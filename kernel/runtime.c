#include <stddef.h>

/*
 * Minimal C runtime required by the freestanding resident kernel.
 *
 * Design references:
 *   - docs/architecture.md § Native software rule
 *   - docs/toolchain.md (freestanding compiler/toolchain policy)
 *
 * The kernel is linked with -nostdlib on purpose.  We do not want a hosted C
 * runtime, startup code, or an accidental libc dependency inside the permanent
 * 8 KiB nucleus.  LLVM is nevertheless allowed to lower ordinary C operations
 * to a small set of well-known runtime primitives.  We provide those primitives
 * explicitly here as they become necessary.
 *
 * Keep this file boring.  These routines are compiler support, not OS policy.
 */

/*
 * memcpy
 * ------
 * LLVM may lower structure assignment/copy to memcpy even in freestanding
 * code.  mapper.c, for example, copies complete MAP shadow structures when
 * saving/restoring nested mappings.
 *
 * This bootstrap implementation favors correctness and tiny code over speed.
 * Once DMAgic and the memory subsystem are mature we can benchmark whether
 * larger copies deserve a specialized path.  Small compiler-generated copies
 * may still be faster here than paying DMA setup cost.
 */
void *memcpy(
    void *restrict destination,
    const void *restrict source,
    size_t count)
{
    unsigned char *dst = (unsigned char *)destination;
    const unsigned char *src = (const unsigned char *)source;
    size_t i;

    for (i = 0; i < count; ++i) {
        dst[i] = src[i];
    }

    return destination;
}
