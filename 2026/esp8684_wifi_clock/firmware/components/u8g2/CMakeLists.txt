# Wrapper around the shared u8g2 tree. Do not point EXTRA_COMPONENT_DIRS at
# D:/work/3rdparty/u8g2 — that CMakeLists.txt calls project() and would also
# overwrite PIO's EXTRA_COMPONENT_DIRS (src + Arduino).
set(U8G2_DIR "D:/work/3rdparty/u8g2/csrc")
file(GLOB u8g2_srcs "${U8G2_DIR}/*.c")

idf_component_register(
  SRCS ${u8g2_srcs}
  INCLUDE_DIRS ${U8G2_DIR}
)
