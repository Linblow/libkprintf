set(LIBNAME "${PSP_LIB_NAME_PREFIX}net_wlan_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceWlanDrv_driver
)
