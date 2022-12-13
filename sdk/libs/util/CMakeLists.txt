set(LIBNAME "${PSP_LIB_NAME_PREFIX}util")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceUtility
  sceUtility_private
  sceUtility_netparam_internal
)
