#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <common/psp/sdk.h>

/* LTO removes symbols that are seemingly unused, typically exported functions/vars.
   This attribute prevents that.  */
#define EXPORT_LTO              __attribute__((__used__))


#if 0

//TODO include uOFW kernel headers
//TODO only include pspsdk user headers
//TODO move pspsdk missing user functions to local new headers

#include <pspkernel.h>

#include <pspctrl.h>
#include <pspctrl_kernel.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspge.h>
#include <psploadexec_kernel.h>
#include <pspopenpsid.h>
#include <psprtc.h>
#include <pspsdk.h>
#include <pspsysclib.h>
#include <pspsysmem_kernel.h>
#include <pspumd.h>

/* Some basic PSP kernel functions are missing in PSPSDK headers. */

extern int sceDisplaySetHoldMode(int mode);
// extern int sceKernelSetCompiledSdkVersion(unsigned int version);
extern int sceKernelSetCompilerVersion(unsigned int version);
extern int sceKernelStopUnloadSelfModuleWithStatus(
    int         exitStatus,
    SceSize     args, 
    const void *argp, 
    int        *modresult,
    const SceKernelSMOption *pOpt
);

#endif 

#ifdef __cplusplus
}
#endif

#endif /* COMMON_TYPES_H */
