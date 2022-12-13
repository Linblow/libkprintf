# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# Set the appropriate link/compile options for a PSP firmware kernel module.
# 
# [IN] TARGET CMake target name
function(psp_set_fw_module_options TARGET)
  target_link_options(${TARGET} PRIVATE
    -nostartfiles
    -nostdlib
  )
  target_compile_options(${TARGET} PRIVATE
    -fno-builtin-bcmp
    -fno-builtin-bcopy
    -fno-builtin-bzero
    -fno-builtin-index
    -fno-builtin-memchr
    -fno-builtin-memcmp
    -fno-builtin-memcpy
    -fno-builtin-memmove
    -fno-builtin-memset
    -fno-builtin-printf
    -fno-builtin-putchar
    -fno-builtin-puts
    -fno-builtin-rindex
    -fno-builtin-snprintf
    -fno-builtin-sprintf
    -fno-builtin-strcat
    -fno-builtin-strchr
    -fno-builtin-strcmp
    -fno-builtin-strcpy
    -fno-builtin-strlen
    -fno-builtin-strncmp
    -fno-builtin-strncpy
    -fno-builtin-strpbrk
    -fno-builtin-strrchr
    -fno-builtin-strstr
    -fno-builtin-tolower
    -fno-builtin-toupper
    -Wno-builtin-declaration-mismatch
  )
endfunction()
