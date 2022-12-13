add_subdirectory(psp/common)
add_subdirectory(psp/debug)
add_subdirectory(psp/util)

include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)
cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)

# --------
# inih
# --------

# NOTE: inih requires libc's ctype 

add_library(inih INTERFACE)
target_sources(inih INTERFACE
  inih/ini.c
  inih/ini.h
)
target_compile_definitions(inih INTERFACE ${INIH_OPTIONS})
target_include_directories(inih INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# --------
# printf
# --------

add_library(printf INTERFACE)
target_sources(printf INTERFACE
  printf/src/printf/printf.c
  printf/src/printf/printf.h
)
target_compile_definitions(printf INTERFACE ${PRINTF_OPTIONS})

if (PRINTF_ALIAS_STANDARD_FUNCTION_NAMES)
  target_compile_definitions(printf INTERFACE PRINTF_ALIAS_STANDARD_FUNCTION_NAMES=1)
  # Prevent printf optimizations ie. printf replaced by puts.
  # Only needed when PRINTF_ALIAS_STANDARD_FUNCTION_NAMES is set to 1.
  target_compile_options(printf INTERFACE -fno-builtin-printf)
endif()
target_include_directories(printf INTERFACE ${CMAKE_CURRENT_LIST_DIR}/printf/src)
