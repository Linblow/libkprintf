set(LIBNAME "${PSP_LIB_NAME_PREFIX}net")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceNet
  sceNet_lib
  sceNetIfhandle
  sceNetIfhandle_lib
  sceNetInet
  sceNetInet_lib
  sceNetResolver
  sceNetStun
  sceNetUpnp
  sceNetWispr
)
