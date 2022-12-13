/* Copyright (C) 2011 - 2015 The uOFW team
   See the file COPYING for copying permission.
*/

#ifndef THREADMAN_KERNEL_H
#define	THREADMAN_KERNEL_H

#include "common_header.h"
#include "threadman_user.h"

typedef struct {
	SceUInt32   low;
	SceUInt32   hi;
} SceKernelSysClock;

/* Threads */

typedef s32 (*SceKernelThreadEntry)(SceSize args, void *argp);

typedef s32 (*SceKernelRebootKernelThreadEntry)(s32 arg1, u32 arg2, s32 arg3, s32 arg4);

typedef struct {
    SceSize     size;
    SceUID      stackMpid;
} SceKernelThreadOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         status;
    SceKernelThreadEntry entry;
    void        *stack;
    int         stackSize;
    void        *gpReg;
    int         initPriority;
    int         currentPriority;
    int         waitType;
    SceUID      waitId;
    int         wakeupCount;
    int         exitStatus;
    SceKernelSysClock runClocks;
    SceUInt     intrPreemptCount;
    SceUInt     threadPreemptCount;
    SceUInt     releaseCount;
    int         notifycallback;
} SceKernelThreadInfo;

typedef struct {
    SceSize     size;
    int         status;
    int         currentPriority;
    int         waitType;
    int         waitId;
    int         wakeupCount;
    SceKernelSysClock runClocks;
    SceUInt     intrPreemptCount;
    SceUInt     threadPreemptCount;
    SceUInt     releaseCount;
    int         notifycallback;
} SceKernelThreadRunStatus;

/** 
 * thread priority - lower numbers mean higher priority 
 */
#define SCE_KERNEL_INVALID_PRIORITY             (0)
#define SCE_KERNEL_HIGHEST_PRIORITY_KERNEL      (1)
#define SCE_KERNEL_HIGHEST_PRIORITY_USER        (16)
#define SCE_KERNEL_MODULE_INIT_PRIORITY         (32)
#define SCE_KERNEL_LOWEST_PRIORITY_USER         (111)
#define SCE_KERNEL_LOWEST_PRIORITY_KERNEL       (126)

/* thread size */
#define SCE_KERNEL_TH_KERNEL_DEFAULT_STACKSIZE  (4 * 1024) /* 4 KB */
#define SCE_KERNEL_TH_USER_DEFAULT_STACKSIZE    (256 * 1024) /* 256 KB */

/* SceKernelThreadInfo.status */
#define SCE_KERNEL_THS_RUN              (0x01)
#define SCE_KERNEL_THS_READY            (0x02)
#define SCE_KERNEL_THS_WAIT             (0x04)
#define SCE_KERNEL_THS_SUSPEND          (0x08)
#define SCE_KERNEL_THS_WAITSUSPEND      (0x0c)
#define SCE_KERNEL_THS_DORMANT          (0x10)
#define SCE_KERNEL_THS_DEAD             (0x20)

/* SceKernelThreadInfo.waitType */
#define SCE_KERNEL_TSW_SLEEP            (1)
#define SCE_KERNEL_TSW_DELAY            (2)
#define SCE_KERNEL_TSW_SEMA             (3)
#define SCE_KERNEL_TSW_EVENTFLAG        (4)
#define SCE_KERNEL_TSW_MBX              (5)
#define SCE_KERNEL_TSW_VPL              (6)
#define SCE_KERNEL_TSW_FPL              (7)
#define SCE_KERNEL_TSW_MPP              (8)
#define SCE_KERNEL_TSW_WAITTHEND        (9)
#define SCE_KERNEL_TSW_RELTHEVH         (10)
#define SCE_KERNEL_TSW_DELCALLBACK      (11)
#define SCE_KERNEL_TSW_MUTEX            (12)
#define SCE_KERNEL_TSW_LWMUTEX          (13)
#define SCE_KERNEL_TSW_TLSPL            (14)
#define SCE_KERNEL_TSW_SLEEP_CB         (0x101)
#define SCE_KERNEL_TSW_DELAY_CB         (0x102)
#define SCE_KERNEL_TSW_SEMA_CB          (0x103)
#define SCE_KERNEL_TSW_EVENTFLAG_CB     (0x104)
#define SCE_KERNEL_TSW_MBX_CB           (0x105)
#define SCE_KERNEL_TSW_VPL_CB           (0x106)
#define SCE_KERNEL_TSW_FPL_CB           (0x107)
#define SCE_KERNEL_TSW_MPP_CB           (0x108)
#define SCE_KERNEL_TSW_WAITTHEND_CB     (0x109)
#define SCE_KERNEL_TSW_MUTEX_CB         (0x10c)
#define SCE_KERNEL_TSW_LWMUTEX_CB       (0x10d)

/* thread attributes / SceKernelThreadInfo.attr */
#define SCE_KERNEL_TH_DEFAULT_ATTR              (0)
#define SCE_KERNEL_TH_VSH_MODE                  (0xC0000000u) /* Thread runs in VSH mode. */
#define SCE_KERNEL_TH_APP_MODE                  (0xB0000000u) /* Thread runs in Application mode. */
#define SCE_KERNEL_TH_USB_WLAN_MODE             (0xA0000000u) /* Thread runs in USB_WLAN mode. */
#define SCE_KERNEL_TH_MS_MODE                   (0x90000000u) /* Thread runs in MS mode. */
#define SCE_KERNEL_TH_USER_MODE                 (0x80000000u) /* Thread runs in User mode. */
#define SCE_KERNEL_TH_NO_FILLSTACK              (0x00100000u)
#define SCE_KERNEL_TH_CLEAR_STACK               (0x00200000u) /* Specifies that thread memory area should be cleared to 0 when deleted. */
#define SCE_KERNEL_TH_LOW_STACK                 (0x00400000u) /* Specifies that the stack area is allocated from the lower addresses in memory, not the higher ones. */
#define SCE_KERNEL_TH_UNK_800000                (0x00800000u)
#define SCE_KERNEL_TH_USE_VFPU                  (0x00004000u) /* Specifies that the VFPU is available. */
#define SCE_KERNEL_TH_NEVERUSE_FPU              (0x00002000u)


#define SCE_KERNEL_THREAD_ID_SELF               (0) /* UID representing the calling thread. */

#define SCE_KERNEL_AT_THFIFO                    (0x00000000) /* Waiting threads are queued on a FIFO basis. */
#define SCE_KERNEL_AT_THPRI                     (0x00000100) /* Waiting threads are queued based on priority. */
#define SCE_KERNEL_AT_SINGLE                    (0x00000000)
#define SCE_KERNEL_AT_MULTI                     (0x00000200)
#define SCE_KERNEL_AT_MSFIFO                    (0x00000000)
#define SCE_KERNEL_AT_MSPRI                     (0x00000400)

SceUID sceKernelCreateThread(const char *name, SceKernelThreadEntry entry, s32 initPriority,
                             SceSize stackSize, SceUInt attr, SceKernelThreadOptParam *option);
int sceKernelDeleteThread(SceUID thid);
int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp);
int sceKernelSleepThread(void);
int sceKernelSleepThreadCB(void);
int sceKernelWakeupThread(SceUID thid);
int sceKernelCancelWakeupThread(SceUID thid);
int sceKernelDonateWakeupThread(SceUID thid);
int sceKernelSuspendThread(SceUID thid);
int sceKernelResumeThread(SceUID thid);
int sceKernelExitThread(s32 status);
s32 sceKernelExitDeleteThread(s32 exitStatus);
int sceKernelTerminateThread(SceUID thid);
int sceKernelTerminateDeleteThread(SceUID thid);
int sceKernelSuspendDispatchThread(void);
int sceKernelResumeDispatchThread(int olddisp);
int sceKernelChangeCurrentThreadAttr(SceUInt clearAttr, SceUInt setAttr);
int sceKernelChangeThreadPriority(SceUID thid, int priority);
int sceKernelRotateThreadReadyQueue(int priority);
int sceKernelGetThreadCurrentPriority(void);
int sceKernelGetThreadExitStatus(SceUID thid);
s32 sceKernelGetThreadId(void);
int sceKernelIsUserModeThread(void);
int sceKernelWaitThreadEnd(SceUID thid, SceUInt *timeout);
int sceKernelWaitThreadEndCB(SceUID thid, SceUInt *timeout);
int sceKernelReleaseWaitThread(SceUID thid);
int sceKernelSuspendAllUserThreads(void);
int sceKernelReferThreadStatus(SceUID thid, SceKernelThreadInfo *info);
int sceKernelReferThreadRunStatus(SceUID thid, SceKernelThreadRunStatus *runstat);
int sceKernelExtendThreadStack(SceSize stackSize, int (*func)(void *), void *common);
// int sceKernelCheckThreadStack(void);
int sceKernelGetThreadStackFreeSize(SceUID thid);

int sceKernelGetSystemTime(SceKernelSysClock *clk);
u64 sceKernelGetSystemTimeWide(void);
u32 sceKernelGetSystemTimeLow(void);

int sceKernelDelayThread(SceUInt usec);
int sceKernelDelayThreadCB(SceUInt usec);
int sceKernelDelaySysClockThread(const SceKernelSysClock *clk);
int sceKernelDelaySysClockThreadCB(const SceKernelSysClock *clk);

int sceKernelUSec2SysClock(SceUInt usec, SceKernelSysClock *clk);
int sceKernelSysClock2USec(const SceKernelSysClock *clk, int *sec, int *usec);
u64 sceKernelUSec2SysClockWide(SceUInt usec);
int sceKernelSysClock2USecWide(SceUInt64 clk, int *sec, int *usec);

enum SceUserLevel {
    SCE_USER_LEVEL_MS       = 1,
    SCE_USER_LEVEL_USBWLAN  = 2,
    SCE_USER_LEVEL_APP      = 3,
    SCE_USER_LEVEL_VSH      = 4,
};
int sceKernelGetUserLevel(void);

/* System status */
typedef struct SceKernelSystemStatus {
    SceSize size;
    SceUInt status;
    SceKernelSysClock idleClocks;
    SceUInt comesOutOfIdleCount;
    SceUInt threadSwitchCount;
    SceUInt vfpuSwitchCount;
} SceKernelSystemStatus;

/* SceKernelSystemStatus->status */
#define SCE_KERNEL_TSS_THREAD           (0)
#define SCE_KERNEL_TSS_DISABLEDISPATCH  (1)
#define SCE_KERNEL_TSS_DISABLEINTR      (3)
#define SCE_KERNEL_TSS_NOTHREAD         (4)

int sceKernelReferSystemStatus(SceKernelSystemStatus *info);

typedef enum {
    SCE_KERNEL_TMID_Thread              = 1,
    SCE_KERNEL_TMID_Semaphore           = 2,
    SCE_KERNEL_TMID_EventFlag           = 3,
    SCE_KERNEL_TMID_Mbox                = 4,
    SCE_KERNEL_TMID_Vpl                 = 5,
    SCE_KERNEL_TMID_Fpl                 = 6,
    SCE_KERNEL_TMID_Mpipe               = 7,
    SCE_KERNEL_TMID_Callback            = 8,
    SCE_KERNEL_TMID_ThreadEventHandler  = 9,
    SCE_KERNEL_TMID_Alarm               = 10,
    SCE_KERNEL_TMID_VTimer              = 11,
    SCE_KERNEL_TMID_SleepThread         = 64,
    SCE_KERNEL_TMID_DelayThread         = 65,
    SCE_KERNEL_TMID_SuspendThread       = 66,
    SCE_KERNEL_TMID_DormantThread       = 67,
} SceKernelIdListType;

int sceKernelGetThreadmanIdList(int type, SceUID *readbuf, int readbufsize, int *idcount);
SceKernelIdListType sceKernelGetThreadmanIdType(SceUID uid);

/* Mutexes */

typedef struct {
    SceSize     size;
} SceKernelMutexOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         initCount;
    int         currentCount;
    SceUID      currentOwner;
    int         numWaitThreads;
} SceKernelMutexInfo;

/* Mutex attributes */
#define SCE_KERNEL_MUTEX_ATTR_TH_FIFO       (SCE_KERNEL_AT_THFIFO)
#define SCE_KERNEL_MUTEX_ATTR_TH_PRI        (SCE_KERNEL_AT_THPRI)
#define SCE_KERNEL_MUTEX_ATTR_RECURSIVE     (0x0200u) /*Allow recursive locks by threads that own the mutex. */

SceUID sceKernelCreateMutex(char *name, SceUInt attr, int initCount, const SceKernelMutexOptParam *opt);
int sceKernelDeleteMutex(SceUID uid);
int sceKernelLockMutex(SceUID uid, int lockCount, SceUInt *pTimeout);
int sceKernelLockMutexCB(SceUID uid, int lockCount, SceUInt *pTimeout);
int sceKernelTryLockMutex(SceUID uid, int lockCount);
int sceKernelUnlockMutex(SceUID uid, int unlockCount);
int sceKernelCancelMutex(SceUID uid, int newLockCount, int *pNumWaitThreads);
int sceKernelReferMutexStatus(SceUID uid, SceKernelMutexInfo *pInfo);

/* Light-weight mutexes */

typedef struct {
    u32         unkn[8];
} SceKernelLwMutexWork;

typedef struct {
    SceSize     size;
} SceKernelLwMutexOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    SceUID      uid;
    SceKernelLwMutexWork *branch;
    int         initCount;
    int         currentCount;
    SceUID      currentOwner;
    int         numWaitThreads;
} SceKernelLwMutexInfo;

#define SCE_KERNEL_LWMA_THFIFO      (SCE_KERNEL_AT_THFIFO)
#define SCE_KERNEL_LWMA_THPRI       (SCE_KERNEL_AT_THPRI)
#define SCE_KERNEL_LWMA_RECURSIVE   (0x0200u)

/* NOTE: some of these are already defined in usersystemlib_kkernel.h / threadman_user.h.
   They probably should be defined here instead. */
int sceKernelCreateLwMutex(SceKernelLwMutexWork *work, const char *name, SceUInt attr, int initCount, const SceKernelLwMutexOptParam *opt);
int sceKernelDeleteLwMutex(SceKernelLwMutexWork *work);
// int sceKernelLockLwMutex(SceKernelLwMutexWork *work, int lockcount, SceUInt *timeout);
// int sceKernelLockLwMutexCB(SceKernelLwMutexWork *work, int lockcount, SceUInt *timeout);
// int sceKernelTryLockLwMutex(SceKernelLwMutexWork *work, int lockcount);
// int sceKernelUnlockLwMutex(SceKernelLwMutexWork *work, int unlockcount);
// int sceKernelReferLwMutexStatus(SceKernelLwMutexWork *work, SceKernelLwMutexInfo *info);
// int sceKernelReferLwMutexStatusByID(SceUID uid, SceKernelLwMutexInfo *info);

/* Event flags */

typedef struct {
    SceSize     size;
} SceKernelEventFlagOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    SceUInt     initPattern;
    SceUInt     currentPattern;
    int         numWaitThreads;
} SceKernelEventFlagInfo;

/* Event flag attributes. */
#define SCE_KERNEL_EA_SINGLE            (SCE_KERNEL_AT_SINGLE) /** Multiple thread waits are prohibited. */ 
#define SCE_KERNEL_EA_MULTI             (SCE_KERNEL_AT_MULTI)  /** Multiple thread waits are permitted. */

/* Event flag wait modes. */
#define SCE_KERNEL_EW_AND               (0x00u)      /** Wait for all bits in the bit pattern to be set. */
#define SCE_KERNEL_EW_OR                (0x01u)      /** Wait for one or more bits in the bit pattern to be set. */
#define SCE_KERNEL_EW_CLEAR_ALL         (0x10u)      /** Clear all bits after wait condition is satisfied. */
#define SCE_KERNEL_EW_CLEAR_PAT         (0x20u)      /** Clear bits specified by bit pattern after wait condition is satisfied. */
#define SCE_KERNEL_EW_CLEAR             (SCE_KERNEL_EW_CLEAR_ALL)

SceUID sceKernelCreateEventFlag(const char *name, SceUInt attr, SceUInt initPattern, const SceKernelEventFlagOptParam *opt);
int sceKernelSetEventFlag(SceUID evid, u32 bitpattern);
int sceKernelClearEventFlag(SceUID evid, u32 bitpattern);
int sceKernelPollEventFlag(int evid, u32 bitpattern, int waitmode, u32 *resultpat);
int sceKernelWaitEventFlag(int evid, u32 bitpattern, int waitmode, u32 *resultpat, SceUInt *timeout);
int sceKernelWaitEventFlagCB(int evid, u32 bitpattern, int waitmode, u32 *resultpat, SceUInt *timeout);
int sceKernelCancelEventFlag(SceUID evid, SceUInt setpattern, s32 *numWaitThreads);
int sceKernelDeleteEventFlag(int evid);
int sceKernelReferEventFlagStatus(SceUID event, SceKernelEventFlagInfo *status);

/* Message box */

typedef struct {
    SceSize     size;
} SceKernelMbxOptParam;

typedef struct SceKernelMsgPacket {
    struct SceKernelMsgPacket *next;
    u8          msgPriority;
    u8          dummy[3];
} SceKernelMsgPacket;

typedef struct SceKernelMbxInfo {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         numWaitThreads;
    int         numMessage;
    SceKernelMsgPacket *topPacket;
} SceKernelMbxInfo;

#define SCE_KERNEL_MBA_THFIFO   SCE_KERNEL_AT_THFIFO
#define SCE_KERNEL_MBA_THPRI    SCE_KERNEL_AT_THPRI
#define SCE_KERNEL_MBA_MSFIFO   SCE_KERNEL_AT_MSFIFO
#define SCE_KERNEL_MBA_MSPRI    SCE_KERNEL_AT_MSPRI

SceUID sceKernelCreateMbx(const char *name, SceUInt attr, const SceKernelMbxOptParam *opt);
int sceKernelDeleteMbx(SceUID uid);
int sceKernelSendMbx(SceUID uid, SceKernelMsgPacket *sendmsg);
int sceKernelReceiveMbx(SceUID uid, SceKernelMsgPacket **recvmsg, SceUInt *timeout);
int sceKernelReceiveMbxCB(SceUID uid, SceKernelMsgPacket **recvmsg, SceUInt *timeout);
int sceKernelPollMbx(SceUID uid, SceKernelMsgPacket **recvmsg);
int sceKernelCancelReceiveMbx(SceUID uid, int *numWaitThreads);
int sceKernelReferMbxStatus(SceUID uid, SceKernelMbxInfo *info);

/* Message pipe */

typedef struct {
    SceSize     size;
} SceKernelMppOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         bufSize;
    int         freeSize;
    int         numSendWaitThreads;
    int         numReceiveWaitThreads;
} SceKernelMppInfo;

#define SCE_KERNEL_MPA_THFIFO_S (0x0000u)
#define SCE_KERNEL_MPA_THPRI_S  (0x0100u)
#define SCE_KERNEL_MPA_THFIFO_R (0x0000u)
#define SCE_KERNEL_MPA_THPRI_R  (0x1000u)
#define SCE_KERNEL_MPA_THFIFO   (SCE_KERNEL_MPA_THFIFO_S | SCE_KERNEL_MPA_THFIFO_R)
#define SCE_KERNEL_MPA_THPRI    (SCE_KERNEL_MPA_THPRI_S | SCE_KERNEL_MPA_THPRI_R)
#define SCE_KERNEL_MPA_MEMBTM   (0x4000u)

#define SCE_KERNEL_MPW_FULL     (0u)
#define SCE_KERNEL_MPW_ASAP     (1u)

SceUID sceKernelCreateMsgPipe(const char *name, SceUID mpid, SceUInt attr, SceSize bufsize, const SceKernelMppOptParam *opt);
int sceKernelDeleteMsgPipe(SceUID uid);
int sceKernelSendMsgPipe(SceUID uid, const void *sendbuf, SceSize sendsize, int waitmode, SceSize *result, SceUInt *timeout);
int sceKernelSendMsgPipeCB(SceUID uid, const void *sendbuf, SceSize sendsize, int waitmode, SceSize *result, SceUInt *timeout);
int sceKernelTrySendMsgPipe(SceUID uid, const void *sendbuf, SceSize sendsize, int waitmode, SceSize *result);
int sceKernelReceiveMsgPipe(SceUID uid, void *recvbuf, SceSize recvsize, int waitmode, SceSize *result, SceUInt *timeout);
int sceKernelReceiveMsgPipeCB(SceUID uid, void *recvbuf, SceSize recvsize, int waitmode, SceSize *result, SceUInt *timeout);
int sceKernelTryReceiveMsgPipe(SceUID uid, void *recvbuf, SceSize recvsize, int waitmode, SceSize *result);
int sceKernelCancelMsgPipe(SceUID uid, int *numSendWaitThreads, int *numReceiveWaitThreads);
int sceKernelReferMsgPipeStatus(SceUID uid, SceKernelMppInfo *info);

/* Semaphores */

typedef struct {
    SceSize     size;
} SceKernelSemaOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         initCount;
    int         currentCount;
    int         maxCount;
    int         numWaitThreads;
} SceKernelSemaInfo;

#define SCE_KERNEL_SA_THFIFO    (SCE_KERNEL_AT_THFIFO) /* A FIFO queue is used for the waiting thread */
#define SCE_KERNEL_SA_THPRI     (SCE_KERNEL_AT_THPRI)  /* The waiting thread is queued by its thread priority */

SceUID sceKernelCreateSema(const char *name, SceUInt attr, int initCount, int maxCount, const SceKernelSemaOptParam *opt);
int sceKernelDeleteSema(SceUID uid);
int sceKernelSignalSema(SceUID uid, int signalCount);
int sceKernelWaitSema(SceUID uid, int needCount, SceUInt *timeout);
int sceKernelWaitSemaCB(SceUID uid, int needCount, SceUInt *timeout);
int sceKernelPollSema(SceUID uid, int needCount);
int sceKernelCancelSema(SceUID uid, int setCount, int *numWaitThreads);
int sceKernelReferSemaStatus(SceUID uid, SceKernelSemaInfo *info);

/* Alarms. */

typedef SceUInt (*SceKernelAlarmHandler)(void *common);

typedef struct {
	SceSize size;
	SceKernelSysClock schedule;
	SceKernelAlarmHandler handler;
	void *common;
} SceKernelAlarmInfo;

SceUID sceKernelSetAlarm(SceUInt clock, SceKernelAlarmHandler handler, void *common);
SceUID sceKernelSetSysClockAlarm(SceKernelSysClock *clock, SceKernelAlarmHandler handler, void *common);
int sceKernelCancelAlarm(SceUID alarmid);
int sceKernelReferAlarmStatus(SceUID alarmid, SceKernelAlarmInfo *info);

/* Virtual timer */

typedef SceUInt (*SceKernelVTimerHandler)(SceUID vtid, SceKernelSysClock *schedule, SceKernelSysClock *actual, void *common);
typedef SceUInt (*SceKernelVTimerHandlerWide)(SceUID vtid, SceUInt64 *schedule, SceUInt64 *actual, void *common);

typedef struct {
    SceSize     size;
} SceKernelVTimerOptParam;

typedef struct {
    SceSize                 size;
    char                    name[SCE_UID_NAME_LEN + 1];
    int                     active;
    SceKernelSysClock       base;
    SceKernelSysClock       current;
    SceKernelSysClock       schedule;
    SceKernelVTimerHandler  handler;
    void                   *common;
} SceKernelVTimerInfo;

SceUID sceKernelCreateVTimer(const char *name, const SceKernelVTimerOptParam *opt);
int sceKernelDeleteVTimer(SceUID uid);
int sceKernelGetVTimerBase(SceUID uid, SceKernelSysClock *base);
int sceKernelGetVTimerTime(SceUID uid, SceKernelSysClock *clk);
int sceKernelSetVTimerTime(SceUID uid, SceKernelSysClock *clk);
int sceKernelStartVTimer(SceUID uid);
int sceKernelStopVTimer(SceUID uid);
int sceKernelSetVTimerHandler(SceUID uid, SceKernelSysClock *schedule, SceKernelVTimerHandler handler, void *common);
int sceKernelCancelVTimerHandler(SceUID uid);
u64 sceKernelGetVTimerBaseWide(SceUID uid);
u64 sceKernelGetVTimerTimeWide(SceUID uid);
u64 sceKernelSetVTimerTimeWide(SceUID uid, SceUInt64 clk);
int sceKernelSetVTimerHandlerWide(SceUID uid, SceUInt64 schedule, SceKernelVTimerHandlerWide handler, void *common);
int sceKernelReferVTimerStatus(SceUID uid, SceKernelVTimerInfo *info);

/* Callbacks */

typedef int (*SceKernelCallbackFunction)(int count, int arg, void *common);

typedef struct {
    SceSize size;
    char name[SCE_UID_NAME_LEN + 1];
    SceUID threadId;
    SceKernelCallbackFunction callback;
    void *common;
    s32 notifyCount;
    s32 notifyArg;
} SceKernelCallbackInfo;

SceUID sceKernelCreateCallback(const char *name, SceKernelCallbackFunction callback, void *common);
int sceKernelDeleteCallback(SceUID cbid);
int sceKernelNotifyCallback(SceUID cbid, int arg);
int sceKernelCancelCallback(SceUID cbid);
int sceKernelGetCallbackCount(SceUID cbid);
int sceKernelCheckCallback(void);
int sceKernelReferCallbackStatus(SceUID cbid, SceKernelCallbackInfo *info);

/* VPL: Variable-size memory pool functions */

typedef struct {
    SceSize 	size;
} SceKernelVplOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         poolSize;
    int         freeSize;
    int         numWaitThreads;
} SceKernelVplInfo;

#define SCE_KERNEL_VA_THFIFO    SCE_KERNEL_AT_THFIFO
#define SCE_KERNEL_VA_THPRI     SCE_KERNEL_AT_THPRI
#define SCE_KERNEL_VA_THPASS    (0x0200u)
#define SCE_KERNEL_VA_MEMBTM    (0x4000u)

SceUID sceKernelCreateVpl(const char *name, SceUID mpid, SceUInt attr, SceSize size, SceKernelVplOptParam *opt);
int sceKernelDeleteVpl(SceUID uid);
int sceKernelAllocateVpl(SceUID uid, SceSize size, void **data, SceUInt *timeout);
int sceKernelAllocateVplCB(SceUID uid, SceSize size, void **data, SceUInt *timeout);
int sceKernelTryAllocateVpl(SceUID uid, SceSize size, void **data);
int sceKernelFreeVpl(SceUID uid, void *data);
int sceKernelCancelVpl(SceUID uid, int *numWaitThreads);
int sceKernelReferVplStatus(SceUID uid, SceKernelVplInfo *info);

/* FPL: Fixed-size memory pool functions. */

typedef struct {
    SceSize     size;
    SceSize     alignment;
} SceKernelFplOptParam;

typedef struct {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         blockSize;
    int         numBlocks;
    int         freeBlocks;
    int         numWaitThreads;
} SceKernelFplInfo;

#define SCE_KERNEL_FA_THFIFO    SCE_KERNEL_AT_THFIFO
#define SCE_KERNEL_FA_THPRI     SCE_KERNEL_AT_THPRI
#define SCE_KERNEL_FA_MEMBTM    (0x4000u)

int sceKernelDeleteFpl(SceUID uid);
int sceKernelAllocateFpl(SceUID uid, void **data, SceUInt *timeout);
int sceKernelAllocateFplCB(SceUID uid, void **data, SceUInt *timeout);
int sceKernelTryAllocateFpl(SceUID uid, void **data);
int sceKernelFreeFpl(SceUID uid, void *data);
int sceKernelCancelFpl(SceUID uid, int *numWaitThreads);
int sceKernelReferFplStatus(SceUID uid, SceKernelFplInfo *info);

/* User Thread Local Storage (TLS) memory pool */

typedef struct SceKernelTlsplOptParam {
    SceSize     size;
    SceSize     alignment;
} SceKernelTlsplOptParam;

typedef struct SceKernelTlsplInfo {
    SceSize     size;
    char        name[SCE_UID_NAME_LEN + 1];
    SceUInt     attr;
    int         idx;
    int         blockSize;
    int         numBlocks;
    int         freeBlocks;
    int         numWaitThreads;
} SceKernelTlsplInfo;

#define SCE_KERNEL_TA_THFIFO    SCE_KERNEL_AT_THFIFO
#define SCE_KERNEL_TA_THPRI     SCE_KERNEL_AT_THPRI
#define SCE_KERNEL_TA_MEMBTM    (0x4000u)

SceUID sceKernelCreateTlspl(const char *name, SceUID mpid, SceUInt attr, SceSize blockSize, int numBlocks, const SceKernelTlsplOptParam *opt);
int sceKernelDeleteTlspl(SceUID uid);
void *sceKernelGetTlsAddr(SceUID uid);
int sceKernelReferTlsplStatus(SceUID uid, SceKernelTlsplInfo *info);

/* Kernel Thread Local Storage (TLS) */

int sceKernelAllocateKTLS(int id, int (*cb)(unsigned int *size, void *arg), void *arg);
int sceKernelFreeKTLS(int id);
void *sceKernelGetKTLS(int id);
void *sceKernelGetThreadKTLS(int id, SceUID thid, int mode);

#endif /* THREADMAN_KERNEL_H */

