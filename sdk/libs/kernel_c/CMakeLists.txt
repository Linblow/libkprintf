set(LIBNAME "${PSP_LIB_NAME_PREFIX}kernel_c")
add_library(${LIBNAME} INTERFACE)

# StdioForKernel has a bunch of C functions:
# fdgetc, fdgets, fdputc, fdputs, fdprintf, 
# getchar, gets, putchar, puts, printf

target_link_libraries(${LIBNAME} INTERFACE
  StdioForKernel
  SysclibForKernel
)
