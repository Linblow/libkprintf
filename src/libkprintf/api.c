#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>

EXPORT_LTO int kprintf_is_started()
{
    /* No need to lock with intr/sema */
    return g_state == STATE_STARTED;
}

EXPORT_LTO int kprintf_register_default_handler()
{
    int ret;
    SET_K1(0);
    ret = register_default_handler();
    RESTORE_K1();
    return ret;
}

EXPORT_LTO int kprintf_register_handler(kprintf_handler_t cb, void *arg)
{
    int ret;
    SET_K1(0);
    ret = register_handler(cb, arg, 0);
    RESTORE_K1();
    return ret;
}

EXPORT_LTO int kprintf_unregister_handler(int index)
{
    int ret;
    SET_K1(0);
    ret = unregister_handler(index);
    RESTORE_K1();
    return ret;
}

EXPORT_LTO int kprintf_install()
{
    int ret;
    SET_K1(0);
    ret = install_kernel_handler();
    RESTORE_K1();
    return ret;
}

