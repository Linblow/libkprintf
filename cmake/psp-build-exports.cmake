# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# The pspsdk tools targets must be installed locally.
include(pspsdk-tools-targets)

# Build the .c source file from a PSP exports definition file (.exp).
# The output file will be built *only* when a target adds it to its sources.
# This is for PRX modules that export functions.
# 
# [IN] EXP    **Full** path to the input exports file (.exp)
# [IN] TO     Name of the output .c file
# [IN] OUTDIR Working directory for the build-exports command [OPTIONAL] 
#             Defaults to ${CMAKE_CURRENT_BINARY_DIR}.
function(psp_exp_file_to_c)
  set(options "")
  set(oneValueArgs EXP TO OUTDIR)
  set(multiValueArgs "")
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (NOT DEFINED ARG_EXP OR "${ARG_EXP}" STREQUAL "")
    message(FATAL_ERROR "missing argument EXP")
  endif()
  if (NOT DEFINED ARG_TO OR "${ARG_TO}" STREQUAL "")
    message(FATAL_ERROR "missing argument TO")
  endif()
  if (NOT DEFINED ARG_OUTDIR OR "${ARG_OUTDIR}" STREQUAL "")
    set(ARG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
  endif()

  get_target_property(EXEC pspsdk::build-exports LOCATION)
  add_custom_command(
    OUTPUT ${ARG_TO}
    DEPENDS ${ARG_EXP}
    WORKING_DIRECTORY ${ARG_OUTDIR}
    COMMAND ${EXEC} -b --uofw "${ARG_EXP}" > "${ARG_TO}"
  )
endfunction()

# Build a PSP target exports .c source file and add it to the target's sources.
# The target MUST have the custom property PSP_EXPORTS defined. 
# It contains the file name of its exports definition file (eg. exports.exp).
# This is for PRX modules that export functions.
# 
# [IN] TARGET CMake PSP target
function(psp_target_build_exports_c TARGET)
  get_target_property(EXP_SRC ${TARGET} PSP_EXPORTS)
  if (NOT DEFINED EXP_SRC OR "${EXP_SRC}" STREQUAL "")
    message(FATAL_ERROR "PSP_EXPORTS property was not set for target ${TARGET}")
  endif()

  cmake_path(GET EXP_SRC STEM C_SRC)
  set(C_SRC "generated_${C_SRC}.c")
  # set(C_SRC "${CMAKE_CURRENT_BINARY_DIR}/${C_SRC}")

  psp_exp_file_to_c(EXP "${EXP_SRC}" TO "${C_SRC}")
  target_sources(${TARGET} PRIVATE ${C_SRC})
  set_source_files_properties(${C_SRC} PROPERTIES COMPILE_OPTIONS -Wno-builtin-declaration-mismatch)
endfunction()
