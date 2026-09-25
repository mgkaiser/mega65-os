#include "mapper.h"

struct map_half {
    uint16_t offset_pages;
    uint8_t enable;
    uint8_t megabyte;
};
struct map_state { struct map_half lo, hi; };
#define KMAP_STACK_DEPTH 8

static struct map_state current_map;
static struct map_state saved_map[KMAP_STACK_DEPTH];
static uint8_t map_depth;

/* Private hardware handoff. Globals avoid baking an unverified C argument ABI
 * into mapper.s; the public mapper API remains ordinary C.
 */
volatile uint8_t kmap_hw_y;
volatile uint8_t kmap_hw_z;
extern void kmap_apply_upper(void);

static void apply_state(const struct map_state *state)
{
    uint16_t o = state->hi.offset_pages;
    kmap_hw_y = (uint8_t)o;
    kmap_hw_z = (uint8_t)(((state->hi.enable & 0x0f) << 4) |
                          ((o >> 8) & 0x0f));
    kmap_apply_upper();
}

void kmap_init(void)
{
    current_map.lo.offset_pages = 0; current_map.lo.enable = 0; current_map.lo.megabyte = 0;
    current_map.hi.offset_pages = 0; current_map.hi.enable = 0; current_map.hi.megabyte = 0;
    map_depth = 0;
}

enum kmap_result kmap_acquire(enum kmap_window window, uint32_t phys_addr,
                              kmap_token_t *token)
{
    uint32_t offset;
    if (!token) return KMAP_EINVAL;
    *token = KMAP_TOKEN_INVALID;
    if (window != KMAP_WINDOW_EXTENSION) return KMAP_ENOTSUP;
    if (phys_addr & 0x1fffu) return KMAP_EALIGN;
    if (phys_addr >= 0x100000u) return KMAP_ENOTSUP;
    if (map_depth >= KMAP_STACK_DEPTH) return KMAP_EDEPTH;

    saved_map[map_depth] = current_map;
    *token = map_depth++;

    offset = (phys_addr - 0x0000c000u) & 0x000fffffu;
    current_map.hi.offset_pages = (uint16_t)(offset >> 8);
    current_map.hi.enable = 0x04; /* Page 6 only */
    current_map.hi.megabyte = 0;
    apply_state(&current_map);
    return KMAP_OK;
}

enum kmap_result kmap_release(kmap_token_t token)
{
    if (!map_depth || token != (uint8_t)(map_depth - 1)) return KMAP_EORDER;
    current_map = saved_map[--map_depth];
    apply_state(&current_map);
    return KMAP_OK;
}
