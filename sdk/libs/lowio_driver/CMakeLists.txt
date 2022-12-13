set(LIBNAME "${PSP_LIB_NAME_PREFIX}lowio_driver")
add_library(${LIBNAME} INTERFACE)

target_link_libraries(${LIBNAME} INTERFACE
  sceClockgen_driver
  sceDmacplus_driver
  sceDdr_driver
  sceGpio_driver
  sceI2c_driver
  sceLcdc_driver
  sceNand_driver
  scePwm_driver
  sceSyscon_driver
  sceSysreg_driver
)
