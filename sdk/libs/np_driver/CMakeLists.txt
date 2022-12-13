set(LIBNAME "${PSP_LIB_NAME_PREFIX}np_driver")
add_library(${LIBNAME} INTERFACE)

# NOTE: see 'drm(_driver)' for scePspNpDrm_user/driver
target_link_libraries(${LIBNAME} INTERFACE
  sceNpInstall_driver
)
