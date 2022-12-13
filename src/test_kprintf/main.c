#include <common/psp/types.h>
#include <util/sys.h>

/* Calls Kprintf() every second. */
int _main(SceSize args, void *argp)
{
    u32 i;
    char username[128] = {0};

    sys_get_username(username, sizeof(username));
    i = 0;
    while (1)
    {
        // Kprintf("Hello %s %d from Kprintf!", username, ++i);
        Kprintf("Hello %s %d from Kprintf!\n", username, ++i);
        sceKernelDelayThread(1*1000*1000);
    }

    return 0;
}
