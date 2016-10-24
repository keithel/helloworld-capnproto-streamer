project(position-sender)
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")

find_package (CapnProto REQUIRED)
include_directories(${CAPNP_INCLUDE_DIRS})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ../position.capnp)

aux_source_directory(. SRC_LIST)
include_directories(..)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${CAPNP_HDRS} ${CAPNP_SRCS})
target_link_libraries (${PROJECT_NAME} ${CAPNP_LIBRARIES_LITE})
