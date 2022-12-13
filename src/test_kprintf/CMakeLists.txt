# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# Use compiler LTO optimization.
set(LTO_OPTIMIZATION ON)

project(
  test_kprintf
  VERSION 1.1.0 LANGUAGES C ASM
  DESCRIPTION "Kprintf test")

set(TARGET test_kprintf)

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
cmake_path(GET CMAKE_CURRENT_BINARY_DIR PARENT_PATH PARENT_BIN_DIR)
configure_file(config.h.in config.h @ONLY NEWLINE_STYLE LF)

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

# psp_strip_sections(${TARGET})
psp_target_build_exports_c(${TARGET}_startup)

# --------
# module
# --------

add_executable(${TARGET} "")
set_target_properties(${TARGET} PROPERTIES
  OUTPUT_NAME ${TARGET}
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  PSP_EXPORTS ${CMAKE_CURRENT_LIST_DIR}/exports.exp
)

target_sources(${TARGET} PRIVATE
  main.c
)
target_include_directories(${TARGET} PRIVATE
  ${PARENT_DIR}
  ${PARENT_BIN_DIR}
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
  psp_util
  pspfw_kernel
  pspfw_c
)

get_target_property(PRX_NAME ${TARGET} OUTPUT_NAME)
psp_prxgen(${TARGET} ${PRX_NAME}.prx)

# Install module to the project's dist directory (contents for PSP).  
set(INSTALL_DIR ms0/seplugins)
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
