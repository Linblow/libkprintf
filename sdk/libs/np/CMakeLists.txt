set(LIBNAME "${PSP_LIB_NAME_PREFIX}np")
add_library(${LIBNAME} INTERFACE)

# NOTE: see 'drm(_driver)' for scePspNpDrm_user/driver
target_link_libraries(${LIBNAME} INTERFACE
  sceNp
  sceNpAuth
  sceNpCamp
  sceNpCommerce2
  sceNpCommerce2RegCam
  sceNpCommerce2Store
  sceNpCore
  sceNpInstall_user
  sceNpMatching2
  sceNpService
)
