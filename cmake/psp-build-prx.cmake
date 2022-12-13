# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# The pspsdk tools targets must be installed locally.
include(pspsdk-tools-targets)

# Generate a stripped PRX from an ELF, and encrypt the generated PRX for OFW.
# [IN] TARGET     Target name (ELF executable)
# [IN] BOOT_NAME  Output PRX file name (typically BOOT.BIN)
# [IN] EBOOT_NAME Output encrypted PRX file name (typically EBOOT.BIN)
# [IN] TAGID      ~PSP file encryption tag ID (0 <= n < 28)
function(psp_create_game_boot TARGET BOOT_NAME EBOOT_NAME TAGID)
  psp_prxgen(${TARGET} ${BOOT_NAME})
  psp_create_eboot_bin(${TARGET} ${BOOT_NAME} ${EBOOT_NAME} ${TAGID})
endfunction()

# Encrypt & sign a BOOT.BIN game module.
# The input/output file name is relative to the target's binary dir.
# [IN] TARGET     Target name (only necessary for CMake build target name)
# [IN] BOOT_NAME  Input PRX file name (typically BOOT.BIN)
# [IN] EBOOT_NAME Output PRX file name (typically EBOOT.BIN)
# [IN] TAGID      ~PSP file encryption tag ID (0 <= n < 28)
function(psp_create_eboot_bin TARGET BOOT_NAME EBOOT_NAME TAGID)
  set(BOOT_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${BOOT_NAME})
  set(EBOOT_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${EBOOT_NAME})
  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${EBOOT_NAME}" "${BOOT_NAME}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${EBOOT_PATH})
  add_custom_command(
    OUTPUT ${EBOOT_PATH}
    DEPENDS ${BOOT_PATH}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET}>
    COMMAND $<TARGET_FILE:pspsdk::sign-np> -elf "${BOOT_PATH}" "${EBOOT_PATH}" ${TAGID}
    COMMENT "Signing PSP game boot ${BOOT_NAME} as ${EBOOT_NAME}"
  )
endfunction()

# Encrypt & sign a plain PRX into a DEMO executable that will run in OFW.
# [IN] TARGET       Target name (only necessary for CMake build target name)
# [IN] IN_PRX_NAME  Input PRX file name with extension (.PRX/.BIN)
# [IN] OUT_PRX_NAME Output PRX file name with extension (.PRX/.BIN)
function(psp_prxofw TARGET IN_PRX_NAME OUT_PRX_NAME)
  set(IN_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${IN_PRX_NAME})
  set(OUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${OUT_PRX_NAME})
  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${IN_PRX_NAME}" "${OUT_PRX_NAME}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${OUT_PATH})
  add_custom_command(
    OUTPUT ${OUT_PATH}
    DEPENDS ${IN_PATH}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET}>
    COMMAND PrxEncrypter "${IN_PATH}" "${OUT_PATH}"
    COMMENT "Encrypting & signing PSP DEMO exec OFW ${IN_PRX_NAME} as ${OUT_PRX_NAME}"
  )
endfunction()

# Generate a stripped PRX from an ELF.
# [IN] TARGET   Target name (ELF executable)
# [IN] PRX_NAME Output PRX file name with extension (.PRX/.BIN)
function(psp_prxgen TARGET PRX_NAME)
  set(PRX_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME})
  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${PRX_NAME}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${PRX_PATH})
  add_custom_command(
    OUTPUT ${PRX_PATH}
    DEPENDS ${TARGET}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET}>
    COMMAND $<TARGET_FILE:pspsdk::fixup-imports> "$<TARGET_FILE_NAME:${TARGET}>"
    COMMAND $<TARGET_FILE:pspsdk::prxgen> "$<TARGET_FILE_NAME:${TARGET}>" "${PRX_PATH}"
  )
endfunction()

# Generate a PSP kernel firmware PRX from an ELF.
# The relocations sections will be in their own segment.
# All the program/section headers will be removed.
# [IN] TARGET   Target name (ELF executable)
# [IN] PRX_NAME Output PRX file name with extension (.PRX/.BIN)
function(psp_kprxgen TARGET PRX_NAME)
  set(PRX_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PRX_NAME})
  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${PRX_NAME}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${PRX_PATH})
  add_custom_command(
    OUTPUT ${PRX_PATH}
    DEPENDS ${TARGET}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET}>
    COMMAND $<TARGET_FILE:pspsdk::fixup-imports> "$<TARGET_FILE_NAME:${TARGET}>"
    COMMAND $<TARGET_FILE:pspsdk::prxgen> "$<TARGET_FILE_NAME:${TARGET}>" "${PRX_PATH}"
  )
endfunction()
