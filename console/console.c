#include <stdint.h>

#define REG8(a) (*(volatile uint8_t *)(uintptr_t)(a))
#define SCREEN ((volatile uint8_t *)0x0800u)
#define COLOR  ((volatile uint8_t *)0xd800u)
#define CELLS 2000u

static uint8_t cursor_x, cursor_y;

static uint8_t screen_code(uint8_t c)
{
    if (c >= 'A' && c <= 'Z') return (uint8_t)(c - 64);
    if (c >= 'a' && c <= 'z') return (uint8_t)(c - 96);
    return c;
}

void console_clear(void)
{
    uint16_t i;
    for(i=0;i<CELLS;i++){ SCREEN[i]=0x20; COLOR[i]=0x01; }
    cursor_x=cursor_y=0;
}

void console_init(void)
{
    /* Enable VIC-IV I/O, select 2 KiB colour-RAM aperture, 80-column H640,
     * and an 80-byte text row. Screen RAM is fixed at $0800 for bring-up.
     */
    REG8(0xd02f)=0x47; REG8(0xd02f)=0x53;
    REG8(0xd030) |= 0x01;       /* CRAM2K: $D800-$DFFF */
    REG8(0xd031) |= 0x80;       /* H640: 640 pixels / 80 columns */
    REG8(0xd018)=(uint8_t)((REG8(0xd018)&0x0f)|0x20); /* screen $0800 */
    REG8(0xd058)=80; REG8(0xd059)=0;
    console_clear();
}

void console_write(const char *s)
{
    while(*s){
        uint8_t c=(uint8_t)*s++;
        if(c=='\n'){cursor_x=0;if(cursor_y<24)cursor_y++;continue;}
        if(c=='\r'){cursor_x=0;continue;}
        if(cursor_x>=80){cursor_x=0;if(cursor_y<24)cursor_y++;}
        if(cursor_y>=25) return; /* scrolling comes later */
        {
            uint16_t p=(uint16_t)cursor_y*80u+cursor_x++;
            SCREEN[p]=screen_code(c); COLOR[p]=0x01;
        }
    }
}
