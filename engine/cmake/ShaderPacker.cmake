message(STATUS "Packing GL shaders")
message(STATUS "Out shader file: ${out_file}")

file(GLOB SHADERS
    ${SHADERS_DIR}/*.vert
    ${SHADERS_DIR}/*.frag
    ${SHADERS_DIR}/*.geom
    ${SHADERS_DIR}/*.tesc
    ${SHADERS_DIR}/*.tese)

file(WRITE ${out_file} "")
foreach (shader IN LISTS SHADERS)
  message(STATUS "\tProcessing: ${shader}")

  get_filename_component(s_name_raw ${shader} NAME)
  string(REPLACE "." "_" s_name ${s_name_raw})

  file(READ ${shader} shader_text)

  file(APPEND ${out_file} "static const char* ${s_name} = R\"(\n${shader_text})\";\n\n")
endforeach ()
