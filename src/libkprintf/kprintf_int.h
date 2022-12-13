#ifndef LIBKPRINTF_KPRINTF_INT_H
#define LIBKPRINTF_KPRINTF_INT_H

#include <common/psp/types.h>
#include <libkprintf/config.h>
#include <libkprintf/kprintf.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Event flags. */
#define EV_FLUSH        (1 << 0)
#define EV_TERMINATE    (1 << 1)
#define EV_STOP_OK      (1 << 2)

#define STATE_STOPPED   (0)
#define STATE_STARTED   (1)
#define STATE_STOPPING  (2)
#define STATE_STARTING  (3)
#define STATE_BROKEN    (4)

typedef struct kprintf_reghandler_t kprintf_reghandler_t;
typedef struct kprintf_msg_t kprintf_msg_t;

struct kprintf_reghandler_t
{
    kprintf_handler_t cb;
    void *arg;
};

/**
 * Compute the maximum capacity in bytes of a message buffer.
 * 1 byte for the msg null-terminator.
 * 1 byte when the default handler appends a newline to the msg.
 * 14 extra bytes for user modifications.
 */
#define KPRINTF_MSG_SIZE(msglen) ((msglen) + 1 + 1 + 14)

/**
 * Compute the exact size of a struct kprintf_msg_t.
 * 12 bytes for the fixed-length members of the struct.
 */
#define SIZEOF_KPRINTF_MSG(msglen) (12 + KPRINTF_MSG_SIZE(msglen))

struct kprintf_msg_t
{
    /** Next message. */
    kprintf_msg_t *next;
    /** Whether this is a user mode call (sceKernelPrintf) or kernel (Kprintf). */
    int isUser;
    /** Current length in bytes of the formatted message string (not counting the null-terminator). */
    u16 len;
    /** Maximum capacity in bytes of the message buffer.  */
    u16 size;
    /** Formatted Kprintf message (variable-size). */
    char msg[];
};

/* Use these macros instead of pspSdkSetK1, as they save memory. */
#define SET_K1(k1_int)          \
    u32 __k1;                   \
__asm__ volatile(               \
    ".set push\n"               \
    ".set noreorder\n"          \
    "move %0, $k1\n"            \
    "li   $k1, %1\n"            \
    ".set pop\n"                \
    : "=r"(__k1)                \
    : "K"(k1_int))

#define RESTORE_K1()            \
__asm__ volatile(               \
    ".set push\n"               \
    ".set noreorder\n"          \
    "move $k1, %0\n"            \
    ".set pop\n"                \
    : : "r"(__k1))

#define KPRINTF(fmt, ...) Kprintf("%s" fmt, "KprintfLibrary: ", ## __VA_ARGS__)

/* globals */
extern volatile int g_state;
extern SceUID g_vplid;
extern SceUID g_evid;
extern SceUID g_semid;
extern SceUID g_thid;
extern int g_default_handler_index;
extern u32 g_handler_count;
extern kprintf_reghandler_t g_handlers[];
extern kprintf_msg_t *g_head;
extern SceUID g_stdout;

/* event.c */
int event_thread(SceSize args, void *argp);

/* handler.c */
void invoke_handlers(kprintf_msg_t *m);
int register_handler(kprintf_handler_t cb, void *arg, int is_default_handler);
int register_default_handler();
int unregister_handler(int index);
int install_kernel_handler();

/* msg.c */
int new_msg(kprintf_msg_t **pM, const char *msg, u32 len, int isUser);
void add_msg(kprintf_msg_t *m);
// int remove_delete_msg(kprintf_msg_t *m);
void flush_messages();


#ifdef __cplusplus
}
#endif

#endif /* LIBKPRINTF_KPRINTF_INT_H */
