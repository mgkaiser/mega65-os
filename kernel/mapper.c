#include "mapper.h"

/*
 * The 4510/45GS02 MAP instruction has one offset and one megabyte selector per
 * 32 KiB half, plus four enable bits selecting the 8 KiB pages in that half.
 * Hardware does not provide a normal-mode readback instruction, so the kernel
 * owns a software shadow from first takeover onward.
 *
 * Keep this representation private.  Drivers see only kmap_acquire/release.
 */
struct map_half {
    uint16_t offset_pages; /* signed modulo-20-bit offset / 256 */
    uint8_t enable;        /* low four bits: pages within this 32 KiB half */
    uint8_t megabyte;
};

struct map_state {
    struct map_half lo;
    struct map_half hi;
};

#define KMAP_STACK_DEPTH 8

static struct map_state current_map;
static struct map_state saved_map[KMAP_STACK_DEPTH];
static uint8_t map_depth;

/* Assembly veneer. It accepts the complete software representation so the C
 * API does not expose MAP register encoding.  Initial implementation uses the
 * C65-compatible first-megabyte form; >1 MiB support can be added here without
 * changing callers.
 */
extern void kmap_apply_upper(uint16_t offset_pages, uint8_t enable);

static void apply_state(const struct map_state *state)
{
    /*
     * Page 7 is deliberately left unmapped: the resident nucleus executes
     * from physical/logical $E000-$FFFF while Page 6 is changed underneath it.
     *
     * The current bootstrap mapper supports upper-half targets in the first
     * megabyte.  The public API already carries 28-bit physical addresses so
     * extending the assembly backend to the MEGA65 megabyte-select MAP
     * sequence is not an ABI change.
     */
    kmap_apply_upper(state->hi.offset_pages, state->hi.enable);
}

void kmap_init(void)
{
    current_map.lo.offset_pages = 0;
    current_map.lo.enable = 0;
    current_map.lo.megabyte = 0;
    current_map.hi.offset_pages = 0;
    current_map.hi.enable = 0;
    current_map.hi.megabyte = 0;
    map_depth = 0;
}

enum kmap_result kmap_acquire(enum kmap_window window,
                              uint32_t phys_addr,
                              kmap_token_t *token)
{
    uint32_t offset;

    if (token == 0)
        return KMAP_EINVAL;
    *token = KMAP_TOKEN_INVALID;

    if (window != KMAP_WINDOW_EXTENSION)
        return KMAP_ENOTSUP;
    if ((phys_addr & 0x1fffu) != 0)
        return KMAP_EALIGN;

    /* The backend is deliberately limited, not the interface. */
    if (phys_addr >= 0x100000u)
        return KMAP_ENOTSUP;
    if (map_depth >= KMAP_STACK_DEPTH)
        return KMAP_EDEPTH;

    saved_map[map_depth] = current_map;
    *token = map_depth++;

    /* MAP offsets are relative to the logical address.  For Page 6 the
     * logical base is $C000.  Arithmetic is modulo the first 1 MiB.
     */
    offset = (phys_addr - 0x0000c000u) & 0x000fffffu;
    current_map.hi.offset_pages = (uint16_t)(offset >> 8);
    current_map.hi.enable = 0x04; /* upper-half page index 2 == Page 6 */
    current_map.hi.megabyte = 0;

    apply_state(&current_map);
    return KMAP_OK;
}

enum kmap_result kmap_release(kmap_token_t token)
{
    if (map_depth == 0 || token != (kmap_token_t)(map_depth - 1))
        return KMAP_EORDER;

    current_map = saved_map[--map_depth];
    apply_state(&current_map);
    return KMAP_OK;
}
