#ifndef MEGA65_OS_KMODULE_H
#define MEGA65_OS_KMODULE_H
#include <stdint.h>
#include "mapper.h"
#include "module.h"
enum kmodule_result { KMODULE_OK=0, KMODULE_EMAP, KMODULE_EHEADER, KMODULE_EKIND };
enum kmodule_result kmodule_console_init(uint32_t phys);
enum kmodule_result kmodule_console_clear(uint32_t phys);
enum kmodule_result kmodule_console_write(uint32_t phys,const char *text);
#endif
