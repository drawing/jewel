cmake_minimum_required(VERSION 3.0)

project (UnitTests)

set(CMAKE_CXX_FLAGS " -L/Users/bodeng/local/lib/ -std=c++11")

include_directories(../hash/)
include_directories(../http/)
include_directories(${PROJECT_BINARY_DIR})

aux_source_directory (./hash/ HASH_TESTS_SRC)
aux_source_directory (../hash/ HASH_LIB_SRC)

set(PROTO_OUT test_node.pb.h test_node.pb.cc)
set(PROTO_IN ${CMAKE_CURRENT_SOURCE_DIR}/hash/test_node.proto)

add_custom_command(
	OUTPUT ${PROTO_OUT}
	COMMAND protoc --cpp_out=. -I${CMAKE_CURRENT_SOURCE_DIR}/hash/ ${PROTO_IN}
	DEPENDS ${PROTO_IN}
	)

add_custom_target(proto DEPENDS ${PROTO_OUT})


add_executable (HashUnitTests main.cpp ${HASH_TESTS_SRC} ${HASH_LIB_SRC})
add_dependencies(HashUnitTests proto)

