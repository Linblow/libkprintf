# This MUST be an INTERFACE library because we want the compiler LTO 
# optimization to work properly when a target links this library.
# A static library wouldn't get us the expected result (unless each function
# has its own object file).

option(USE_DEBUG_SCREEN_16 "Support 16-bits display mode" OFF)

set(TARGET psp_debug)
add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE
  # _dprintf.c
  # _dprintf.h
  font.c
  # kb.c
  # kb.h
  putchar_.c
  scr.c
  scrk.c
  scr.h
  # trace.c
  # trace.h
)
if (USE_DEBUG_SCREEN_16)
  target_compile_definitions(${TARGET} INTERFACE USE_DEBUG_SCREEN_16=1)
endif()

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
target_include_directories(${TARGET} INTERFACE ${PARENT_DIR})

target_link_libraries(${TARGET} INTERFACE
  # NOTE:
  # Don't link pspfw_user / pspfw_kernel because we want to let 
  # the target that links this lib decide which one it prefers.
  psp_common
  # pspfw_graphics
)
