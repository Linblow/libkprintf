// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#ifndef PSP_UTIL_LOG_H
#define PSP_UTIL_LOG_H

#include <common/psp/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Define USE_LOG when NO_LOG_PRINTF is not defined. */
#ifndef NO_LOG_PRINTF
#undef  USE_LOG
#define USE_LOG 1
#endif

/* Taken from newlibc assert.h */
#ifndef __FUNC_NAME__
/* Use g++'s demangled names in C++.  */
# if defined __cplusplus && defined __GNUC__
#  define __FUNC_NAME__ __PRETTY_FUNCTION__
/* C99 requires the use of __func__.  */
# elif __STDC_VERSION__ >= 199901L
#  define __FUNC_NAME__ __func__
/* Older versions of gcc don't have __func__ but can use __FUNCTION__.  */
# elif __GNUC__ >= 2
#  define __FUNC_NAME__ __FUNCTION__
/* failed to detect __func__ support.  */
# else
#  define __FUNC_NAME__ ((char *) 0)
# endif
#endif /* !__FUNC_NAME__ */

#define __PRINTF_INFO(func, fmt, ...) \
    func(__FILE__ ":%d %s - " fmt, __LINE__, __FUNC_NAME__, ## __VA_ARGS__)

#if NO_KPRINTF
# define _KPRINTF(fmt, ...)         do {} while (0)
# define _KPRINTF_INFO(fmt, ...)    do {} while (0)
# define Kprintf(fmt, ...)          do {} while (0)
#else /* ! NO_KPRINTF */
# define _KPRINTF(fmt, ...)         Kprintf(fmt, __VA_ARGS__)
# define _KPRINTF_INFO(fmt, ...)    __PRINTF_INFO(Kprintf, fmt, __VA_ARGS__)
#endif /* NO_KPRINTF */

#if USE_LOG
# define _LOG_PRINTF(fmt, ...)      log_printf(fmt, __VA_ARGS__)
# define _LOG_PRINTF_INFO(fmt, ...) __PRINTF_INFO(log_printf, fmt, __VA_ARGS__)
#else /* ! USE_LOG */
# define _LOG_PRINTF(fmt, ...)      do {} while (0)
# define _LOG_PRINTF_INFO(fmt, ...) do {} while (0)
#endif /* USE_LOG */

/* Log-printf wrappers. */
#define OUT(fmt,  ...)      _LOG_PRINTF(fmt "\n", ## __VA_ARGS__)
#define NOTE(fmt, ...)      _LOG_PRINTF("NOTE " fmt "\n", ## __VA_ARGS__)
#define INFO(fmt, ...)      _LOG_PRINTF("INFO " fmt "\n", ## __VA_ARGS__)
#define WARN(fmt, ...)      _LOG_PRINTF("WARN " fmt "\n", ## __VA_ARGS__)
#define FAIL(fmt, ...)      _LOG_PRINTF("FAIL " fmt "\n", ## __VA_ARGS__

/* Log-printf wrappers with extra info. */
#define DOUT(fmt,  ...)     _LOG_PRINTF_INFO(fmt "\n", ## __VA_ARGS__)
#define DNOTE(fmt, ...)     _LOG_PRINTF_INFO("NOTE " fmt "\n", ## __VA_ARGS__)
#define DINFO(fmt, ...)     _LOG_PRINTF_INFO("INFO " fmt "\n", ## __VA_ARGS__)
#define DWARN(fmt, ...)     _LOG_PRINTF_INFO("WARN " fmt "\n", ## __VA_ARGS__)
#define DFAIL(fmt, ...)     _LOG_PRINTF_INFO("FAIL " fmt "\n", ## __VA_ARGS__)

/* Kprintf wrappers. */
#define KOUT(fmt,  ...)     _KPRINTF(fmt "\n", ## __VA_ARGS__)
#define KNOTE(fmt, ...)     _KPRINTF("NOTE " fmt "\n", ## __VA_ARGS__)
#define KINFO(fmt, ...)     _KPRINTF("INFO " fmt "\n", ## __VA_ARGS__)
#define KWARN(fmt, ...)     _KPRINTF("WARN " fmt "\n", ## __VA_ARGS__)
#define KFAIL(fmt, ...)     _KPRINTF("FAIL " fmt "\n", ## __VA_ARGS__)

/* Kprintf wrappers with extra info. */
#define DKOUT(fmt,  ...)    _KPRINTF_INFO(fmt "\n", ## __VA_ARGS__)
#define DKNOTE(fmt, ...)    _KPRINTF_INFO("NOTE " fmt "\n", ## __VA_ARGS__)
#define DKINFO(fmt, ...)    _KPRINTF_INFO("INFO " fmt "\n", ## __VA_ARGS__)
#define DKWARN(fmt, ...)    _KPRINTF_INFO("WARN " fmt "\n", ## __VA_ARGS__)
#define DKFAIL(fmt, ...)    _KPRINTF_INFO("FAIL " fmt "\n", ## __VA_ARGS__)

#if 0
// e.g. log_register_handler(file_handler, sceKernelStdout());
static void file_handler(const char *msg, size_t msglen, void *arg)
{
    SceUID fd = (SceUID)arg;
    sceIoWrite(fd, msg, msglen);
}

static void kprintf_handler(const char *msg, size_t msglen, void *arg)
{
    Kprintf(msg);
}
#endif

typedef struct log_handler_t log_handler_t;
typedef void (*log_handler_cb_t)(const char *msg, size_t msglen, void *arg);
typedef void (*log_flush_cb_t)(void *arg);

struct log_handler_t
{
    log_handler_cb_t cb;
    log_flush_cb_t flush;
    void *arg;
};

#if USE_LOG

int  log_register_handler(log_handler_cb_t cb, log_flush_cb_t flush, void *arg);
int  log_unregister_handler(int index);
int  log_printf(const char *fmt, ...);
void log_flush();

#else /* ! USE_LOG */

#define log_register_handler(cb, arg)   do {} while (0)
#define log_unregister_handler(index)   do {} while (0)
#define log_printf(fmt, ...)            do {} while (0)
#define log_flush()                     do {} while (0)

#endif /* USE_LOG */


#ifdef __cplusplus
}
#endif

#endif /* PSP_UTIL_LOG_H */
