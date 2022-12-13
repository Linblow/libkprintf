// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <util/cb.h>

typedef void (*exit_cb_t)(void *common);

static exit_cb_t g_user_exit_cb = NULL;
static void *g_user_common = NULL;

static volatile int g_sleeping = 0;
// static SceUID g_umd_cbid = -1; 
static SceUID g_exit_cbid = -1; 
static SceUID g_thid = -1;

// static int umd_callback(int arg);
static int exit_callback(int count, int arg, void *common);
static int create_callbacks();
static int callback_thread(SceSize args, void *argp);

// static int umd_callback(int arg)
// {
    // return 0;
// }

static int exit_callback(int count, int arg, void *common)
{
    (void)count;
    (void)arg;
    (void)common;
    if (g_user_exit_cb)
        g_user_exit_cb(g_user_common);
    sceKernelExitGame();
    sceKernelSleepThread();
    return 0;
}

static int create_callbacks()
{
    // g_umd_cbid = sceKernelCreateCallback("umd_cb", (SceKernelCallbackFunction)umd_callback, 0);
    // sceUmdRegisterUMDCallBack(g_umd_cbid);
    g_exit_cbid = sceKernelCreateCallback("exit_cb", exit_callback, NULL);
    sceKernelRegisterExitCallback(g_exit_cbid);
    return 0;
}

static int callback_thread(SceSize args, void *argp)
{
    create_callbacks();
    g_sleeping = 1;
    sceKernelSleepThreadCB();
    return 0;
}

void kill_callbacks()
{
    while (!g_sleeping)
        sceKernelDelayThread(1000);
    // sceKernelDeleteCallback(g_umd_cbid);
    sceKernelDeleteCallback(g_exit_cbid);
    sceKernelTerminateDeleteThread(g_thid);
    // g_umd_cbid = -1;
    g_exit_cbid = -1;
    g_thid = -1;
}

int setup_callbacks(void (*exit_cb_cb)(void *common), void *common)
{
    int ret;
    u32 attr = SCE_KERNEL_TH_NO_FILLSTACK | SCE_KERNEL_TH_CLEAR_STACK;
    g_user_exit_cb = exit_cb_cb;
    g_user_common = common;
    ret = sceKernelCreateThread("cb_thread", (SceKernelThreadEntry)callback_thread, 16, 3*1024, attr, NULL);
    if (ret < 0)
        return ret;
    g_thid = ret;
    return sceKernelStartThread(g_thid, 0, NULL);
}
