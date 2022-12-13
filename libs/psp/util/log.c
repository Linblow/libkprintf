#include <common/psp/types.h>
#include <util/log.h>
#include <string.h>

#ifndef LOG_MAX_HANDLERS
#define LOG_MAX_HANDLERS (5)
#endif

#ifndef LOG_VSNPRINTF_BUFSZ
#define LOG_VSNPRINTF_BUFSZ (512)
#endif

/* Define USE_LOG when NO_LOG_PRINTF is not defined. */
#ifndef NO_LOG_PRINTF
#undef  USE_LOG
#define USE_LOG 1
#endif

#if USE_LOG

static log_handler_t g_handlers[LOG_MAX_HANDLERS] = {0};
static u32 g_handler_count = 0;

static int find_free_handler_index()
{
    u32 i;
    for (i = 0; i < LOG_MAX_HANDLERS; ++i)
    {
        if (g_handlers[i].cb == NULL)
            return i;
    }
    return -1;
}

int log_register_handler(log_handler_cb_t cb, log_flush_cb_t flush, void *arg)
{
    int index;
    if (cb == NULL)
        return -1;
    if (g_handler_count >= LOG_MAX_HANDLERS)
        return -2;
    index = find_free_handler_index();
    if (index < 0)
        return -3;
    g_handlers[index].cb = cb;
    g_handlers[index].flush = flush;
    g_handlers[index].arg = arg;
    g_handler_count++;
    return index;
}

int log_unregister_handler(int index)
{
    if (index < 0 || index >= LOG_MAX_HANDLERS)
        return -1;
    if (g_handlers[index].cb == NULL)
        return -2;
    g_handlers[index].cb = NULL;
    g_handlers[index].arg = 0;
    g_handler_count--;
    return 0;
}

int log_printf(const char *fmt, ...)
{
    int len;
    char buf[LOG_VSNPRINTF_BUFSZ];
    int i;

    if (g_handler_count == 0)
        return 0;

    va_list va;
    va_start(va, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    if (len <= 0 || len >= sizeof(buf))
        return 0; /* error */

    for (i = 0; i < LOG_MAX_HANDLERS; ++i)
    {
        if (g_handlers[i].cb != NULL)
            g_handlers[i].cb(buf, len, g_handlers[i].arg);
    }

    return len;
}

void log_flush()
{
    int i;
    for (i = 0; i < g_handler_count; ++i)
    {
        if (g_handlers[i].flush != NULL)
            g_handlers[i].flush(g_handlers[i].arg);
    }
}

#endif /* USE_LOG */
