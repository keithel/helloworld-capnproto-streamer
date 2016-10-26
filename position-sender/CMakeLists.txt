project(position-sender)
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
if(NOT DEFINED CapnProto_DIR)
  message("Setting CapnProto_DIR var to ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CapnProto")
  set (CapnProto_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CapnProto")
endif()

macro(makeLink src dest)
 add_custom_command(OUTPUT ${dest}
   COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest} DEPENDS ${src} COMMENT "mklink ${src} -> ${dest}")
endmacro()

makeLink(${CMAKE_CURRENT_SOURCE_DIR}/../position.capnp ${CMAKE_CURRENT_BINARY_DIR}/position.capnp)
set (CAPNPC_SRC_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")

find_package (CapnProto REQUIRED)
include_directories(${CAPNP_INCLUDE_DIRS})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${CMAKE_CURRENT_BINARY_DIR}/position.capnp)

aux_source_directory(. SRC_LIST)
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME} ${SRC_LIST} ${CAPNP_HDRS} ${CAPNP_SRCS})
target_link_libraries (${PROJECT_NAME} ${CAPNP_LIBRARIES_LITE})
