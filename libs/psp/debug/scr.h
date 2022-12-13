// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-FileCopyrightText: 2005 Marcus R. Brown <mrbrown@ocgnet.org>
// SPDX-FileCopyrightText: 2005 James Forshaw <tyranid@gmail.com>
// SPDX-FileCopyrightText: 2005 John Kelley <ps2dev@kelley.ca>
// SPDX-License-Identifier: BSD-3-Clause
#ifndef DEBUG_SCR_H
#define DEBUG_SCR_H

#include <common/types.h>

#ifdef __cplusplus
extern "C" {
#endif

enum debug_scr_flags_t
{
    FLAG_SCR_DEFAULT       = 1 << 0, /* init: Always set the default flags (ignore other flags if added). */
    FLAG_SCR_SET_DISPLAY   = 1 << 1, /* init: Set display mode and frame buf. */
    FLAG_SCR_CLEAR         = 1 << 2, /* init: Clear the VRAM frame buffer with bgcol. */
    FLAG_SCR_USE_BGCOL     = 1 << 3, /* Whether to enable background color. */
    FLAG_SCR_USE_CLEARLINE = 1 << 4, /* Whether to clear line. */
};

int  scr_init();
int  scr_initex(void *vram_base, int mode, u32 flags);
void scr_clear();
void scr_clearline(int y);
void scr_putchar(int x, int y, u32 fgcol, u8 ch);
int  scr_printdata(const char *buf, int bufsz);
int  scr_puts(const char *str);
int  scr_putsl(const char *str);
int  scr_printf(const char *fmt, ...) __attribute__((__format__(__printf__, 1, 2)));

#if KERNEL_MODULE
int scr_kprintf(const char *fmt, ...) __attribute__((__format__(__printf__, 1, 2)));
#else
#define scr_kprintf(fmt, ...)   do {} while (0)
#endif

int scr_use_bgcol(int on);
int scr_use_clearline(int on);

u32 scr_fgcol();
u32 scr_bgcol();
u32 scr_set_fgcol(u32 color);
u32 scr_set_bgcol(u32 color);

int        scr_vram_mode();
void *     scr_vram_base();
int        scr_vram_offset();
const u8 * scr_font();
int        scr_set_vram_mode(int mode);
void *     scr_set_vram_base(void *base);
int        scr_set_vram_offset(int offset);

u16  scr_x();
u16  scr_y();
u16  scr_setx(u16 x);
u16  scr_sety(u16 y);
void scr_setxy(u16 x, u16 y);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_SCR_H */
