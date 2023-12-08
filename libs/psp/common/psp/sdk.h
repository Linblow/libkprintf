#ifndef COMMON_PSP_SDK_H
#define COMMON_PSP_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

//TODO include errors headers

/* uOFW  PSP kernel and (some) user headers. */

/* We include our own error headers. */
// #include <dnas_error.h>
// #include <ms_cm_error.h>
// #include <power_error.h>
// #include <umd_error.h>

#include <common_imp.h>
/* We need to use pspsdk's pspmoduleinfo.h in order to compile (psp-build-exports). */
#include <pspmoduleinfo.h> // pspsdk
#include <pspmoduleexport.h> // uofw

#include <audio.h>
#include <avcodec_audiocodec.h>
#include <clockgen.h>
#include <codec.h>
#include <ctrl.h>
/* display.h is currently missing everything. */
// #include <display.h>
#include <dmacman.h>
#include <exceptionman.h>
#include <ge_kernel.h> // already includes ge_user.h
#include <idstorage.h>
#include <init.h>
#include <interruptman.h>
#include <iofilemgr_kernel.h>
#include <iofilemgr_stdio.h>
#include <led.h>
#include <lfatfs.h>
#include <libaac.h>
#include <loadcore.h>
#include <loadexec_kernel.h>
#include <loadexec_user.h>
#include <lowio_ddr.h>
#include <lowio_gpio.h>
#include <lowio_i2c.h>
#include <lowio_nand.h>
#include <lowio_sysreg.h>
#include <mediaman.h>
#include <mediaman_kernel.h>
#include <mediaman_user.h>
#include <memlmd.h>
#include <mesgled.h>
#include <modulemgr_init.h>
#include <modulemgr_kernel.h>
#include <modulemgr_moduleInfo.h>
#include <modulemgr_nids.h>
#include <modulemgr_options.h>
#include <modulemgr_user.h>
#include <openpsid_ddrdb.h>
#include <syscon.h>
#include <sysmem_common.h>
#include <sysmem_kdebug.h>
#include <sysmem_kernel.h>
#include <sysmem_suspend_kernel.h>
#include <sysmem_sysclib.h>
#include <sysmem_sysevent.h>
#include <sysmem_user.h>
#include <sysmem_utils_kernel.h>
#include <systimer.h>
#include <threadman_kernel.h>
#include <threadman_user.h>
#include <usbbus.h>
#include <usersystemlib_kernel.h>

/* PSPSDK headers which are missing or incomplete in uOFW.
   Note these don't include any other headers (ie. no psptypes.h),
   and are therefore "compatible" with uOFW's. */
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspopenpsid.h>

/* Missing defintions in both uOFW and pspsdk: */

/**
 * This function specifies whether or not, when sceKernelLoadExec() is next executed, to 
 * transition to the next application with the currently displayed frame buffer held as is. By 
 * holding the frame buffer as is, the black-out time can be shortened. In order to reboot with 
 * the frame buffer held as is, the frame buffer must be positioned at the beginning of the 
 * Graphics Engine eDRAM, and the width must be defined to be 512 pixels.
 * @param mode Mode for holding frame buffer during reboot: 
 *             0 = black out during reboot (default)
 *             1 = hold currently displayed frame buffer during reboot
 * @return int 
 */
int sceDisplaySetHoldMode(int mode);


#ifdef __cplusplus
}
#endif

#endif /* COMMON_PSP_SDK_H */
