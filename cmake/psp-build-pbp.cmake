# Generate a PARAM.SFO file.
function(psp_mksfo TARGET OUT_PARAM_SFO TITLE)
  # Full path is required, otherwise the command is always run!
  # See https://cmake.org/pipermail/cmake/2014-May/057549.html
  set(PARAM_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${OUT_PARAM_SFO})
  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${OUT_PARAM_SFO}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${PARAM_PATH})
  add_custom_command(
      OUTPUT ${PARAM_PATH}
      COMMAND mksfo "${TITLE}" "${PARAM_PATH}"
      COMMENT "Building PSP SFO ${OUT_PARAM_SFO}"
  )
endfunction()

function(psp_mksfoex TARGET OUT_PARAM_SFO TITLE PARAMS)
  # This ugly shit sorts A-Z the params list by also preserving 
  # the param type which is first character of each element.
  if (0)
    set(_PARAMS "")
    set(TYPES "d" "s")
    foreach(P IN LISTS PARAMS)
      string(SUBSTRING "${P}" 0 1 TYPE)
      if (NOT "${TYPE}" IN_LIST TYPES)
        message(FATAL_ERROR "Invalid SFO parameter type ('${TYPE}' was specified)")
      endif()
      string(SUBSTRING "${P}" 1 -1 P)
      # Move the type to the end since we'll restore it after sorting.
      # Not doing that would cause an invalid sort because of the leading type.
      list(APPEND _PARAMS "${P}${TYPE}")
    endforeach()
    # Sort the parameters.
    list(SORT _PARAMS)
    # Move back each saved type to the elem start.
    set(PARAMS "")
    foreach(P IN LISTS _PARAMS)
      string(LENGTH "${P}" LEN)
      MATH(EXPR POS "${LEN}-1")
      string(SUBSTRING "${P}" ${POS} 1 TYPE)
      string(SUBSTRING "${P}" 0 ${POS} P)
      list(APPEND PARAMS "${TYPE}${P}")
    endforeach()
  endif()

  # Add the "-" indicating these are options.
  list(JOIN PARAMS " -" PARAMS)
  set(PARAMS "-${PARAMS}")

  # Full path is required, otherwise the command is always run!
  # See https://cmake.org/pipermail/cmake/2014-May/057549.html
  set(PARAM_PATH ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${OUT_PARAM_SFO})
  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${OUT_PARAM_SFO}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${PARAM_PATH})
  add_custom_command(
      OUTPUT ${PARAM_PATH}
      COMMAND mksfoex ${PARAMS} "${TITLE}" "${PARAM_PATH}"
      COMMENT "Building PSP extended SFO ${OUT_PARAM_SFO}"
  )
endfunction()

# Generate a bootable EBOOT.PBP package.
function(psp_mkpbp TARGET OUT_PBP_NAME IN_PARAM_SFO IN_EXEC_NAME)
  set(ICON0 ${ARGV4})
  set(ICON1 ${ARGV5})
  set(PIC0  ${ARGV6})
  set(PIC1  ${ARGV7})
  set(SND0  ${ARGV8})
  set(PSAR  ${ARGV9})

  if (NOT "${ICON0}" STREQUAL "" AND NOT EXISTS "${ICON0}")
    message(NOTICE "PSP ICON0 file not found: ${ICON0}")
    set(ICON0 "")
  endif()
  if (NOT "${ICON1}" STREQUAL "" AND NOT EXISTS "${ICON1}")
    message(NOTICE "PSP ICON1 file not found: ${ICON1}")
    set(ICON1 "")
  endif()
  if (NOT "${PIC0}" STREQUAL "" AND NOT EXISTS "${PIC0}")
    message(NOTICE "PSP PIC0 file not found: ${PIC0}")
    set(PIC0 "")
  endif()
  if (NOT "${PIC1}" STREQUAL "" AND NOT EXISTS "${PIC1}")
    message(NOTICE "PSP PIC1 file not found: ${PIC1}")
    set(PIC1 "")
  endif()
  if (NOT "${SND0}" STREQUAL "" AND NOT EXISTS "${SND0}")
    message(NOTICE "PSP SND0 file not found: ${SND0}")
    set(SND0 "")
  endif()
  if (NOT "${PSAR}" STREQUAL "" AND NOT EXISTS "${PSAR}")
    message(NOTICE "PSP PSAR file not found: ${PSAR}")
    set(PSAR "")
  endif()


  # Full path is required, otherwise the command is always run!
  # See https://cmake.org/pipermail/cmake/2014-May/057549.html
  set(BIN        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
  set(PBP_PATH   ${BIN}/${OUT_PBP_NAME})
  set(PARAM_PATH ${BIN}/${IN_PARAM_SFO})
  set(PRX_PATH   ${BIN}/${IN_EXEC_NAME})

  concat_as_tgtname(CUSTOM_TGT "${TARGET}" "${OUT_PBP_NAME}" "${IN_EXEC_NAME}")
  add_custom_target(${CUSTOM_TGT} ALL DEPENDS ${PBP_PATH})
  add_custom_command(
    OUTPUT ${PBP_PATH}
    DEPENDS ${PARAM_PATH} ${PRX_PATH}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET}>
    # COMMAND pack-pbp
    #         "${PBP_PATH}"
    #         "${PARAM_PATH}" 
    #         "$<IF:$<BOOL:${ICON0}>,${ICON0},NULL>"
    #         "$<IF:$<BOOL:${ICON1}>,${ICON1},NULL>"
    #         "$<IF:$<BOOL:${PIC0}>,${PIC0},NULL>"
    #         "$<IF:$<BOOL:${PIC1}>,${PIC1},NULL>"
    #         "$<IF:$<BOOL:${SND0}>,${SND0},NULL>"
    #         "${PRX_PATH}"
    #         NULL
    COMMAND pack-pbp
            -q
            -o "${PBP_PATH}"
            -1 "${PARAM_PATH}" 
            $<IF:$<BOOL:${ICON0}>,-2,> $<IF:$<BOOL:${ICON0}>,"${ICON0}",>
            $<IF:$<BOOL:${ICON1}>,-3,> $<IF:$<BOOL:${ICON1}>,"${ICON1}",>
            $<IF:$<BOOL:${PIC0}>,-4,> $<IF:$<BOOL:${PIC0}>,"${PIC0}",>
            $<IF:$<BOOL:${PIC1}>,-5,> $<IF:$<BOOL:${PIC1}>,"${PIC1}",>
            $<IF:$<BOOL:${SND0}>,-6,> $<IF:$<BOOL:${SND0}>,"${SND0}",>
            -7 "${PRX_PATH}"
            $<IF:$<BOOL:${PSAR}>,-8,> $<IF:$<BOOL:${PSAR}>,"${PSAR}",>
    COMMENT "Building PBP archive ${OUT_PBP_NAME}"
  )
endfunction()
