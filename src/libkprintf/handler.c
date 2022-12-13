#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>
#include <printf/printf.h>

/**
 * Kprintf handler callback.
 * @param arg Optional argument passed to sceKernelRegisterKprintfHandler().
 * @param fmt Printf-like format string.
 * @param va Variable arguments list.
 * @param isUser Whether this is a user mode call (sceKernelPrintf) or kernel (Kprintf).
 * @return >= 0 on success, the total number of characters is returned .
 *         < 0 on failure, a negative number (error code).
 *         Either way, this value is returned by Kprintf() / sceKernelPrintf().
 */
// typedef int (*SceKernelKprintfHandler)(void *arg, const char *fmt, va_list va, int isUser);

/* NOTE: make sure to use the 6.60/6.61 stubs for the correct NID!
   Most PSPSDK's kernel NIDs are outdated ones from old firmwares. */
// extern void sceKernelRegisterKprintfHandler(SceKernelKprintfHandler handler, void *arg);
// extern int sceKernelDipsw(u32 reg);
// extern int sceKernelPrintf(const char *fmt, ...);

/* This function MUST be called in interrupts-ENABLED state. 
   Otherwise, the user callbacks are most likely going to fail. */
void invoke_handlers(kprintf_msg_t *m)
{
    int i;
    int res;
    for (i = 0; i < KPRINTF_MAX_HANDLERS; ++i)
    {
        if (g_handlers[i].cb != NULL)
        {
            res = g_handlers[i].cb(g_handlers[i].arg, m->msg, &(m->len), m->size, m->isUser);
            if (res == KPRINTF_MSG_STOP)
                break; /* do not pass this message to the next handlers. */
        }
    }
}

static int find_free_handler_index()
{
    u32 i;
    for (i = 0; i < KPRINTF_MAX_HANDLERS; ++i)
    {
        if (g_handlers[i].cb == NULL)
            return i;
    }
    return -1;
}

int register_handler(kprintf_handler_t cb, void *arg, int is_default_handler)
{
    int ret, sret, index;
    
    if (g_state != STATE_STARTED)
        return KPRINTF_ERR_NOT_STARTED;
    if (cb == NULL)
        return KPRINTF_ERR_INVALID_ARGUMENT;
    
    sret = sceKernelWaitSema(g_semid, 1, NULL);
    if (sret < 0)
        return sret;
    
    ret = g_default_handler_index;
    if (is_default_handler && g_default_handler_index >= 0)
        goto end;

    ret = KPRINTF_ERR_MAX_HANDLERS;
    if (g_handler_count >= KPRINTF_MAX_HANDLERS)
        goto end;
    
    ret = KPRINTF_ERR_MAX_HANDLERS;
    index = find_free_handler_index();
    if (index < 0)
        goto end;

    if (is_default_handler)
        g_default_handler_index = index;
    
    g_handlers[index].cb = cb;
    g_handlers[index].arg = arg;
    g_handler_count++;
    ret = index;

end:
    sret = sceKernelSignalSema(g_semid, 1);
    return sret < 0 ? sret : ret;
}

static int default_file_handler(void *arg, char *msg, unsigned short *pLen, unsigned short size, int isUser)
{
    SceUID fd = (SceUID)arg;
    int len = *pLen;
    int can_write = 0;

#if KPRINTF_DH_WRITE_PRINTF
    if (isUser)
        can_write = 1;
#endif
#if KPRINTF_DH_WRITE_KPRINTF
    if ( ! isUser)
        can_write = 1;
#endif

#if KPRINTF_DH_AUTO_NEWLINE
    if (msg[len - 1] != '\n')
    {
        msg[len] = '\n';
        msg[len+1] = 0;
        len++;
    }
#endif

    if (can_write)
        sceIoWrite(fd, msg, len);
    
    *pLen = len;
    return KPRINTF_MSG_OK;
}

int register_default_handler()
{
    int ret;
    ret = register_handler(default_file_handler, (void *)g_stdout, 1);
    if (ret == 0)
        KPRINTF("registered default handler\n");
    return ret;
}

int unregister_handler(int index)
{
    int ret, sret;

    if (g_state != STATE_STARTED)
        return KPRINTF_ERR_NOT_STARTED;
    
    sret = sceKernelWaitSema(g_semid, 1, NULL);
    if (sret < 0)
        return sret;
    
    ret = KPRINTF_ERR_INVALID_INDEX;
    if (index < 0 || index >= KPRINTF_MAX_HANDLERS)
        goto end;
    
    ret = KPRINTF_ERR_NO_HANDLER;
    if (g_handlers[index].cb == NULL)
        goto end;

    if (index == g_default_handler_index)
        g_default_handler_index  = -1;
    
    g_handlers[index].cb = NULL;
    g_handlers[index].arg = 0;
    g_handler_count--;
    ret = 0;

end:
    sret = sceKernelSignalSema(g_semid, 1);
    return sret < 0 ? sret : ret;
}

/* IMPORTANT: interrupts are disabled inside the Kprintf handler. 
   Most APIs such as sceIo cannot be called in interrupts-disabled state.
   Hence the need to add the message to a queue for processing in the events thread. */
static int kprintf_handler(void *arg, const char *fmt, va_list va, int isUser)
{
    int ret;
    char buf[KPRINTF_BUFSZ];
    int len;
    kprintf_msg_t *m = NULL;

    /* Nothing to do? */
    if (g_state != STATE_STARTED)
        return 0;

    /* call tiny printf library's vsnprintf */
    len = vsnprintf(buf, KPRINTF_BUFSZ, fmt, va);
    if (len <= 0 || len >= sizeof(buf))
        return 0; /* error */

    ret = new_msg(&m, buf, len, isUser);
    if (ret < 0)
        return ret;

    add_msg(m);
    sceKernelSetEventFlag(g_evid, EV_FLUSH);
    return len;
}

int install_kernel_handler()
{
    int ret;

    /* Register 56 controls whether to invoke the registered Kprintf handler. 
       We MUST enable it. This call can only succeed in kernel-mode. */
    ret = sceKernelDipswSet(56);
    if (ret < 0)
        return ret;

    sceKernelRegisterKprintfHandler(kprintf_handler, NULL);
    return 0;
}
