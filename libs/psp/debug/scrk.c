// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-FileCopyrightText: 2005 Marcus R. Brown <mrbrown@ocgnet.org>
// SPDX-FileCopyrightText: 2005 James Forshaw <tyranid@gmail.com>
// SPDX-FileCopyrightText: 2005 John Kelley <ps2dev@kelley.ca>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <debug/scr.h>

/* Debug screen printf for kernel, uses the kernel prnt function instead of vsnprintf. */
#if KERNEL_MODULE

static void prnt_cb(struct prnt_ctx *ctx, int ch)
{
    switch (ch)
    {
        case 0x200: /* start of string */
            ctx->len = 0;
            break;
        case 0x201: /* end of string */
            scr_printdata(ctx->buf, ctx->len);
            ctx->len = 0;
            break;
        case '\r': /* ignore this one */
            break;
        default: /* regular character */
            ctx->buf[ctx->len++] = ch;
            if(ctx->len == PRNT_CTX_BUFSZ)
            {
                scr_printdata(ctx->buf, ctx->len);
                ctx->len = 0;
            }
    }
}

void scr_kprintf(const char *fmt, ...)
{
    prnt_ctx ctx;
    ctx.len = 0;
    va_list va;
    va_start(va, fmt);
    prnt((prnt_callback)prnt_cb, (void *)&ctx, fmt, va);
    va_end(va);
}

#endif /* KERNEL_MODULE */
