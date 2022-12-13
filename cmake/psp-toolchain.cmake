# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause
# Adapted from Davee's infinity2 psp-toolchain.cmake.
# This is for cmake v3.19+, with a few things added.

cmake_minimum_required(VERSION 3.19)

if (DEFINED CMAKE_CROSSCOMPILING)
  return()
endif()

set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_VERSION    1)
set(CMAKE_SYSTEM_PROCESSOR  "PSP")

# set these global flags for cmake client scripts to change behavior
set(PSP true)
set(BUILD_PSP true)
set(PLATFORM_PSP true)

include(get-pspdev-path)
set(CMAKE_INSTALL_PREFIX "${PSPDEV}/psp" CACHE PATH "Default install prefix")

# Semicolon-separated list of directories specifying a search path for CMake 
# modules to be loaded by the include() or find_package() commands before 
# checking the default modules that come with CMake. 
# By default it is empty, it is intended to be set by the project.
# set(CMAKE_MODULE_PATH
#   "${PSPDEV}/lib/cmake"
#   "${PSPDEV}/psp/lib/cmake"
#   ${CMAKE_MODULE_PATH}
# )

# Semicolon-separated list of root paths to search on the filesystem.
# This variable is most useful when cross-compiling. 
# CMake uses the paths in this list as alternative roots to find filesystem 
# items with find_package(), find_library() etc.
set(CMAKE_FIND_ROOT_PATH
  "${PSPDEV}"
  "${PSPDEV}/psp"
  "${CMAKE_INSTALL_PREFIX}"
  "${CMAKE_INSTALL_PREFIX}/share"
)

# Only the roots in CMAKE_FIND_ROOT_PATH will be searched 
# by find_library(), find_file() and find_path().
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Skip C/C++ compiler check.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set the suffix for the generated files name.
# This cannot be done directly inside the toolchain file!
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/set-obj-ext.cmake")

set(BIN "${PSPDEV}/bin")
if (CMAKE_HOST_WIN32)
  set(EXE_SUFFIX ".exe")
endif()

set(CMAKE_C_COMPILER   "${BIN}/psp-gcc${EXE_SUFFIX}"     CACHE PATH "C compiler")
set(CMAKE_CXX_COMPILER "${BIN}/psp-g++${EXE_SUFFIX}"     CACHE PATH "C++ compiler")
set(CMAKE_ASM_COMPILER "${BIN}/psp-gcc${EXE_SUFFIX}"     CACHE PATH "assembler")
set(CMAKE_STRIP        "${BIN}/psp-strip${EXE_SUFFIX}"   CACHE PATH "strip")
set(CMAKE_AR           "${BIN}/psp-ar${EXE_SUFFIX}"      CACHE PATH "archive")
set(CMAKE_LINKER       "${BIN}/psp-ld${EXE_SUFFIX}"      CACHE PATH "linker")
set(CMAKE_NM           "${BIN}/psp-nm${EXE_SUFFIX}"      CACHE PATH "nm")
set(CMAKE_OBJCOPY      "${BIN}/psp-objcopy${EXE_SUFFIX}" CACHE PATH "objcopy")
set(CMAKE_OBJDUMP      "${BIN}/psp-objdump${EXE_SUFFIX}" CACHE PATH "objdump")
set(CMAKE_RANLIB       "${BIN}/psp-ranlib${EXE_SUFFIX}"  CACHE PATH "ranlib")

list(APPEND COMMON_INCLUDE_DIR
  "${PSPDEV}/psp/sdk/include" # PSPSDK kernel/user API headers.
  "${PSPDEV}/psp/include" # PSPSDK user libraries.
)

#TODO not all of these should be defined. should be set per project
# Common C/CXX/ASM flags
list(APPEND COMMON_FLAGS
  "G0" "mno-gpopt"
  "mpreferred-stack-boundary=4" 
  "msingle-float" "mhard-float" "mfp32"
  "Wall"
  "Wno-format-zero-length"
  # "Wdouble-promotion" 
  "fshort-wchar" 
  "fomit-frame-pointer" 
  "fdelayed-branch"
  "fdebug-types-section"
)

# Define our build types.
list(APPEND COMMON_FLAGS
  "D_BUILD_TYPE_DEB=1"
  "D_BUILD_TYPE_DEB_WITH_INFO=2"
  "D_BUILD_TYPE_REL=3"
  "D_BUILD_TYPE_REL_WITH_DEBUG_OUTPUT=4"
  "D_BUILD_TYPE_REL_FOR_DEV=5"
  "D_BUILD_TYPE_DEBUG=1"
  "D_BUILD_TYPE_DEBUG_WITH_INFO=2"
  "D_BUILD_TYPE_RELEASE=3"
  "D_BUILD_TYPE_RELEASE_WITH_DEBUG_OUTPUT=4"
  "D_BUILD_TYPE_RELEASE_FOR_DEV=5"
)

# Minimal debug info.
list(APPEND DEBUG_FLAGS
  "DBUILD_TYPE=1"
  "D_BUILD_TYPE=1"
  "DDEBUG=1"
  "D_DEBUG=1"
  "Og"
)

# Maximum debug info.
list(APPEND DEBUGWITHINFO_FLAGS
  "DBUILD_TYPE=2"
  "D_BUILD_TYPE=2"
  "DDEBUG=1"
  "D_DEBUG=1"
  # gcc: use an additional -glevel option to change the debug level for DWARF.
  # "gdwarf"
  "g3"
  # "gdwarf32"
  # "gsplit-dwarf"
  # "ggdb3"
  "gpubnames"
  "ginline-points"
  "gstatement-frontiers"
)

list(APPEND RELEASE_FLAGS
  "DBUILD_TYPE=3"
  "D_BUILD_TYPE=3"
  "DNDEBUG=1"
  "D_NDEBUG=1"
  "g0"
  "Os"
)

# Same as Release with debug output.
list(APPEND RELWITHDEBUGOUTPUT_FLAGS
  "DBUILD_TYPE=4"
  "D_BUILD_TYPE=4"
  "DNDEBUG=1"
  "D_NDEBUG=1"
  "g0"
  "Os"
)

# Same as RelWithDebugOutput with specific features for devs.
list(APPEND RELFORDEV_FLAGS
  "DBUILD_TYPE=5"
  "D_BUILD_TYPE=5"
  "DNDEBUG=1"
  "D_NDEBUG=1"
  "g0"
  "Os"
)

list(APPEND LINKER_FLAGS
  "L${PSPSDK}/lib"      # PSP user/kernel API.
  "L${PSPDEV}/psp/lib"  # PSPSDK user libraries.
  "specs=${PSPSDK}/lib/prxspecs"
  "Wl,-q,-T${PSPSDK}/lib/linkfile.prx"
)

list(TRANSFORM  COMMON_INCLUDE_DIR  PREPEND "-I")
list(JOIN       COMMON_INCLUDE_DIR  " " COMMON_INCLUDE_DIR)

list(TRANSFORM  COMMON_FLAGS PREPEND "-")
list(JOIN       COMMON_FLAGS " " COMMON_FLAGS)
list(TRANSFORM  DEBUG_FLAGS PREPEND "-")
list(JOIN       DEBUG_FLAGS " " DEBUG_FLAGS)
list(TRANSFORM  DEBUGWITHINFO_FLAGS PREPEND "-")
list(JOIN       DEBUGWITHINFO_FLAGS " " DEBUGWITHINFO_FLAGS)
list(TRANSFORM  RELEASE_FLAGS PREPEND "-")
list(JOIN       RELEASE_FLAGS " " RELEASE_FLAGS)
list(TRANSFORM  RELWITHDEBUGOUTPUT_FLAGS PREPEND "-")
list(JOIN       RELWITHDEBUGOUTPUT_FLAGS " " RELWITHDEBUGOUTPUT_FLAGS)
list(TRANSFORM  RELFORDEV_FLAGS PREPEND "-")
list(JOIN       RELFORDEV_FLAGS " " RELFORDEV_FLAGS)
list(TRANSFORM  LINKER_FLAGS PREPEND "-")
list(JOIN       LINKER_FLAGS " " LINKER_FLAGS)

set(CMAKE_C_STANDARD   99) # C99 / gnu99
set(CMAKE_CXX_STANDARD 14) # C++14

set(CMAKE_C_FLAGS   "${COMMON_FLAGS} ${COMMON_INCLUDE_DIR}"   CACHE STRING "C flags")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${COMMON_INCLUDE_DIR}"   CACHE STRING "C++ flags")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} ${COMMON_INCLUDE_DIR} ${CMAKE_ASM_FLAGS}"  CACHE STRING "ASM flags")

set(CMAKE_C_FLAGS_DEBUG   "${DEBUG_FLAGS}" CACHE STRING "C Debug flags")
set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS}" CACHE STRING "C++ Debug flags")
set(CMAKE_ASM_FLAGS_DEBUG "${DEBUG_FLAGS}" CACHE STRING "ASM Debug flags")

set(CMAKE_C_FLAGS_DEBUGWITHINFO    "${DEBUGWITHINFO_FLAGS}" CACHE STRING "C DebugWithInfo flags")
set(CMAKE_CXX_FLAGS_DEBUGWITHINFO  "${DEBUGWITHINFO_FLAGS}" CACHE STRING "C++ DebugWithInfo flags")
set(CMAKE_ASL_FLAGS_DEBUGWITHINFO  "${DEBUGWITHINFO_FLAGS}" CACHE STRING "ASM DebugWithInfo flags")

set(CMAKE_C_FLAGS_RELEASE   "${RELEASE_FLAGS}" CACHE STRING "C Release flags")
set(CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS}" CACHE STRING "C++ Release flags")
set(CMAKE_ASM_FLAGS_RELEASE "${RELEASE_FLAGS}" CACHE STRING "ASM Release flags")

set(CMAKE_C_FLAGS_RELWITHDEBUGOUTPUT   "${RELWITHDEBUGOUTPUT_FLAGS}" CACHE STRING "C RelWithDebugOutput flags")
set(CMAKE_CXX_FLAGS_RELWITHDEBUGOUTPUT "${RELWITHDEBUGOUTPUT_FLAGS}" CACHE STRING "C++ RelWithDebugOutput flags")
set(CMAKE_ASM_FLAGS_RELWITHDEBUGOUTPUT "${RELWITHDEBUGOUTPUT_FLAGS}" CACHE STRING "ASM RelWithDebugOutput flags")

set(CMAKE_C_FLAGS_RELFORDEV   "${RELFORDEV_FLAGS}" CACHE STRING "C RelForDev flags")
set(CMAKE_CXX_FLAGS_RELFORDEV "${RELFORDEV_FLAGS}" CACHE STRING "C++ RelForDev flags")
set(CMAKE_ASM_FLAGS_RELFORDEV "${RELFORDEV_FLAGS}" CACHE STRING "ASM RelForDev flags")

set(CMAKE_SHARED_LINKER_FLAGS "" CACHE STRING "shared linker flags")
set(CMAKE_MODULE_LINKER_FLAGS "" CACHE STRING "module linker flags")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS}" CACHE STRING "executable linker flags")
