set(LIBNAME "${PSP_LIB_NAME_PREFIX}crypto_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  memlmd
  sceAmctrl_driver
  sceChnnslv_driver
  sceDbman_driver
  sceDbsvr_driver
  sceDdrdb_driver
  sceMemab_driver
  sceMesgd_driver
  sceMesgIns_driver
  sceMesgLed_driver
  sceMgMemoryStick_driver
  sceMgr_driver
  sceMlnBridge_driver
  sceMlnBridge_msapp_driver
  sceMlnpsnl_driver
  sceNwman_driver
  sceOpenPSID_driver
  scePauth_driver
  scePcact_driver
  sceResmap_driver
  sceResmgr_driver
  sceWmd_driver
  semaphore
)
