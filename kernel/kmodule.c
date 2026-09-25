#include "kmodule.h"

static enum kmodule_result open_module(uint32_t phys,uint8_t kind,kmap_token_t *t,
                                      const struct kmod_header **h)
{
    if(kmap_acquire(KMAP_WINDOW_EXTENSION,phys,t)!=KMAP_OK) return KMODULE_EMAP;
    *h=(const struct kmod_header *)(uintptr_t)KMOD_WINDOW_BASE;
    if((*h)->magic!=KMOD_MAGIC || (*h)->abi_version!=KMOD_ABI_VERSION){
        kmap_release(*t); return KMODULE_EHEADER;
    }
    if((*h)->kind!=kind){kmap_release(*t);return KMODULE_EKIND;}
    return KMODULE_OK;
}
static void *entry(const struct kmod_header *h,uint16_t off)
{
    (void)h; return (void *)(uintptr_t)(KMOD_WINDOW_BASE+off);
}
enum kmodule_result kmodule_console_init(uint32_t phys)
{
    kmap_token_t t; const struct kmod_header *h; enum kmodule_result r=open_module(phys,KMOD_KIND_CONSOLE,&t,&h);
    if(r!=KMODULE_OK)return r; ((kmod_init_fn)entry(h,h->entry_init))(); kmap_release(t); return KMODULE_OK;
}
enum kmodule_result kmodule_console_clear(uint32_t phys)
{
    kmap_token_t t; const struct kmod_header *h; enum kmodule_result r=open_module(phys,KMOD_KIND_CONSOLE,&t,&h);
    if(r!=KMODULE_OK)return r; ((kmod_clear_fn)entry(h,h->entry_clear))(); kmap_release(t); return KMODULE_OK;
}
enum kmodule_result kmodule_console_write(uint32_t phys,const char *text)
{
    kmap_token_t t; const struct kmod_header *h; enum kmodule_result r=open_module(phys,KMOD_KIND_CONSOLE,&t,&h);
    if(r!=KMODULE_OK)return r; ((kmod_write_fn)entry(h,h->entry_write))(text); kmap_release(t); return KMODULE_OK;
}
