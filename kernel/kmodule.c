#include "kmodule.h"

/*
 * Bootstrap kernel-module call support.
 *
 * Design references:
 *   - docs/linker-loader.md § Fixed logical execution windows
 *   - docs/linker-loader.md § Demand loading
 *   - docs/architecture.md § Upper working set
 *   - docs/devices.md § Bootstrap console module
 *
 * This is the first, deliberately small version of the eventual loadable-object
 * trampoline/binder. A module lives at an arbitrary physical address, but its
 * executable image is linked for KMOD_WINDOW_BASE. To call it we:
 *
 *   1. acquire an upper working-set mapping,
 *   2. validate the module header through that mapping,
 *   3. convert the exported entry offset to a callable logical address,
 *   4. call the entry while the module remains mapped,
 *   5. restore the caller's previous mapping.
 */

static enum kmodule_result open_module(
    uint32_t phys,
    uint8_t kind,
    kmap_token_t *token,
    const struct kmod_header **header)
{
    enum kmap_result map_result;

    /* Temporarily expose the module at the fixed execution window. */
    map_result = kmap_acquire(
        KMAP_WINDOW_EXTENSION,
        phys,
        token);

    if (map_result != KMAP_OK) {
        return KMODULE_EMAP;
    }

    /* The module linker script places its header at the start of the image. */
    *header = (const struct kmod_header *)(uintptr_t)KMOD_WINDOW_BASE;

    /* Never interpret entry offsets from an unknown or incompatible image. */
    if ((*header)->magic != KMOD_MAGIC ||
        (*header)->abi_version != KMOD_ABI_VERSION) {

        kmap_release(*token);
        return KMODULE_EHEADER;
    }

    /* Prevent a console wrapper from accidentally calling another module kind. */
    if ((*header)->kind != kind) {
        kmap_release(*token);
        return KMODULE_EKIND;
    }

    return KMODULE_OK;
}

/*
 * Convert an exported module-relative entry offset into a logical address.
 * See docs/linker-loader.md § Fixed logical execution windows.
 */
static void *entry(
    const struct kmod_header *header,
    uint16_t offset)
{
    /* Kept in the interface because the eventual binder will use the owning
     * object when resolving and validating an export.
     */
    (void)header;

    return (void *)(uintptr_t)(KMOD_WINDOW_BASE + offset);
}

enum kmodule_result kmodule_console_init(uint32_t phys)
{
    kmap_token_t token;
    const struct kmod_header *header;
    enum kmodule_result result;
    kmod_init_fn init;

    result = open_module(
        phys,
        KMOD_KIND_CONSOLE,
        &token,
        &header);

    if (result != KMODULE_OK) {
        return result;
    }

    init = (kmod_init_fn)entry(
        header,
        header->entry_init);

    /* The module must stay mapped until its exported function returns. */
    init();

    kmap_release(token);

    return KMODULE_OK;
}

enum kmodule_result kmodule_console_clear(uint32_t phys)
{
    kmap_token_t token;
    const struct kmod_header *header;
    enum kmodule_result result;
    kmod_clear_fn clear;

    result = open_module(
        phys,
        KMOD_KIND_CONSOLE,
        &token,
        &header);

    if (result != KMODULE_OK) {
        return result;
    }

    clear = (kmod_clear_fn)entry(
        header,
        header->entry_clear);

    clear();

    kmap_release(token);

    return KMODULE_OK;
}

enum kmodule_result kmodule_console_write(
    uint32_t phys,
    const char *text)
{
    kmap_token_t token;
    const struct kmod_header *header;
    enum kmodule_result result;
    kmod_write_fn write;

    result = open_module(
        phys,
        KMOD_KIND_CONSOLE,
        &token,
        &header);

    if (result != KMODULE_OK) {
        return result;
    }

    write = (kmod_write_fn)entry(
        header,
        header->entry_write);

    /*
     * text currently points into the resident kernel image, so mapping the
     * console into the upper working-set window does not hide it. General
     * cross-object pointer semantics belong to the future object ABI.
     */
    write(text);

    kmap_release(token);

    return KMODULE_OK;
}
