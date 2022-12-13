set(LIBNAME "${PSP_LIB_NAME_PREFIX}graphics")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceGe_user
  sceGe_lazy
  sceDisplay
)

set(LIBNAME "${PSP_LIB_NAME_PREFIX}graphics_kernel")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceGe_driver
  sceDisplay_driver
)

add_library(psp_gu STATIC IMPORTED GLOBAL)
set_target_properties(psp_gu PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/libgu.a"
  IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/libgu_debug.a"
  IMPORTED_CONFIGURATIONS "Release;Debug"
)

add_library(psp_gum STATIC IMPORTED GLOBAL)
set_target_properties(psp_gum PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/libgum.a"
)

add_library(psp_gum_vfpu STATIC IMPORTED GLOBAL)
set_target_properties(psp_gum_vfpu PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/libgum_vfpu.a"
)