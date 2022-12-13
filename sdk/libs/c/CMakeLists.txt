set(LIBNAME "${PSP_LIB_NAME_PREFIX}c")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  # IMPORTANT: cglue two times to avoid the fucking annoying stubs out of order error
  cglue
  c
  cglue
  pthreadglue
  pthread
)
