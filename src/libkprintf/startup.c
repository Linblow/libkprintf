#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>

#define MOD_ATTR                        \
    SCE_MODULE_KERNEL               |   \
    SCE_MODULE_ATTR_EXCLUSIVE_START |   \
    SCE_MODULE_ATTR_EXCLUSIVE_LOAD

SCE_MODULE_INFO(MOD_NAME, MOD_ATTR, MOD_VER_MAJOR, MOD_VER_MINOR);
SCE_SDK_VERSION(SDK_VERSION);

#if USE_START_STOP_THREAD_PARAMETER
#define START_STOP_INITPRIO  (16)
#define START_STOP_STACKSIZE (1*1024)
#define START_STOP_ATTR      (0)
SCE_MODULE_START_THREAD_PARAMETER(3, START_STOP_INITPRIO, START_STOP_STACKSIZE, START_STOP_ATTR);
SCE_MODULE_STOP_THREAD_PARAMETER(3, START_STOP_INITPRIO, START_STOP_STACKSIZE, START_STOP_ATTR);
#endif /* USE_START_STOP_THREAD_PARAMETER */

SCE_MODULE_BOOTSTART("_start");
SCE_MODULE_STOP("_stop");
SCE_MODULE_REBOOT_BEFORE_FOR_USER("_stop");

static int init() 
{
    int ret;
    SceUID vplid = -1;
    SceUID evid = -1;
    SceUID semid = -1;
    SceUID thid = -1;
    u32 vplsize = KPRINTF_MSG_VPLSIZE_KIB * 1024;
    u32 stacksize = KPRINTF_EV_THREAD_STACKSIZE_KIB* 1024;
    int initprio = KPRINTF_EV_THREAD_PRIORITY;

    g_state = STATE_STARTING;

    ret = sceKernelCreateVpl("", SCE_KERNEL_PRIMARY_KERNEL_PARTITION, SCE_KERNEL_VA_MEMBTM, vplsize, NULL);
    vplid = ret;
    if (ret < 0)
        goto end;
    ret = sceKernelCreateEventFlag("", SCE_KERNEL_EA_MULTI, 0, NULL);
    evid = ret;
    if (ret < 0)
        goto end;
    ret = sceKernelCreateSema("", 0, 1, 1, NULL);
    semid = ret;
    if (ret < 0)
        goto end;
    ret = sceKernelCreateThread(MOD_NAME "_th", (SceKernelThreadEntry)event_thread, initprio, stacksize, 0, NULL);
    thid = ret;
    if (ret < 0)
        goto end;

    g_vplid = vplid;
    g_evid = evid;
    g_semid = semid;
    g_thid = thid;
    g_default_handler_index = -1;
    g_handler_count = 0;
    g_head = NULL;
    
    ret = sceKernelStartThread(thid, 0, NULL);
    if (ret < 0)
        goto end;

    ret = install_kernel_handler();
    if (ret < 0)
        goto end;
    
    g_state = STATE_STARTED;
    ret = 0;
end:
    if (ret != 0)
    {
        if (thid > 0)
            sceKernelTerminateDeleteThread(thid);
        if (semid > 0)
            sceKernelDeleteSema(semid);
        if (evid > 0)
            sceKernelDeleteEventFlag(evid);
        if (vplid > 0)
            sceKernelDeleteVpl(vplid);
        g_state = STATE_STOPPED;
    }

    return ret;
}

EXPORT_LTO int _start(SceSize args, void *argp)
{
    g_stdout = sceKernelStdout();
    return init() == 0 ? SCE_KERNEL_RESIDENT : SCE_KERNEL_NO_RESIDENT;
}

EXPORT_LTO int _stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    u32 timeout = 250*1000;
    if (g_state == STATE_STARTED)
    {
        KPRINTF("unregister internal Kprintf handler\n");

        /* Our module is gonna be unloaded, and our handler will be invalid.
           We have to make sure our handler is no longer registered.  */
        sceKernelRegisterKprintfHandler(NULL, NULL);
        g_state = STATE_STOPPED;

        /* The events thread may be flusing messages. 
           We wanna make sure it has finished before stopping the module.
           To do so, we send the events thread a terminate event.
           When it's done flushing, it'll send us a EV_STOP_OK event. */
        sceKernelSetEventFlag(g_evid, EV_FLUSH | EV_TERMINATE);
        /* Wait a bit until the events thread tells us to continue. */
        sceKernelWaitEventFlag(g_evid, EV_STOP_OK, SCE_KERNEL_EW_OR | SCE_KERNEL_EW_CLEAR_PAT, NULL, &timeout);
    }
    return 0;
}

#if 1
/* This ugly hack is required so that LTO doesn't remove the exports in the final exec. */
extern const struct SceLibraryEntry __library_exports[];
__attribute__((used)) int __psp_exports_dummy_func() {return (int)&__library_exports; }
#endif
