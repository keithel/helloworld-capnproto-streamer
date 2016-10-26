find_path(CAPNP_INCLUDE_DIRS capnp/generated-header-support.h HINTS "/usr/include")
find_library(CAPNP_LIB_KJ NAMES kj HINTS "/usr/lib/x86_64-linux-gnu")
find_library(CAPNP_LIB_KJ-ASYNC NAMES kj-async HINTS "/usr/lib/x86_64-linux-gnu")
find_library(CAPNP_LIB_CAPNP NAMES capnp HINTS "/usr/lib/x86_64-linux-gnu")
find_library(CAPNP_LIB_CAPNP-RPC NAMES capnp-rpc "/usr/lib/x86_64-linux-gnu")
mark_as_advanced(CAPNP_LIB_KJ CAPNP_LIB_KJ-ASYNC CAPNP_LIB_CAPNP CAPNP_LIB_CAPNP-RPC)

set(CAPNP_LIBRARIES_LITE
    ${CAPNP_LIB_KJ}
    ${CAPNP_LIB_CAPNP})

set(CAPNP_LIBRARIES
    ${CAPNP_LIBRARIES_LITE}
    ${CAPNP_LIB_KJ-ASYNC}
    ${CAPNP_LIB_CAPNP-RPC})

find_program(CAPNP_EXECUTABLE
    NAMES capnp
    DOC "Cap'n Proto Command-line Tool"
    HINTS "/usr/bin")

find_program(CAPNPC_CXX_EXECUTABLE
    NAMES capnpc-c++
    DOC "Cap'n Proto C++ Compiler"
    HINTS "/usr/bin")

function(CAPNP_GENERATE_CPP SOURCES HEADERS)
  if(NOT ARGN)
    message(SEND_ERROR "CAPNP_GENERATE_CPP() called without any source files.")
  endif()
  if(NOT CAPNP_EXECUTABLE)
    message(SEND_ERROR "Could not locate capnp executable (CAPNP_EXECUTABLE).")
  endif()
  if(NOT CAPNPC_CXX_EXECUTABLE)
    message(SEND_ERROR "Could not locate capnpc-c++ executable (CAPNPC_CXX_EXECUTABLE).")
  endif()
  if(NOT CAPNP_INCLUDE_DIRS)
    message(SEND_ERROR "Could not locate capnp header files (CAPNP_INCLUDE_DIRS).")
  endif()

  # Default compiler includes
  set(include_path -I ${CMAKE_CURRENT_SOURCE_DIR} -I ${CAPNP_INCLUDE_DIRS})

  if(DEFINED CAPNPC_IMPORT_DIRS)
    # Append each directory as a series of '-I' flags in ${include_path}
    foreach(directory ${CAPNPC_IMPORT_DIRS})
      get_filename_component(absolute_path "${directory}" ABSOLUTE)
      list(APPEND include_path -I ${absolute_path})
    endforeach()
  endif()

  if(DEFINED CAPNPC_OUTPUT_DIR)
    # Prepend a ':' to get the format for the '-o' flag right
    set(output_dir ":${CAPNPC_OUTPUT_DIR}")
  else()
    set(output_dir ":.")
  endif()

  if(NOT DEFINED CAPNPC_SRC_PREFIX)
    set(CAPNPC_SRC_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
  endif()
  get_filename_component(CAPNPC_SRC_PREFIX "${CAPNPC_SRC_PREFIX}" ABSOLUTE)

  set(${SOURCES})
  set(${HEADERS})
  foreach(schema_file ${ARGN})
    get_filename_component(file_path "${schema_file}" ABSOLUTE)
    get_filename_component(file_dir "${file_path}" PATH)

    # Figure out where the output files will go
    if (NOT DEFINED CAPNPC_OUTPUT_DIR)
      set(output_base "${file_path}")
    else()
      # Output files are placed in CAPNPC_OUTPUT_DIR, at a location as if they were
      # relative to CAPNPC_SRC_PREFIX.
      string(LENGTH "${CAPNPC_SRC_PREFIX}" prefix_len)
      string(SUBSTRING "${file_path}" 0 ${prefix_len} output_prefix)
      if(NOT "${CAPNPC_SRC_PREFIX}" STREQUAL "${output_prefix}")
        message(SEND_ERROR "Could not determine output path for '${schema_file}' ('${file_path}') with source prefix '${CAPNPC_SRC_PREFIX}' into '${CAPNPC_OUTPUT_DIR}'.")
      endif()

      string(SUBSTRING "${file_path}" ${prefix_len} -1 output_path)
      set(output_base "${CAPNPC_OUTPUT_DIR}${output_path}")
    endif()

    add_custom_command(
      OUTPUT "${output_base}.c++" "${output_base}.h"
      COMMAND "${CAPNP_EXECUTABLE}"
      ARGS compile
          -o ${CAPNPC_CXX_EXECUTABLE}${output_dir}
          --src-prefix ${CAPNPC_SRC_PREFIX}
          ${include_path}
          ${CAPNPC_FLAGS}
          ${file_path}
      DEPENDS "${schema_file}"
      COMMENT "Compiling Cap'n Proto schema ${schema_file}"
      VERBATIM
    )
    list(APPEND ${SOURCES} "${output_base}.c++")
    list(APPEND ${HEADERS} "${output_base}.h")
  endforeach()

  set_source_files_properties(${${SOURCES}} ${${HEADERS}} PROPERTIES GENERATED TRUE)
  set(${SOURCES} ${${SOURCES}} PARENT_SCOPE)
  set(${HEADERS} ${${HEADERS}} PARENT_SCOPE)
endfunction()

