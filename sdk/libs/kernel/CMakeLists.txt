set(LIBNAME "${PSP_LIB_NAME_PREFIX}kernel")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  DmacManForKernel
  ExceptionManagerForKernel
  InitForKernel
  InterruptManagerForKernel
  IoFileMgrForKernel
  KDebugForKernel
  LoadCoreForKernel
  LoadExecForKernel
  ModuleMgrForKernel
  sceAudioRouting_driver
  sceCertLoader_driver
  sceChkreg_driver
  sceIdStorage_driver
  sceImpose_driver
  sceReg_driver
  sceRtc_driver
  # Should sceSemawm go to crypto_driver since mod has attr flag 0x4000 ?
  sceSemawm
  sceSuspendForKernel
  sceSysEventForKernel
  # NOTE: moved these two to 'kernel_c'
  # StdioForKernel
  # SysclibForKernel
  SysMemForKernel
  SysTimerForKernel
  ThreadManForKernel
  UtilsForKernel
)
