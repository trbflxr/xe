message(STATUS "Packing GL shaders")
message(STATUS "Out dir: ${OUT_DIR}")

set(OUT_FILE ${OUT_DIR}/shaders.hpp)

file(GLOB SHADERS
    ${SHADERS_DIR}/*.glsl
    ${SHADERS_DIR}/*.vert
    ${SHADERS_DIR}/*.frag
    ${SHADERS_DIR}/*.geom
    ${SHADERS_DIR}/*.tesc
    ${SHADERS_DIR}/*.tese)

file(WRITE ${OUT_FILE} "#ifndef XE_DEFAULT_SHADERS_HPP\n#define XE_DEFAULT_SHADERS_HPP\nnamespace xe {\n")
foreach (shader IN LISTS SHADERS)
  message(STATUS "\tProcessing: ${shader}")

  get_filename_component(s_name_raw ${shader} NAME)
  string(REPLACE "." "_" s_name ${s_name_raw})

  file(READ ${shader} shader_text)

  file(APPEND ${OUT_FILE} "static const char* ${s_name} = R\"(\n${shader_text})\";\n")
endforeach ()
file(APPEND ${OUT_FILE} "}\n#endif")
