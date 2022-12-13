# This MUST be an INTERFACE library because we want the compiler LTO 
# optimization to work properly when a target links this library.
# A static library wouldn't get us the expected result (unless each function
# has its own object file).

set(TARGET psp_common)
add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE
  details/module.h
  psp/sdk.h
  psp/types.h
)

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
target_include_directories(${TARGET} INTERFACE ${PARENT_DIR})
