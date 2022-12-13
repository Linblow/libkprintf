set(LIBNAME "${PSP_LIB_NAME_PREFIX}drm_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceDNAS_lib
  scePsheet_driver
  scePspNpDrm_driver
)
