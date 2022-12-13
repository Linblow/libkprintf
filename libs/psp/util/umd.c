// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <util/umd.h>

/* Get the null-terminated UMD/ISO title ID (eg. UCUS98645).
   Ret 0 success; 1 IO error; < 0 SCE error */ 
int read_title_id(char title_id[10])
{
    int n;
    SceUID fd;
    char buf[16];

    if (title_id == NULL)
        return SCE_ERROR_INVALID_POINTER;

    fd = sceIoOpen("disc0:/UMD_DATA.BIN", SCE_O_RDONLY, 0644);
    if (fd < 0)
        return fd;
    n = sceIoRead(fd, buf, 10);
    sceIoClose(fd);
    if (n < 0)
        return n;
    if (n != 10)
        return 1;

    /* Transform eg. "UCUS-98645" into "UCUS98645" */
    memmove(buf + 4, buf + 5, 5);
    buf[9] = 0;
    memcpy(title_id, buf, 10);
    return 0;
}
