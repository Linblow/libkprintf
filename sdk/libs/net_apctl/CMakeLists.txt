set(LIBNAME "${PSP_LIB_NAME_PREFIX}net_apctl")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceNetApctl
  sceNetApctl_lib
  sceNetApctl_internal_user
  sceNetApctl_lib2
)
