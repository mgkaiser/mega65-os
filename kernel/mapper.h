#ifndef MEGA65_OS_MAPPER_H
#define MEGA65_OS_MAPPER_H

#include <stdint.h>

/*
 * Stable kernel mapping interface.
 *
 * Callers request a semantic window; they do not manipulate MAP register
 * encodings.  This lets the implementation grow into the full pager without
 * changing driver/extension call sites.
 */
enum kmap_window {
    KMAP_WINDOW_EXTENSION = 0, /* logical Pages 4-5: $8000-$BFFF; phase 1 maps Page 4 */
};

typedef uint8_t kmap_token_t;

#define KMAP_TOKEN_INVALID ((kmap_token_t)0xff)

enum kmap_result {
    KMAP_OK = 0,
    KMAP_EINVAL,
    KMAP_EALIGN,
    KMAP_ENOTSUP,
    KMAP_EDEPTH,
    KMAP_EORDER
};

/* Initialize the software MAP shadow to the native first-64K map established
 * by the transition loader.  Must be called before kmap_acquire().
 */
void kmap_init(void);

/* Map a physical extent into a semantic kernel execution window.
 *
 * phys_addr is a 28-bit MEGA65 physical address and must be 8 KiB aligned.
 * On success, *token identifies the previous mapping and must later be passed
 * to kmap_release().  Tokens are intentionally opaque to callers.
 *
 * The phase-1 implementation maps one 8 KiB module slab at $8000-$9FFF. Pages 5-7 remain real/untranslated, preserving kernel workspace, near I/O, and the resident nucleus. The full extension window may grow to 16 KiB ($8000-$BFFF) when contiguous module extents are used.  The interface
 * is already shaped for additional windows/backing objects later.
 */
enum kmap_result kmap_acquire(enum kmap_window window,
                              uint32_t phys_addr,
                              kmap_token_t *token);

/* Restore the mapping that was active before the matching acquire.
 * Phase 1 requires LIFO release; a future mapper may relax that internally
 * without changing this API.
 */
enum kmap_result kmap_release(kmap_token_t token);

#endif
