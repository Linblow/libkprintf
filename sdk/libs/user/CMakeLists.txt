set(LIBNAME "${PSP_LIB_NAME_PREFIX}user")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  InterruptManager
  IoFileMgrForUser
  Kernel_Library
  LoadExecForUser
  ModuleMgrForUser
  sceAudioRouting
  sceCertLoader
  sceDmac
  sceImpose
  sceReg
  sceRtc
  sceSuspendForUser
  StdioForUser
  SysMemUserForUser
  ThreadManForUser
  UtilsForUser
)
