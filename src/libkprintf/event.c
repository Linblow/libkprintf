#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>

#if DEBUG
static int debug_file_write(const char *path, const char *fmt, ...)
{
    int len;
    char buf[256];
    SceUID fd;
    int ret;
    va_list va;
    va_start(va, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    if (len <= 0 || len >= sizeof(buf))
        return 0; /* error */
    fd = sceIoOpen(path, SCE_O_CREAT | SCE_O_APPEND | SCE_O_RDWR, 0777);
    ret = sceIoWrite(fd, buf, len);
    sceIoClose(fd);
    sceIoSync("ms0:", 0);
    return ret;
}

static int dmsg(const char *fmt, ...)
{
    int len;
    char buf[256];
    va_list va;
    va_start(va, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    if (len <= 0 || len >= sizeof(buf))
        return 0; /* error */
    return sceIoWrite(g_stdout, buf, len);
}
#endif

static int detect_psplink(u32 timeout_ms)
{
    u32 t = 0;
    while (t < timeout_ms)
    {
        if (sceKernelFindModuleByName("PSPLINK"))
            return 1;
        sceKernelDelayThread(1000);
        t += 1000;
    }
    return 0;
}

int event_thread(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    int ret;
    u32 ev;

    while (g_state != STATE_STARTED)
        sceKernelDelayThread(100);
    
#if KPRINTF_DH_AUTO_REGISTER
    register_default_handler();
#endif

    /* Give time to PSPLINK and the host to properly setup the USB connection.
       Thus allowing early messages to be seen in pspsh's terminal. */
    if (detect_psplink(KPRINTF_PSPLINK_DETECT_TIMEOUT))
        sceKernelDelayThread(KPRINTF_PSPLINK_USB_DELAY);

#if DEBUG
    dmsg("\n");
    dmsg("msg_vplsize_kib:         %d\n", KPRINTF_MSG_VPLSIZE_KIB);
    dmsg("ev_thread_priority:      %d\n", KPRINTF_EV_THREAD_PRIORITY);
    dmsg("ev_thread_stacksize_kib: %d\n", KPRINTF_EV_THREAD_STACKSIZE_KIB);
    dmsg("dh_flags: ");
#if KPRINTF_DH_AUTO_REGISTER
    dmsg("AUTO_REGISTER ");
#endif
#if KPRINTF_DH_AUTO_NEWLINE
    dmsg("AUTO_NEWLINE ");
#endif
#if KPRINTF_DH_WRITE_KPRINTF
    dmsg("WRITE_KPRINTF ");
#endif
#if KPRINTF_DH_WRITE_PRINTF
    dmsg("WRITE_PRINTF");
#endif
    dmsg("\n");
    dmsg("psplink_detect_timeout:  %u\n", KPRINTF_PSPLINK_DETECT_TIMEOUT);
    dmsg("psplink_usb_delay:       %u\n", KPRINTF_PSPLINK_USB_DELAY);
#endif

    while (1)
    {
        ev = 0;
        sceKernelWaitEventFlagCB(g_evid, EV_FLUSH | EV_TERMINATE, SCE_KERNEL_EW_OR | SCE_KERNEL_EW_CLEAR_PAT, &ev, NULL);
        if ((ev & EV_FLUSH) != 0)
        {
            ret = sceKernelWaitSema(g_semid, 1, NULL);
            if (ret == 0)
            {
                flush_messages();
                sceKernelSignalSema(g_semid, 1);
            }
            else
            {
                /* Resend the event since somehow we couldn't get the sema. */
                sceKernelSetEventFlag(g_evid, EV_FLUSH);
                sceKernelDelayThread(1000);
            }
        }
        if ((ev & EV_TERMINATE) != 0)
        {
            sceKernelSetEventFlag(g_evid, EV_STOP_OK);
            return 0;
        }
    }

    return 0;
}
