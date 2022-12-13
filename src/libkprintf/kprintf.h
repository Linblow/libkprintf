#ifndef LIBKPRINTF_KPRINTF_H
#define LIBKPRINTF_KPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kprintf_error_t kprintf_error_t;

enum kprintf_error_t
{
    KPRINTF_OK                   =  0,
    KPRINTF_ERR_NOT_STARTED      = -1, /**!< Module is not (yet) started. */
    KPRINTF_ERR_INVALID_ARGUMENT = -2, /**!< Pointer is NULL. */
    KPRINTF_ERR_INVALID_INDEX    = -3, /**!< Invalid handler index. */
    KPRINTF_ERR_MAX_HANDLERS     = -4, /**!< Max number of registered handlers reached. */
    KPRINTF_ERR_NO_HANDLER       = -5, /**!< No handler registered with this index. */
};

/** Pass the message to the next handlers, if any. */
#define KPRINTF_MSG_OK      (0)
/** Remove the message immediately after. */
#define KPRINTF_MSG_STOP    (1)


/**
 * Kprintf handler callback.
 * @param arg    Optional argument passed to kprintf_register_handler().
 * @param msg    Formatted message string (i.e vsnprintf already called). Can be modified for the next handlers.
 * @param pLen   Pointer to the length of the formatted string. Can be modified for the next handlers.
 * @param size   Maximum capacity in bytes of the message buffer. 
 * @param isUser Whether this is a user mode call (sceKernelPrintf) or kernel (Kprintf).
 * @return KPRINTF_MSG_OK to pass the message to the next handlers;
 *         KPRINTF_MSG_STOP to not pass the message to the next handlers
 */
typedef int (*kprintf_handler_t)(void *arg, char *msg, unsigned short *pLen, unsigned short size, int isUser);

/**
 * Check whether the kprintf library is usable. 
 * When a module calls a function imported from another module, 
 * which is not linked yet, the function then returns the error 
 * SCE_KERNEL_ERROR_LIBRARY_NOT_YET_LINKED (0x8002013a).
 * @return 1 module is linked and started;
 *         0 module is linked, but not started (wait and retry);
 *         0x8002013a module is not yet linked (wait and retry)
 */
int kprintf_is_started();

/**
 * Register the default handler.
 * The default handler writes all messages to stdout.
 * stdout output can be seen in psplink's pspsh terminal through USB.
 * This handler can only be registered once.
 * Consecutive calls will return the same index.
 * @return >= 0 on success, handler's index; < 0 error
 */
int kprintf_register_default_handler();

/**
 * Register a new handler.
 * Multiple handlers can be registered with the same or different callback/argument.
 * Each registered handler will be invoked everytime Kprintf / sceKernelPrintf is called.
 * @param cb Handler callback.
 * @param arg Optional argument passed to the callback.
 * @return >= 0 on success, handler's index; < 0 error
 */
int kprintf_register_handler(kprintf_handler_t cb, void *arg);

/**
 * Unregister a handler.
 * @param index Handler's index
 * @return 0 success; < 0 error
 */
int kprintf_unregister_handler(int index);

/**
 * Reinstall the internal Kprintf handler.
 * Useful in case our handler was over-registered by another module (who knows).
 * @return 0 success; < SCE driver error
 */
int kprintf_install();


#ifdef __cplusplus
}
#endif

#endif /* LIBKPRINTF_KPRINTF_H */
