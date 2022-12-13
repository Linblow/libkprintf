# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# Use compiler LTO optimization.
# Care must be taken for exported functions.
set(LTO_OPTIMIZATION ON)

project(
  kprintf_module
  VERSION 1.1.0 LANGUAGES C ASM
  DESCRIPTION "PSP Kprintf library module")

set(TARGET kprintf)

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
cmake_path(GET CMAKE_CURRENT_BINARY_DIR PARENT_PATH PARENT_BIN_DIR)
include(${PARENT_DIR}/config.cmake)
configure_file(config.h.in config.h @ONLY NEWLINE_STYLE LF)

# --------
# stubs library
# --------

add_library(${TARGET}_stub "")
set_target_properties(${TARGET}_stub PROPERTIES
  OUTPUT_NAME kprintf_stub
  # We don't want the suffix for these configurations when building stubs.
  DEBUGWITHINFO_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  RELWITHDEBUGOUTPUT_POSTFIX ""
  RELFORDEV_POSTFIX ""
  PSP_EXPORTS ${CMAKE_CURRENT_LIST_DIR}/exports.exp
)
target_link_options(${TARGET}_stub PRIVATE -nostartfiles)
psp_target_create_stub_libs(${TARGET}_stub SPLIT_STUBS)

# --------
# startup
# --------

# NOTE: This must be built seperately, so that LTO works.

add_library(${TARGET}_startup "")
set_target_properties(${TARGET}_startup PROPERTIES
  DEBUGWITHINFO_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  RELWITHDEBUGOUTPUT_POSTFIX ""
  RELFORDEV_POSTFIX ""
  PSP_EXPORTS ${CMAKE_CURRENT_LIST_DIR}/exports.exp
)
target_sources(${TARGET}_startup PRIVATE
  startup.c
)
target_include_directories(${TARGET}_startup PRIVATE
  ${PARENT_DIR}
  ${PARENT_BIN_DIR}
)
target_compile_definitions(${TARGET}_startup PRIVATE 
  KERNEL_MODULE=1
  # USE_START_STOP_THREAD_PARAMETER=1
)
target_link_options(${TARGET}_startup PRIVATE -nostartfiles)
target_link_libraries(${TARGET}_startup PRIVATE 
  psp_common
  pspfw_kernel
)

psp_target_build_exports_c(${TARGET}_startup)

# --------
# module
# --------

add_executable(${TARGET} "")
set_target_properties(${TARGET} PROPERTIES
  OUTPUT_NAME libkprintf
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  PSP_EXPORTS ${CMAKE_CURRENT_LIST_DIR}/exports.exp
)

set(PUBLIC_HEADERS
  kprintf.h
)
set(PRIVATE_HEADERS
  kprintf_int.h
)
set(SOURCES
  api.c
  event.c
  globals.c
  handler.c
  msg.c
  putchar_.c
)
target_sources(${TARGET}
  PRIVATE
    ${SOURCES} ${PRIVATE_HEADERS}
  PUBLIC
    FILE_SET HEADERS
    # BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} 
    FILES ${PUBLIC_HEADERS}
)
target_include_directories(${TARGET} PRIVATE
  ${PARENT_DIR}
  ${PARENT_BIN_DIR}
)
target_compile_definitions(${TARGET} PRIVATE
  # DEBUG=1
)
if (LTO_OPTIMIZATION)
  target_compile_options(${TARGET} PRIVATE
    -fdata-sections
    -ffunction-sections
    -fsection-anchors
    -flto
  )
endif()

target_link_options(${TARGET} PRIVATE -nostartfiles)
target_link_libraries(${TARGET} PRIVATE 
  ${TARGET}_startup
  psp_common
  printf
  pspfw_kernel
  pspfw_kernel_c
  # NOTE: libc is placed last since Sysclib imports are preferred.
  pspfw_c
)

# psp_strip_sections(${TARGET})

get_target_property(STUB_NAME ${TARGET}_stub OUTPUT_NAME)
get_target_property(PRX_NAME ${TARGET} OUTPUT_NAME)
psp_prxgen(${TARGET} ${PRX_NAME}.prx)

# Install the stubs lib target to pspdev directory.
# install(
#   FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/lib${STUB_NAME}.a
#   CONFIGURATIONS Release
#   DESTINATION ${PSP_PREFIX}/lib/libkprintf
# )

# Install the stubs lib targets to pspdev directory.
get_target_property(STUBS ${TARGET}_stub PSP_STUB_LIBS)
list(TRANSFORM STUBS PREPEND "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/lib")
list(TRANSFORM STUBS APPEND ".a")
install(
  FILES ${STUBS}
  CONFIGURATIONS Release
  DESTINATION ${PSP_PREFIX}/lib/libkprintf
)
#TODO this:
# psp_install_split_stubs(${TARGET} Release "${PSP_PREFIX}/lib/libkprintf")

# Install public headers to pspdev directory.
# This installs the public headers, and nothing else :)
install_target_headers(${TARGET} "${CMAKE_CURRENT_SOURCE_DIR}" "${PSP_PREFIX}/include/libkprintf")

# Install module to the project's dist directory (contents for PSP).
set(INSTALL_DIR ms0/${KPRINTF_MS_LIB_PATH})
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME}.prx
  RENAME ${TARGET}${CMAKE_DEBUG_POSTFIX}.prx
  CONFIGURATIONS Debug
  DESTINATION $<CONFIG>/${INSTALL_DIR}
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME}.prx
  RENAME ${TARGET}${CMAKE_DEBUG_POSTFIX}.prx
  CONFIGURATIONS DebugWithInfo
  DESTINATION $<CONFIG>/${INSTALL_DIR}
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME}.prx
  CONFIGURATIONS Release
  DESTINATION $<CONFIG>/${INSTALL_DIR}
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME}.prx
  # RENAME ${PRX_NAME}${CMAKE_RELWITHDEBUGOUTPUT_POSTFIX}.prx
  CONFIGURATIONS RelWithDebugOutput
  DESTINATION $<CONFIG>/${INSTALL_DIR}
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME}.prx
  # RENAME ${PRX_NAME}${CMAKE_RELFORDEV_POSTFIX}.prx
  CONFIGURATIONS RelForDev
  DESTINATION $<CONFIG>/${INSTALL_DIR}
)
