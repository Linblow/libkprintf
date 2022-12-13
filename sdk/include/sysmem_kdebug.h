/* Copyright (C) 2011, 2012 The uOFW team
   See the file COPYING for copying permission.
*/

#include "common_header.h"
#include <stdarg.h>

#define SCE_DECI2OP_GE_SETOPS   6
#define SCE_DECI2OP_GE_BREAK    7
#define SCE_DECI2OP_GE_PUT_BP   8

typedef struct {   
    int size;
    int (*ops[])();
} SceKernelDeci2Ops;

int sceKernelDeci2pRegisterOperations(SceKernelDeci2Ops *ops);
SceKernelDeci2Ops *sceKernelDeci2pReferOperations(void);

int sceKernelSm1RegisterOperations(void *arg);
void *sceKernelSm1ReferOperations();

int Kprintf(const char *fmt, ...);
int sceKernelPrintf(const char *fmt, ...);

int sceKernelDipsw(u32 reg);
u32 sceKernelDipswAll();
u32 sceKernelDipswLow32();
u32 sceKernelDipswHigh32();
int sceKernelDipswSet(u32 reg);
int sceKernelDipswClear(u32 reg);
int sceKernelDipswCpTime(void);

int sceKernelIsToolMode(void);
int sceKernelIsDevelopmentToolMode(void);
int sceKernelIsUMDMode(void);
int sceKernelIsDVDMode(void);

typedef int (*SceKernelDebugWriteFunc)(SceUID fd, const void *data, SceSize size);
typedef int (*SceKernelDebugReadFunc)(SceUID fd,void *data, SceSize size);

int sceKernelRegisterDebugWrite(SceKernelDebugWriteFunc func);
int sceKernelRegisterDebugRead(SceKernelDebugReadFunc func);
int sceKernelDebugEchoSet(int echo);

int sceKernelDebugWrite(SceUID fd, const void *data, SceSize size);
int sceKernelDebugRead(SceUID fd, void *data, SceSize size);
int sceKernelDebugEcho(void);

void * sceKernelGetDebugPutchar(void);
void sceKernelRegisterDebugPutchar(void (*func)(short*, int));
void sceKernelRegisterDebugPutcharByBootloader(void (*func)(short*, int));

/**
 * Kprintf handler callback.
 * @param arg Optional argument passed to sceKernelRegisterKprintfHandler().
 * @param fmt Printf-like format string.
 * @param va Variable arguments list.
 * @param isUser Whether this is a user mode call (sceKernelPrintf) or kernel (Kprintf).
 * @return >= 0 on success, the total number of characters is returned.
 *         < 0 on failure, a negative number (error code).
 *         Either way, this value is returned by Kprintf() / sceKernelPrintf().
 */
typedef int (*SceKernelKprintfHandler)(void *arg, const char *fmt, va_list va, int isUser);

void sceKernelRegisterKprintfHandler(SceKernelKprintfHandler handler, void *arg);

void sceKernelRegisterAssertHandler(void (*func)(int));
void sceKernelAssert(int test, int lvl);

