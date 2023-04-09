# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.23)

if (NOT CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/psp-toolchain.cmake)
endif()

# FORCE set the project's dist sub-directory as the install prefix.
# Files for the PSP will be installed to this dist directory. 
# However, development files (stubs/headers) will be installed 
# to the local pspsdk directory i.e $PSPDEV/psp 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/dist" CACHE STRING "Install prefix" FORCE)

set(PSP_TARGET_FW "661" CACHE STRING "PSP kernel version to use for stubs libs")
set(PSP_MS_DEVICE "ms0:" CACHE STRING "PSP MemoryStick device prefix")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(get-pspdev-path)

# Required for the CMake scripts that include(pspsdk-tools-targets)
list(APPEND CMAKE_MODULE_PATH
  "${PSPDEV}/lib/cmake"
  "${PSPDEV}/psp/lib/cmake"
)

# Include our CMake scripts.
include(utils)
include(relative-path-c-file-macro)
include(psp-build-exports)
include(psp-build-options)
include(psp-build-pbp)
include(psp-build-prx)
include(psp-build-strip)
include(psp-build-stub)

project(
  psp_kprintf_project
  VERSION 1.0.1 LANGUAGES C ASM
  DESCRIPTION "Sony PSP kprintf kernel library")

set(STUBS_DIR "${CMAKE_CURRENT_LIST_DIR}/sdk/stubs")
set(PSP_STUBS_DIR "${STUBS_DIR}/${PSP_TARGET_FW}")
if (EXISTS "${PSP_STUBS_DIR}")
  link_directories("${PSP_STUBS_DIR}")
else()
  message(FATAL_ERROR "Stubs directory doesn't exist: ${PSP_STUBS_DIR}")
endif()

include_directories("sdk/include")

add_subdirectory(sdk/libs)
add_subdirectory(libs)
add_subdirectory(src/libkprintf)
add_subdirectory(src/test_kprintf)
add_subdirectory(src/test_sce_printf)
add_subdirectory(src/test_tiny_printf)
