# The additional / is important to remove the last character from the path.
# Note that it does not matter if the OS uses / or \, because we are only
# saving the path size.
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
# add_definitions("-D__FILENAME__=__FILE__ + SOURCE_PATH_SIZE")
# C code for it:
# ----
# /* Source file name (see CMakeLists for SOURCE_PATH_SIZE definition).
#    This is portable, but still includes the full paths in the binary. */
# #undef  __FILENAME__
# #define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)
# ----

# Relative path for __FILE__.
# Only for GCC 8 and upwards.
add_compile_options(
    -fmacro-prefix-map=${CMAKE_CURRENT_LIST_DIR}/=
    -ffile-prefix-map=${CMAKE_CURRENT_LIST_DIR}/=
    -fdebug-prefix-map=${CMAKE_CURRENT_LIST_DIR}/=
)
