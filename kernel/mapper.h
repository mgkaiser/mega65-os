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
    KMAP_WINDOW_UPPER0 = 0,   /* Page 4: $8000-$9FFF */
    KMAP_WINDOW_EXTENSION = KMAP_WINDOW_UPPER0 /* bootstrap compatibility name */
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

/* Overlay one 8 KiB physical page in an upper working-set slot.
 *
 * 8 KiB is the allocator/residency quantum. A process primary arena is four
 * contiguous 8 KiB pages and is mapped separately through MAPLO.
 *
 * phys_addr must be 8 KiB aligned. Phase 1 implements upper slot 0 only.
 * Future upper-slot APIs must preserve the MAPHI rule: all simultaneously
 * enabled upper slots share one displacement, so an overlay family must have
 * compatible contiguous physical placement.
 *
 * On success, *token identifies the previous complete MAP state.
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
