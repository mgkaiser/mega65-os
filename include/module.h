#ifndef MEGA65_OS_MODULE_H
#define MEGA65_OS_MODULE_H
#include <stdint.h>

#define KMOD_MAGIC 0x4d36u
#define KMOD_ABI_VERSION 1
#define KMOD_WINDOW_BASE 0xc000u

enum kmod_kind { KMOD_KIND_CONSOLE = 1 };

struct kmod_header {
    uint16_t magic;
    uint8_t abi_version;
    uint8_t kind;
    uint16_t image_size;
    uint16_t entry_init;
    uint16_t entry_clear;
    uint16_t entry_write;
};

typedef void (*kmod_init_fn)(void);
typedef void (*kmod_clear_fn)(void);
typedef void (*kmod_write_fn)(const char *);

#endif
