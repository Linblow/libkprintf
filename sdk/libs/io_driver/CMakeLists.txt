set(LIBNAME "${PSP_LIB_NAME_PREFIX}io_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  LflashFatfmt
  sceCtrl_driver
  sceHprm_driver
  sceLed_driver
  sceLFatFs_driver
  sceMScm_driver
  sceMSstor_driver
  sceSircs_driver
  sceUmd
  sceUmd9660_driver
  sceUmdCache_driver
  sceUmdMan_driver
  sceUsb_driver
  sceUSB_Stor_Ms_driver
  sceUsb1Seg_driver
  sceUsbAcc_internal
  sceUsbAcc_driver
  sceUsbBus_driver
  sceUsbCam_driver
  sceUsbDmb_driver
  sceUsbGps_driver
  sceUsbMic_driver
  sceUsbstor_driver
  sceUsbstor_internal
  sceUsbstormln_driver
)
