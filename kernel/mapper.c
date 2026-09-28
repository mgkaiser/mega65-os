#include "mapper.h"

/*
 * MEGA65 OS MAP state manager.
 *
 * Design references:
 *   - docs/memory.md § Hardware mapping model
 *   - docs/memory.md § Allocation versus mapping
 *   - docs/architecture.md § MAP constraints
 *   - docs/architecture.md § Upper working set
 *
 * Hardware reference:
 *   - mega65-book.pdf: MAP instruction / memory mapping.
 *
 * IMPORTANT: MAP is state replacement, not a set of independent page-table
 * writes. Each 32 KiB half has one displacement shared by its four 8 KiB
 * selectors. Consequently the kernel keeps a complete software shadow of MAP
 * state and treats an 8 KiB page as the allocation/residency unit while
 * respecting the shared-displacement constraint when pages are visible.
 */

struct map_half {
    uint16_t offset_pages; /* Shared displacement for this 32 KiB half, /256. */
    uint8_t enable;        /* Four selector bits: one per logical 8 KiB page. */
    uint8_t megabyte;      /* Reserved for the >1 MiB MAP extension work. */
};

struct map_state {
    struct map_half lo;    /* Logical $0000-$7FFF / MAPLO. */
    struct map_half hi;    /* Logical $8000-$FFFF / MAPHI. */
};

#define KMAP_STACK_DEPTH 8

/* current_map is authoritative software state. MAP itself is not readable, so
 * kernel code must never infer the current mapping from hardware.
 */
static struct map_state current_map;

/* Mapping acquisitions are currently nestable but strictly LIFO. Saving the
 * complete state here makes release restore the caller's mapping exactly.
 */
static struct map_state saved_map[KMAP_STACK_DEPTH];
static uint8_t map_depth;

/*
 * Private C -> assembly handoff.
 *
 * We deliberately use globals instead of passing MAP operands through the C
 * ABI while compiler/ABI bring-up is still in progress. This keeps the public
 * API stable and makes the machine-specific boundary obvious.
 *
 * TODO: this bootstrap backend emits only MAPHI. Before a process MAPLO is
 * active, replace it with a backend that emits the COMPLETE lo+hi shadow on
 * every MAP. See docs/memory.md § Hardware mapping model.
 */
volatile uint8_t kmap_hw_y;
volatile uint8_t kmap_hw_z;
extern void kmap_apply_upper(void);

static void apply_state(const struct map_state *state)
{
    uint16_t o = state->hi.offset_pages;

    /* MAPHI encoding for the first-megabyte bootstrap case. The low eight
     * displacement bits go in Y; Z combines the four selector bits with the
     * high displacement nibble.
     */
    kmap_hw_y = (uint8_t)o;
    kmap_hw_z = (uint8_t)(((state->hi.enable & 0x0f) << 4) |
                          ((o >> 8) & 0x0f));
    kmap_apply_upper();
}

void kmap_init(void)
{
    /* The transition loader hands the kernel the native first-64K view with no
     * process mapping active. Mirror that known state before any acquisition.
     * See docs/boot.md § Handoff.
     */
    current_map.lo.offset_pages = 0;
    current_map.lo.enable = 0;
    current_map.lo.megabyte = 0;
    current_map.hi.offset_pages = 0;
    current_map.hi.enable = 0;
    current_map.hi.megabyte = 0;
    map_depth = 0;
}

enum kmap_result kmap_acquire(enum kmap_window window, uint32_t phys_addr,
                              kmap_token_t *token)
{
    uint32_t offset;

    if (!token) return KMAP_EINVAL;
    *token = KMAP_TOKEN_INVALID;

    /* Phase 1 intentionally exposes only logical Page 4 ($8000-$9FFF).
     * General upper-working-set placement comes after the full MAP shadow
     * backend. See docs/architecture.md § Upper working set.
     */
    if (window != KMAP_WINDOW_UPPER0) return KMAP_ENOTSUP;

    /* Physical backing is allocated/accounted in 8 KiB units. */
    if (phys_addr & 0x1fffu) return KMAP_EALIGN;

    /* The current encoder handles only the simple first-megabyte form. */
    if (phys_addr >= 0x100000u) return KMAP_ENOTSUP;
    if (map_depth >= KMAP_STACK_DEPTH) return KMAP_EDEPTH;

    saved_map[map_depth] = current_map;
    *token = map_depth++;

    /* MAP applies a displacement to the logical address. For slot 0 the
     * logical base is $8000, so calculate physical - logical and express it
     * in MAP's 256-byte displacement units.
     */
    offset = (phys_addr - 0x00008000u) & 0x000fffffu;
    current_map.hi.offset_pages = (uint16_t)(offset >> 8);

    /* Enable only Page 4. Future multi-slot overlays may assign arbitrary
     * roles to the slots, but all enabled MAPHI slots must share this same
     * displacement. See docs/memory.md § Allocation versus mapping.
     */
    current_map.hi.enable = 0x01;
    current_map.hi.megabyte = 0;

    apply_state(&current_map);
    return KMAP_OK;
}

enum kmap_result kmap_release(kmap_token_t token)
{
    /* Strict nesting prevents one subsystem from accidentally tearing down a
     * mapping established by another. The token denotes the immediately
     * preceding complete MAP state.
     */
    if (!map_depth || token != (uint8_t)(map_depth - 1))
        return KMAP_EORDER;

    current_map = saved_map[--map_depth];
    apply_state(&current_map);
    return KMAP_OK;
}
