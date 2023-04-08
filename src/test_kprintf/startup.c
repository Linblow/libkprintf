#include <common/psp/types.h>
#include <test_kprintf/config.h>

#define MOD_ATTR                        \
    SCE_MODULE_KERNEL               |   \
    SCE_MODULE_ATTR_EXCLUSIVE_START |   \
    SCE_MODULE_ATTR_EXCLUSIVE_LOAD

SCE_MODULE_INFO(MOD_NAME, MOD_ATTR, MOD_VER_MAJOR, MOD_VER_MINOR);

SCE_MODULE_BOOTSTART("_start");
SCE_MODULE_STOP("_stop");

// PSP_DISABLE_NEWLIB();
// PSP_DISABLE_AUTOSTART_PTHREAD();

extern int _main(SceSize args, void *argp);

EXPORT_LTO int _start(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    int ret;
    SceUID thid;
    thid = sceKernelCreateThread(MOD_NAME "_th", (SceKernelThreadEntry)_main, 16, 2*1024, 0, NULL);
    if (thid < 0)
        return 1;
    ret = sceKernelStartThread(thid, 0, NULL);
    return ret == 0 ? 0 : 1;
}

EXPORT_LTO int _stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    return 0;
}

#if 1
/* This ugly hack is required so that LTO doesn't remove the exports in the final exec. */
extern const struct SceLibraryEntry __library_exports[];
__attribute__((used)) int __psp_exports_dummy_func() {return (int)&__library_exports; }
#endif
