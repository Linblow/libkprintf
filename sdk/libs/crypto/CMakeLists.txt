set(LIBNAME "${PSP_LIB_NAME_PREFIX}crypto")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceChnnlsv
  sceDdrdb
  sceMlnBridge
  sceMlnBridge_msapp
  sceOpenPSID
  scePauth
  sceResmgr
)
