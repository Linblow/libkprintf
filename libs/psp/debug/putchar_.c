// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <debug/scr.h>

/* putchar function to call with lib printf's printf functions. */
void putchar_(char c)
{
    const char s[2] = {c, 0};
#if PRINTF_TO_TTY_STDOUT
    sceIoWrite(sceKernelStdout(), s, 1);
#else
    scr_printdata(s, 1);
#endif
}
