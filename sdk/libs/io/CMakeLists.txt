set(LIBNAME "${PSP_LIB_NAME_PREFIX}io")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceCtrl
  sceHprm
  sceSircs
  sceUmdCacheUser
  sceUmdUser
  sceUsb
  sceUsb1Seg
  sceUsbAcc
  sceUsbSer
  sceUsbCam
  sceUsbDmb
  sceUsbGps
  sceUsbMic
  sceUsbstor
  sceUsbstormln
)
