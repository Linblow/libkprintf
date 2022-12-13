# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# The pspsdk tools targets must be installed locally.
include(pspsdk-tools-targets)

# Get the stubs libraries name/count, as defined in a PSP exports file (.exp).
# Each S_* argument is a CMake variable name to be filled.
# Note the mandatory syslib is always ignored.
# No output file is built.
# 
# [IN]  EXP     **Full** path to the input exports file (.exp)
# [OUT] S_LIB   List of the stub libraries name (eg. "myLib;otherLib")
# [OUT] S_COUNT List of the stub libraries exports count (eg. "3;5")
# [OUT] S_SRC   List of the ASM sources (eg. "myLib.S;otherLib.S")
function(psp_get_exp_file_stub_list EXP S_LIB S_COUNT S_SRC)
  get_target_property(EXEC pspsdk::build-exports LOCATION)
  execute_process(
    COMMAND ${EXEC} --print-cmake=_ "${EXP}"
    OUTPUT_VARIABLE RESULT OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_VARIABLE  STDERR_OUTPUT
    RESULT_VARIABLE EXIT_CODE
  )

  if (NOT EXIT_CODE EQUAL "0")
    set(MSG "Cannot read the PSP exports file.\nExports file: ${EXP}")
    message(FATAL_ERROR ${MSG})
  endif()

  # RESULT contains CMake code with variables set to the stubs info.
  cmake_language(EVAL CODE "${RESULT}")
  if (NOT "${_S_LIB}" STREQUAL "")
    set(${S_LIB}   ${_S_LIB}   PARENT_SCOPE)
    set(${S_COUNT} ${_S_COUNT} PARENT_SCOPE)
    list(TRANSFORM   _S_LIB    APPEND ".S")
    set(${S_SRC}   ${_S_LIB}   PARENT_SCOPE)
  else()
    set(${S_LIB}   "" PARENT_SCOPE)
    set(${S_COUNT} "" PARENT_SCOPE)
    set(${S_SRC}   "" PARENT_SCOPE)
  endif()
endfunction()

# Build the .S source(s) from a PSP exports definition file (.exp).
# An output file will be built *only* when a target adds it to its sources.
# 
# [IN] EXP    **Full** path to the input exports file (.exp)
# [IN] S_SRC  The *expected* list of output files
function(psp_build_stubs_asm_sources EXP S_SRC)
  # get_target_property(EXEC pspsdk::build-exports LOCATION)
  add_custom_command(
    OUTPUT ${S_SRC}
    DEPENDS ${EXP}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND $<TARGET_FILE:pspsdk::build-exports> -k "${EXP}"
  )
endfunction()

# Build a stub library objects from its .S source file.
# A cmake object-library is added for each export.
#
# Added the optional PREFIX argument for each object-library name 
# to avoid a target name conflict when building several modules that 
# exports different functions with the same object-library name.
#
# Adapted from Davee's infinity2, credits to him / whoever wrote this.
#
# [IN]  PREFIX    Optional prefix for each CMake object-library name
# [IN]  LIBNAME   The stub library name
# [IN]  LIBCOUNT  The stub library exports count
# [OUT] OBJS      Output variable name to get the generated objects list
function(psp_build_stub_objects PREFIX LIBNAME LIBCOUNT OBJS)
  foreach(X RANGE ${LIBCOUNT})
    string(LENGTH "0000${X}" X_POSTFIX_LENGTH)
    math(EXPR X_POSTFIX_LENGTH "${X_POSTFIX_LENGTH}-4")
    string(SUBSTRING "0000${X}" ${X_POSTFIX_LENGTH} 4 X_POSTFIX)
    set(TARGET "${PREFIX}${LIBNAME}_${X_POSTFIX}")
    add_library(${TARGET} OBJECT "${LIBNAME}.S")
    target_compile_definitions(${TARGET} PRIVATE "F_${LIBNAME}_${X_POSTFIX}")
    # set(_OBJS ${_OBJS} $<TARGET_OBJECTS:${TARGET}>)
    list(APPEND _OBJS $<TARGET_OBJECTS:${TARGET}>)
  endforeach()
  set(${OBJS} ${_OBJS} PARENT_SCOPE)
endfunction()

# Build the stubs library for the exports defined in the given target.
# The PSP_EXPORTS target property is a list of exports files full path.
# 
# When the option SPLIT_STUBS is passed, the list of the created CMake 
# stubs lib targets will be added to the target PSP_STUB_LIBS property.
# You can also specify a variable name in STUB_LIBS_LIST to get the names list.
# The PSP_STUB_LIBS property won't be set when SPLIT_STUBS wasn't passed!
# 
# [IN] TARGET          CMake target library name (with PSP_EXPORTS property set)
# [IN] SPLIT_STUBS     Option: create a CMake stubs lib target for each library defined in the exports. 
# [IN] STUB_LIB_PREFIX Option: when SPLIT_STUBS was passed, this can be set to define a prefix
#                      for the new CMake stubs lib targets that are created. An underscore is 
#                      always appended.
# [OUT] STUB_LIBS_LIST Option: when SPLIT_STUBS was passed, name of the variable to 
#                      receive the list of the created CMake stubs lib targets.
function(psp_target_create_stub_libs TARGET)
  set(options SPLIT_STUBS)
  set(oneValueArgs STUB_LIB_PREFIX STUB_LIBS_LIST)
  set(multiValueArgs "")
  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if (ARG_SPLIT_STUBS)
    set(SPLIT_STUBS ON)
  endif()
  if (ARG_STUB_LIB_PREFIX)
    set(STUB_LIB_PREFIX ${ARG_STUB_LIB_PREFIX})
    if (NOT "${STUB_LIB_PREFIX}" STREQUAL "")
      string(APPEND STUB_LIB_PREFIX "_")
    endif()
  endif()

  # Get the exports list for the target's PSP_EXPORTS property.
  get_target_property(EXPORTS_LIST ${TARGET} PSP_EXPORTS)
  if (NOT DEFINED EXPORTS_LIST OR "${EXPORTS_LIST}" STREQUAL "")
    message(FATAL_ERROR "PSP_EXPORTS property was not set for target ${TARGET}")
  endif()

  # List of the created CMake stubs lib targets, if any.
  set(STUB_LIBS_LIST "")

  # Build stubs for each exports file (there's usually only one).
  foreach(EXP IN LISTS EXPORTS_LIST)
    # Get the stubs libs information (name/count lists).
    psp_get_exp_file_stub_list(${EXP} S_LIB S_COUNT S_SRC)
    if (NOT DEFINED S_LIB OR "${S_LIB}" STREQUAL "")
      message(STATUS "No stub libraries to build in ${EXP} for target ${TARGET}")
      continue()
    endif()

    # Generate the exports .S sources.
    psp_build_stubs_asm_sources(${EXP} "${S_SRC}")

    # Build the stubs objects from the generated sources
    # and add them to the given target.
    # Optionally, create a CMake target for each library that was found.
    unset(ALL_OBJS)
    set(INDEX 0)
    foreach(NAME ${S_LIB})
      list(GET S_COUNT ${INDEX} COUNT)
      unset(LIB_OBJS)
      psp_build_stub_objects(${TARGET}_ ${NAME} ${COUNT} LIB_OBJS)
      list(APPEND ALL_OBJS ${LIB_OBJS})
      target_sources(${TARGET} PRIVATE ${LIB_OBJS})

      if (SPLIT_STUBS)
        set(STUB_LIB_NAME ${STUB_LIB_PREFIX}${NAME})
        add_library(${STUB_LIB_NAME} ${LIB_OBJS})
        psp_strip_sections(${STUB_LIB_NAME})
        list(APPEND STUB_LIBS_LIST "${STUB_LIB_NAME}")
      endif()
      
      MATH(EXPR INDEX "${INDEX}+1")
    endforeach()

    psp_strip_sections(${TARGET})
  endforeach()

  get_target_property(PSP_STUB_LIBS ${TARGET} PSP_STUB_LIBS)
  if ("${PSP_STUB_LIBS}" STREQUAL "PSP_STUB_LIBS-NOTFOUND")
    set(PSP_STUB_LIBS "")
  endif()
  list(APPEND PSP_STUB_LIBS ${STUB_LIBS_LIST})
  set_target_properties(${TARGET} PROPERTIES PSP_STUB_LIBS "${PSP_STUB_LIBS}")

  if (ARG_STUB_LIBS_LIST)
    set(${ARG_STUB_LIBS_LIST} ${STUB_LIBS_LIST} PARENT_SCOPE)
  endif()
endfunction()


if (0)
# Create a cmake library for each stub library.
# [IN]  PREFIX  Optional prefix for each CMake object-library name
# [OUT] S_LIB   List of the stub libraries name
# [OUT] S_COUNT List of the stub libraries exports count
# [OUT] TARGETS Optional output cmake variable to get targets name list
function(create_stub_libs PREFIX S_LIB S_COUNT TARGETS)
  set(INDEX 0)
  set(_TARGETS "")
  foreach(NAME ${S_LIB})
    set(TARGET ${PREFIX}${NAME})
    list(GET S_COUNT ${INDEX} COUNT)
    psp_build_stub_objects(${PREFIX} ${NAME} ${COUNT} ${TARGET}_OBJS)
    add_library(${TARGET} STATIC ${${TARGET}_OBJS})
    list(APPEND _TARGETS ${TARGET})
    MATH(EXPR INDEX "${INDEX}+1")
  endforeach()
  if (DEFINED TARGETS AND NOT "${TARGETS}" STREQUAL "")
    set(${TARGETS} ${_TARGETS} PARENT_SCOPE)
  endif()
endfunction()
endif()
