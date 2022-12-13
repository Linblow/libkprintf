set(LIBNAME "${PSP_LIB_NAME_PREFIX}drm")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceDNASCore_lib
  sceGameUpdate
  scePsheet
  scePspNpDrm_user
)
