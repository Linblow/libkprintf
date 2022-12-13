// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>

/* putchar function to call with lib printf's printf functions. */
EXPORT_LTO
void putchar_(char c)
{
    const char s[2] = {c, 0};
    SET_K1(0);
    sceIoWrite(g_stdout, s, 1);
    RESTORE_K1();
}
