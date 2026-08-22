file(GLOB app_sources "*.cc")

idf_component_register(
  SRCS ${app_sources}
  INCLUDE_DIRS
    "."
    "../include"
    "../lib/ztask"
    "../lib/zcli"
    "../lib/zi2c"
    "../lib/drv"
  REQUIRES u8g2 driver
)
