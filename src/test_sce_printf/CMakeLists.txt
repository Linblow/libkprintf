# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# Encrypt & sign app for PSP OFW.
set(MAKE_PSP_OFW_APP ON)

project(
  test_sce_printf
  VERSION 1.0.0 LANGUAGES C
  DESCRIPTION "sceKernelPrintf test")

# App title as seen in the XMB.
set(PARAM_TITLE "${PROJECT_DESCRIPTION} v${PROJECT_VERSION}")

# Name of the app directory created in ms0:/PSP/GAME.
# Will be transformed to a valid C language identifier.
set(APP_DIR_NAME "${PROJECT_NAME}")

# Package information files.
# set(MEDIA_DIR   "${CMAKE_SOURCE_DIR}/media/test_printf")
# set(PBP_ICON0   "${MEDIA_DIR}/ICON0.PNG")
# set(PBP_ICON1   "${MEDIA_DIR}/ICON1.PMF")
# set(PBP_PIC0    "${MEDIA_DIR}/PIC0.PNG")
# set(PBP_PIC1    "${MEDIA_DIR}/PIC1.PNG")
# set(PBP_SND0    "${MEDIA_DIR}/SND0.AT3")

string(MAKE_C_IDENTIFIER "${APP_DIR_NAME}" APP_DIR_CNAME)
cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
cmake_path(GET CMAKE_CURRENT_BINARY_DIR PARENT_PATH PARENT_BIN_DIR)
configure_file(config.h.in config.h @ONLY NEWLINE_STYLE LF)

set(TARGET ${PROJECT_NAME})

# --------
# app
# --------

add_executable(${TARGET} "")
set_target_properties(${TARGET} PROPERTIES
  OUTPUT_NAME BOOT
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
  # PSP_EXPORTS ${CMAKE_CURRENT_LIST_DIR}/exports.exp
)

set(SOURCES
  main.c
  modinfo.c
)
target_sources(${TARGET} PRIVATE ${SOURCES})
target_include_directories(${TARGET} PRIVATE 
  ${PARENT_DIR}
  ${PARENT_BIN_DIR}
)
target_compile_definitions(${TARGET} PRIVATE 
  $<$<CONFIG:Debug>:_DEBUG_OUTPUT=1>
  $<$<CONFIG:DebugWithInfo>:_DEBUG_OUTPUT=1>
  $<$<CONFIG:RelWithDebugOutput>:_DEBUG_OUTPUT=1>
  $<$<CONFIG:RelForDev>:_DEBUG_OUTPUT=1>
)

target_link_libraries(${TARGET} PRIVATE
  pspfw_c
  psp_common
  psp_debug
  psp_util
  pspfw_user
  pspfw_graphics
  pspfw_c
)

get_target_property(PRX_NAME ${TARGET} OUTPUT_NAME)
set(ENC_PRX_NAME ${PRX_NAME}_enc)

# NOTE: Debug* builds PRX will be stripped of their debug symbols.
psp_prxgen(${TARGET} ${PRX_NAME}.prx)
if (MAKE_PSP_OFW_APP)
  psp_prxofw(${TARGET} ${PRX_NAME}.prx ${ENC_PRX_NAME}.prx)
  set(PRX_NAME ${ENC_PRX_NAME})
endif()

psp_mksfo(${TARGET} PARAM.SFO "${PARAM_TITLE}")
psp_mkpbp(
  ${TARGET} EBOOT.PBP PARAM.SFO ${PRX_NAME}.prx 
  ${PBP_ICON0} ${PBP_ICON1} ${PBP_PIC0} ${PBP_PIC1} ${PBP_SND0}
)

set(INSTALL_DIR ms0/PSP/GAME/${APP_DIR_CNAME})
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/EBOOT.PBP
  DESTINATION $<CONFIG>/${INSTALL_DIR}
)
