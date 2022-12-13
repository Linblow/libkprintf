# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: GPL-3.0-or-later

# Can't set these in the toolchain file.
# see <https://gitlab.kitware.com/cmake/cmake/-/issues/18713>
set(CMAKE_C_OUTPUT_EXTENSION    ".o")
set(CMAKE_CXX_OUTPUT_EXTENSION  ".o")
set(CMAKE_ASM_OUTPUT_EXTENSION  ".o")

set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
