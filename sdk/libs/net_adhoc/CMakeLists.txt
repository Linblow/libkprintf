set(LIBNAME "${PSP_LIB_NAME_PREFIX}net_adhoc")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceNetAdhoc
  sceNetAdhoc_lib
  sceNetAdhocctl
  sceNetAdhocctl_lib
  sceNetAdhocAuth_lib
  sceNetAdhocDiscover
  sceNetAdhocDownload
  sceNetAdhocMatching
  sceNetAdhocTransInt
)
