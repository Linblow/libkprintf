# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

#FIXME doesn't work properly on ELF > PRX (crash on load)
# Remove the useless sections (.MIPS.abiflags, .reginfo, etc) in a binary or stub lib.
# [IN] TARGET CMake target binary or stub library name
function(psp_strip_sections TARGET)
  add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND psp-objcopy -w -R .MIPS.* -R .gnu.* -R .reginfo -R .pdr "$<TARGET_FILE:${TARGET}>"
    COMMENT "Stripping useless sections from ${TARGET}"
  )
endfunction()
