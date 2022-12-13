// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <debug/scr.h>
#include <util/cb.h>
#include <util/sys.h>

// PSP_DISABLE_NEWLIB();
PSP_DISABLE_AUTOSTART_PTHREAD();
PSP_MAIN_THREAD_ATTR(SCE_KERNEL_TH_USER_MODE);

char g_username[128] = {0};

static int draw_frame()
{
    u32 i;

    scr_setxy(0, 0);
    scr_puts("Calling libkprintf's printf every second...");

    i = 0;
    while (1)
    {
        printf("Hello %s %lu from printf!\n", g_username, ++i);
        sceKernelDelayThread(1*1000*1000);
    }

    return 0;
};

int main(int argc, char *argv[])
{
    int ret;

    setup_callbacks(NULL, NULL);
    sceDisplaySetHoldMode(1);
    scr_initex(NULL, PSP_DISPLAY_PIXEL_FORMAT_8888, FLAG_SCR_SET_DISPLAY | FLAG_SCR_CLEAR);
    
    sys_get_username(g_username, sizeof(g_username));
    while (1)
    {
        sceDisplayWaitVblankStartCB();
        ret = draw_frame();
        if (ret != 0)
            break;
    }

    kill_callbacks();
    return 0;
}
