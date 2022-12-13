# This MUST be an INTERFACE library because we want the compiler LTO 
# optimization to work properly when a target links this library.
# A static library wouldn't get us the expected result (unless each function
# has its own object file).

set(TARGET psp_util)
add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE
  cb.c
  cb.h
  log.c
  log.h
  sys.c
  sys.h
  umd.c
  umd.h
)

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
target_include_directories(${TARGET} INTERFACE ${PARENT_DIR})

target_link_libraries(${TARGET} INTERFACE 
  # NOTE:
  # Don't link pspfw_user / pspfw_kernel because we want to let 
  # the target that links this lib decide which one it prefers.
  psp_common
)
