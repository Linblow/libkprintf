# Get a valid CMake target name.
# The args are normalized, and joined together using "_" as glue. 
# Final string is converted to lower case, and stored to the given variable name.
function(concat_as_tgtname OUTPUT_VARIABLE)
  set(VAL "")
  foreach(ARG IN LISTS ARGN)
    if ("${VAL}" STREQUAL "")
      set(SEP "")
    else()
      set(SEP "_")
    endif()
    string(MAKE_C_IDENTIFIER "${ARG}" C_ARG)
    # Remove the extra "_", if any.
    string(SUBSTRING "${C_ARG}" 0 1 LETTER)
    if ("${LETTER}" STREQUAL "_")
      set(SEP "")
    endif()
    string(APPEND VAL "${SEP}${C_ARG}")
 endforeach()
 string(TOLOWER "${VAL}" VAL)
 set(${OUTPUT_VARIABLE} "${VAL}" PARENT_SCOPE)
endfunction()

# Install a target's public headers, and nothing else. 
# The problem with install(..) is it also installs the target's binary (elf), 
# which is sometimes not wanted on a regular target.
# This installs the public headers, and nothing else :)
# 
# [IN] TARGET CMake target name.
# [IN] SRC_BASE_DIR Source base dir (typically CMAKE_CURRENT_SOURCE_DIR)
# [IN] INDEST_DEST_DIR Destination directory
function(install_target_headers TARGET SRC_BASE_DIR INSTALL_DEST_DIR)
  get_target_property(HEADERS ${TARGET} HEADER_SET_HEADERS)
  string(LENGTH "${SRC_BASE_DIR}/" LEN)
  foreach(HDR IN LISTS HEADERS)
    string(SUBSTRING "${HDR}" ${LEN} -1 REL_HDR)
    cmake_path(GET REL_HDR PARENT_PATH REL_HDR_DIRNAME)
    set(DEST "${INSTALL_DEST_DIR}/${REL_HDR_DIRNAME}")
    install(
      FILES ${HDR}
      DESTINATION ${DEST}
    )
  endforeach()
endfunction()
