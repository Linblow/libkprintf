set(LIBNAME "${PSP_LIB_NAME_PREFIX}net_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceNetAdhocAuth_driver
  sceNetIfhandle_driver
)
