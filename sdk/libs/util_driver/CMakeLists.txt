set(LIBNAME "${PSP_LIB_NAME_PREFIX}util_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceUtility_driver
)
