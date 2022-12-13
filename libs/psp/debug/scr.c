// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-FileCopyrightText: 2005 Marcus R. Brown <mrbrown@ocgnet.org>
// SPDX-FileCopyrightText: 2005 James Forshaw <tyranid@gmail.com>
// SPDX-FileCopyrightText: 2005 John Kelley <ps2dev@kelley.ca>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <debug/scr.h>

/*
    Define USE_DEBUG_SCREEN_16 to support 16-bits colors.
*/

#define SCE_DISPLAY_MODE_LCD                0    // Both PSP and TOOL
#define SCE_DISPLAY_MODE_VESA1A             0x1a // TOOL only
#define SCE_DISPLAY_MODE_PSEUDO_VGA         0x60 // TOOL only

#define SCE_DISPLAY_PIXEL_RGB565            0 // 16 bits R:G:B   5:6:5
#define SCE_DISPLAY_PIXEL_RGBA5551          1 // 16 bits R:G:B:A 5:5:5:1
#define SCE_DISPLAY_PIXEL_RGBA4444          2 // 16 bits R:G:B:A 4:4:4:4
#define SCE_DISPLAY_PIXEL_RGBA8888          3 // 32 bits R:G:B:A 8:8:8:8

#define SCE_DISPLAY_UPDATETIMING_NEXTHSYNC  0
#define SCE_DISPLAY_UPDATETIMING_NEXTVSYNC  1

/* LCD screen size. */
#define SCR_WIDTH       480
#define SCR_HEIGHT      272
#define SCR_ASPECT      ((float)SCR_WIDTH / (float)SCR_HEIGHT)
#define SCR_OFFSETX     ((4096 - SCR_WIDTH) / 2)
#define SCR_OFFSETY     ((4096 - SCR_HEIGHT) / 2)

/* Frame buffer. */
#define VRAM_TOP        0x00000000
#define VRAM_WIDTH      512
/* 16 bits mode. */
#define VRAM_BUFSIZE    (VRAM_WIDTH * SCR_HEIGHT * 2)
#define VRAM_BP_0       (void *)(VRAM_TOP)
#define VRAM_BP_1       (void *)(VRAM_TOP + VRAM_BUFSIZE)
#define VRAM_BP_2       (void *)(VRAM_TOP + VRAM_BUFSIZE * 2)
/* 32 bits mode. */
#define VRAM_BUFSIZE32  (VRAM_WIDTH * SCR_HEIGHT * 4)
#define VRAM_BP32_0     (void *)(VRAM_TOP)
#define VRAM_BP32_1     (void *)(VRAM_TOP + VRAM_BUFSIZE32)
#define VRAM_BP32_2     (void *)(VRAM_TOP + VRAM_BUFSIZE32 * 2)

/* Character size in pixels. Do not change. */
#define FONTCHAR_WIDTH_PX   7
#define FONTCHAR_HEIGHT_PX  8
/* Character max positions constants. */
#define MX (SCR_WIDTH  / FONTCHAR_WIDTH_PX)  // 480 / 7 = 68.57 = 68
#define MY (SCR_HEIGHT / FONTCHAR_HEIGHT_PX) // 272 / 8 = 34

#ifndef SCR_PRINTF_BUFSZ
#define SCR_PRINTF_BUFSZ    512
#endif 
#ifndef PRNT_CTX_BUFSZ
#define PRNT_CTX_BUFSZ      256
#endif

typedef struct debug_scr_t debug_scr_t;
typedef struct prnt_ctx prnt_ctx;

struct debug_scr_t
{
    u16        x;
    u16        y;
    u32        fgcol;
    u32        bgcol;
    u16        flags;
    u8         reserved;
    u8         vram_mode;
    void *     vram_base;
    int        vram_offset;
    const u8 * font;
};

struct prnt_ctx 
{
    u16  len;
    char buf[PRNT_CTX_BUFSZ];
};

/* Default font (see font.c) */
extern const unsigned char g_msx[];
static int g_init = 0; 
static debug_scr_t g_scr = 
{
    .x              = 0,
    .y              = 0,
    .fgcol          = 0xFFFFFFFF,
    .bgcol          = 0,
    .flags          = FLAG_SCR_USE_BGCOL | FLAG_SCR_USE_CLEARLINE,
    .vram_mode      = SCE_DISPLAY_PIXEL_RGBA8888,
    .vram_base      = (void *)0x04000000,
    .vram_offset    = 0,
    .font           = (const u8 *)g_msx
};

#if USE_DEBUG_SCREEN_16
static u16 convert_8888_to_565(u32 color)
{
    int r, g, b;
    b = (color >> 19) & 0x1F;
    g = (color >> 10) & 0x3F;
    r = (color >> 3)  & 0x1F;
    return r | (g << 5) | (b << 11);
}

static u16 convert_8888_to_5551(u32 color)
{
    int r, g, b, a;
    a = (color >> 24) ? 0x8000 : 0;
    b = (color >> 19) & 0x1F;
    g = (color >> 11) & 0x1F;
    r = (color >> 3)  & 0x1F;
    return a | r | (g << 5) | (b << 10);
}

static u16 convert_8888_to_4444(u32 color)
{
    int r, g, b, a;
    a = (color >> 28) & 0xF; 
    b = (color >> 20) & 0xF;
    g = (color >> 12) & 0xF;
    r = (color >> 4)  & 0xF;
    return (a << 12) | r | (g << 4) | (b << 8);
}

static void clear_screen_16(u16 color)
{
    int x;
    u16 *vram = g_scr.vram_base;
    vram += (g_scr.vram_offset >> 1);
    for (x = 0; x < (VRAM_WIDTH * SCR_HEIGHT); x++)
        *vram++ = color; 
}
#endif /* USE_DEBUG_SCREEN_16 */

static void clear_screen_32(u32 color)
{
    int x;
    u32 *vram = g_scr.vram_base;
    vram += (g_scr.vram_offset >> 2);
    for (x = 0; x < (VRAM_WIDTH * SCR_HEIGHT); x++)
        *vram++ = color; 
}

static void clear_screen(u32 color)
{
#if USE_DEBUG_SCREEN_16
    u16 c;
    switch (g_scr.vram_mode)
    {
        case SCE_DISPLAY_PIXEL_RGBA8888:
            clear_screen_32(color);
            return;
        case SCE_DISPLAY_PIXEL_RGB565:   c = convert_8888_to_565(color);  break;
        case SCE_DISPLAY_PIXEL_RGBA5551: c = convert_8888_to_5551(color); break;
        case SCE_DISPLAY_PIXEL_RGBA4444: c = convert_8888_to_4444(color); break;
        default: c = 0;
    }
    clear_screen_16(c);
#else /* ! USE_DEBUG_SCREEN_16 */
    clear_screen_32(color);
#endif /* USE_DEBUG_SCREEN_16 */
}

static void init_debug_scr(debug_scr_t *scr)
{
    memset(scr, 0, sizeof(debug_scr_t));
    scr->x = 0;
    scr->y = 0;
    scr->fgcol = 0xFFFFFFFF;
    scr->bgcol = 0;
    scr->flags = FLAG_SCR_USE_BGCOL | FLAG_SCR_USE_CLEARLINE;
    scr->vram_mode = SCE_DISPLAY_PIXEL_RGBA8888;
    scr->vram_base = (void *)0x04000000;
    scr->vram_offset = 0;
    scr->font = (const u8 *)g_msx;
}

static int get_valid_mode(int mode)
{
    switch (mode)
    {
        case SCE_DISPLAY_PIXEL_RGB565:
        case SCE_DISPLAY_PIXEL_RGBA5551:
        case SCE_DISPLAY_PIXEL_RGBA4444:
        case SCE_DISPLAY_PIXEL_RGBA8888:
            break;
        default: mode = SCE_DISPLAY_PIXEL_RGBA8888;
    }
    return mode;
}

static void * get_default_vram_base(u32 offset)
{
    /* uncached address. */
    return (void *)((0x40000000 | (u32)sceGeEdramGetAddr()) + offset);
}

static int set_display(const debug_scr_t *scr)
{
    int ret, mode, width, height;
    ret = sceDisplayGetMode(&mode, &width, &height);
    if (ret < 0)
        return ret;
    /* This call will fail when interrupts are disabled. */
    ret = sceDisplaySetMode(SCE_DISPLAY_MODE_LCD, SCR_WIDTH, SCR_HEIGHT);
    if (ret < 0)
        return ret;
    ret = sceDisplaySetFrameBuf(scr->vram_base, VRAM_WIDTH, scr->vram_mode, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);
    if (ret < 0)
    {
        /* Restore previous mode. */
        sceDisplaySetMode(mode, width, height);
        return ret;
    }
    return 0;
}

int scr_initex(void *vram_base, int mode, u32 flags)
{
    int ret;
    debug_scr_t scr;
    
    init_debug_scr(&scr);
    scr.vram_mode = get_valid_mode(mode);
    scr.vram_base = vram_base ? vram_base : get_default_vram_base((u32)VRAM_BP32_0);

    /* Set the specified flags only when FLAG_SCR_DEFAULT was *not* passed. */
    if ((flags & FLAG_SCR_DEFAULT) == 0)
        scr.flags = flags;

    if (flags & FLAG_SCR_SET_DISPLAY)
    {
        ret = set_display(&scr);
        if (ret < 0)
            return ret;
    }
    if (flags & FLAG_SCR_CLEAR)
        clear_screen(scr.bgcol);

    memcpy(&g_scr, &scr, sizeof(debug_scr_t));
    g_init = 1;
    return 0;
}

int scr_init()
{
    return scr_initex(NULL, SCE_DISPLAY_PIXEL_RGBA8888, FLAG_SCR_DEFAULT);
}

u32 scr_fgcol()
{
    return g_scr.fgcol;
}

u32 scr_bgcol()
{
    return g_scr.bgcol;
}

int scr_vram_mode()
{
    return g_scr.vram_mode;
}

void * scr_vram_base()
{
    return g_scr.vram_base;
}

int scr_vram_offset()
{
    return g_scr.vram_offset;
}

const u8 * scr_font()
{
    return g_scr.font;
}

u16 scr_x()
{
    return g_scr.x;
}

u16 scr_y()
{
    return g_scr.y;
}

int scr_use_bgcol(int on)
{
    u32 f = g_scr.flags;
    g_scr.flags = on ? (f | FLAG_SCR_USE_BGCOL) : (f & ~FLAG_SCR_USE_BGCOL);
    return (f & FLAG_SCR_USE_BGCOL) != 0;
}

int scr_use_clearline(int on)
{
    u32 f = g_scr.flags;
    g_scr.flags = on ? (f | FLAG_SCR_USE_CLEARLINE) : (f & ~FLAG_SCR_USE_CLEARLINE);
    return (f & FLAG_SCR_USE_CLEARLINE) != 0;
}

u32 scr_set_fgcol(u32 color)
{
    u32 prev = g_scr.fgcol;
    g_scr.fgcol = color;
    return prev;
}

u32 scr_set_bgcol(u32 color)
{
    u32 prev = g_scr.bgcol;
    g_scr.bgcol = color;
    return prev;
}

int scr_set_vram_mode(int mode)
{
    int prev = g_scr.vram_mode; 
    g_scr.vram_mode = get_valid_mode(mode);
    return prev;
}

void * scr_set_vram_base(void *base)
{
    void *prev = g_scr.vram_base;
    g_scr.vram_base = base;
    return prev;
}

int scr_set_vram_offset(int offset)
{
    int prev = g_scr.vram_offset;
    g_scr.vram_offset = offset;
    return prev;
}

u16 scr_setx(u16 x)
{
    int prev = g_scr.x;
    if (x < MX && x >= 0)
        g_scr.x = x;
    return prev;
}

u16 scr_sety(u16 y)
{
    int prev = g_scr.y;
    if (y < MY && y >= 0)
        g_scr.y = y;
    return prev;
}

void scr_setxy(u16 x, u16 y)
{
    scr_setx(x);
    scr_sety(y);
}

void scr_clear()
{
    int y;
    if (!g_init)
        return;
    //TODO is this loop useless?
    for (y = 0; y < MY; y++)
        scr_clearline(y);
    scr_setxy(0, 0);
    clear_screen(g_scr.bgcol);
}

void scr_clearline(int y)
{
    int i;
    if ((g_scr.flags & FLAG_SCR_USE_CLEARLINE) == 0)
        return;
    if ((g_scr.flags & FLAG_SCR_USE_BGCOL) == 0)
        return;
    for (i = 0; i < MX; i++)
        scr_putchar(i*7 , g_scr.y*8, g_scr.bgcol, 219);
}

static void drawchar_32(int x, int y, u32 fgcol, u32 bgcol, u8 ch)
{
    int i, j;
    u32 *vram_ptr;
    u32 *vram;
    const u8 *font = &g_scr.font[(int)ch * 8];
    // font = &msx[ (int)ch * 8];

    if (!g_init)
        return;

    vram  = g_scr.vram_base + (g_scr.vram_offset >> 2);
    vram += x + (y * VRAM_WIDTH);

    for (i = 0; i < 8; i++, font++)
    {
        vram_ptr = vram;
        for (j = 0; j < 8; j++, vram_ptr++)
        {
            if ((*font & (128 >> j)))
                *vram_ptr = fgcol;
            else if (g_scr.flags & FLAG_SCR_USE_BGCOL)
                *vram_ptr = bgcol;
        }
        vram += VRAM_WIDTH;
    }
}

#if USE_DEBUG_SCREEN_16
static void drawchar_16(int x, int y, u32 fgcol, u32 bgcol, u8 ch)
{
    int i, j;
    u16 *vram_ptr;
    u16 *vram;
    const u8 *font = &g_scr.font[(int)ch * 8];
    // font = &msx[ (int)ch * 8];

    if (!g_init)
        return;

    vram  = g_scr.vram_base + (g_scr.vram_offset >> 1);
    vram += x + (y * VRAM_WIDTH);

    for (i = 0; i < 8; i++, font++)
    {
        vram_ptr = vram;
        for (j = 0; j < 8; j++, vram_ptr++)
        {
            if ((*font & (128 >> j)))
                *vram_ptr = fgcol;
            else if (g_scr.flags & FLAG_SCR_USE_BGCOL)
                *vram_ptr = bgcol;
        }
        vram += VRAM_WIDTH;
    }
}
#endif /* USE_DEBUG_SCREEN_16 */

void scr_putchar(int x, int y, u32 fgcol, u8 ch)
{
#if USE_DEBUG_SCREEN_16
    u16 c, b;
    switch (g_scr.vram_mode)
    {
        case SCE_DISPLAY_PIXEL_RGBA8888:
            drawchar_32(x, y, fgcol, g_scr.bgcol, ch);
            return;
        case SCE_DISPLAY_PIXEL_RGB565:
            c = convert_8888_to_565(fgcol);
            b = convert_8888_to_565(g_scr.bgcol);
            break;
        case SCE_DISPLAY_PIXEL_RGBA5551:
            c = convert_8888_to_5551(fgcol);
            b = convert_8888_to_5551(g_scr.bgcol);
            break;
        case SCE_DISPLAY_PIXEL_RGBA4444:
            c = convert_8888_to_4444(fgcol);
            b = convert_8888_to_4444(g_scr.bgcol);
            break;
        default:
            c = 0;
            b = 0;
    }
    drawchar_16(x, y, c, b, ch);
#else /* ! USE_DEBUG_SCREEN_16 */
    drawchar_32(x, y, fgcol, g_scr.bgcol, ch);
#endif /* USE_DEBUG_SCREEN_16 */
}

/* Print non-nul terminated strings */
int scr_printdata(const char *buf, int bufsz)
{
    int i, j;
    char c;
    if (!g_init)
        return 0;
    for (i = 0; i < bufsz; i++)
    {
        c = buf[i];
        switch (c)
        {
            case '\r':
                g_scr.x = 0;
                break;
            case '\n':
                g_scr.x = 0;
                g_scr.y++;
                if (g_scr.y == MY)
                    g_scr.y = 0;
                scr_clearline(g_scr.y);
                break;
            case '\t':
                for (j = 0; j < 4; j++)
                {
                    scr_putchar(g_scr.x*7, g_scr.y*8, g_scr.fgcol, ' ');
                    g_scr.x++;
                }
                break;
            default:
                scr_putchar(g_scr.x*7, g_scr.y*8, g_scr.fgcol, c);
                g_scr.x++;
                if (g_scr.x == MX)
                {
                    g_scr.x = 0;
                    g_scr.y++;
                    if (g_scr.y == MY)
                        g_scr.y = 0;
                    scr_clearline(g_scr.y);
                }
        }
    }
    return i;
}

int scr_puts(const char *str)
{
    return scr_printdata(str, str ? strlen(str) : 0);
}

int scr_putsl(const char *str)
{
    return scr_puts(str) + scr_printdata("\n", 1);
}

/* Return number of characters transmitted to the output stream or negative 
   value if an output error or an encoding error (for string and character 
   conversion specifiers) occurred. */
int scr_printf(const char *fmt, ...)
{
    char buf[SCR_PRINTF_BUFSZ];
    int len;
    va_list va;
    va_start(va, fmt);
    /* The number of characters written if successful or negative value if an error occurred. 
       If the resulting string gets truncated due to buf_size limit, function returns the 
       total number of characters (not including the terminating null-byte) which would 
       have been written, if the limit was not imposed. */
    len = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    if (len < 0) /* error */
        return len;
    if (len >= sizeof(buf))
        return 0; /* buf too small, nothing written. */
    return scr_printdata(buf, len);
}
