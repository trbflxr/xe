macro(add_msvc_pch pch_header pch_source sources)
  if (MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${pch_header} NAME_WE)
    set(pch_bin "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    set(src ${${sources}})

    set_source_files_properties(${pch_source}
        PROPERTIES COMPILE_FLAGS "/Yc\"${pch_header}\" /Fp\"${pch_bin}\""
        OBJECT_OUTPUTS "${pch_bin}")
    set_source_files_properties(${src}
        PROPERTIES COMPILE_FLAGS "/Yu\"${pch_header}\" /FI\"${pch_header}\" /Fp\"${pch_bin}\""
        OBJECT_DEPENDS "${pch_bin}")
    # Add precompiled header to sources
    list(APPEND ${sources} ${pch_source})
  endif ()
endmacro()
