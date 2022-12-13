// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#include <common/psp/types.h>
#include <util/sys.h>

extern int sceUtilityGetSystemParamString(int id, char *buf, unsigned int bufsize);

#if 0
/* With the PSP, some user mode libs are imported as direct-jump calls.
   Whereas in PPSSPP, they are always HLE kernel syscalls. */
int sys_is_emu_portable()
{
   volatile u32 *import = (volatile u32 *)sceKernelIsCpuIntrEnable;
   return import[0] == 0x03e00008 && MIPS_IS_SYCALL(import[1]);
}
#endif

/* Check whether the system is an emulator. 
   Return 1 is an emulator (PPSSPP/JPCSP); 0 is a real PSP */
int sys_is_emu()
{
   /* PPSSPP/JPCSP always return 0 for success (ignore res).
      PSP always returns an error since "emulator:" is not a device. */
   int res;
   int ret = sceIoDevctl("emulator:", 3, &res, 4, NULL, 0);
   return ret == 0;
}

int sys_get_username(char *name, u32 bufsz)
{
   return sceUtilityGetSystemParamString(1, name, bufsz);
}

int sys_exit()
{
   int ret;
#if KERNEL_MODULE
   ret = sceKernelExitVSHVSH(NULL);
#else
   ret = sceKernelExitGame();
#endif
   return ret;
}
