# This adds interface-only libraries (i.e group of libraries)
# to simplify linking of the PSP kernel stub libraries.

set(PSP_LIB_NAME_PREFIX "pspfw_")

#TODO add av (audio/video libs)

add_subdirectory(c)
add_subdirectory(crypto)
add_subdirectory(crypto_driver)
add_subdirectory(drm)
add_subdirectory(drm_driver)
# graphics / graphics_driver / psp_gu / psp_gum / psp_gum_vfpu
add_subdirectory(graphics)
add_subdirectory(io)
add_subdirectory(io_driver)
add_subdirectory(kernel)
# StdioForKernel / SysclibForKernel
add_subdirectory(kernel_c)
add_subdirectory(lowio_driver)
add_subdirectory(net)
add_subdirectory(net_adhoc)
add_subdirectory(net_apctl)
add_subdirectory(net_driver)
add_subdirectory(net_wlan)
add_subdirectory(net_wlan_driver)
add_subdirectory(np)
add_subdirectory(np_driver)
add_subdirectory(user)
add_subdirectory(util)
add_subdirectory(util_driver)
