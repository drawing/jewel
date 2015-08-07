cmake_minimum_required(VERSION 2.8)

project (UnitTests)

set(CMAKE_CXX_FLAGS " -L/Users/bodeng/local/lib/ -std=c++11")

include_directories(../hash/)
include_directories(../http/)
include_directories(../encoder/)
include_directories(../leveldb_wrapper/)
include_directories(${PROJECT_BINARY_DIR})

aux_source_directory (./hash/ HASH_TESTS_SRC)
aux_source_directory (../hash/ HASH_LIB_SRC)

set(PROTO_OUT test_node.pb.h test_node.pb.cc)
set(PROTO_IN ${CMAKE_CURRENT_SOURCE_DIR}/pbnode/test_node.proto)

add_custom_command(
	OUTPUT ${PROTO_OUT}
	COMMAND protoc --cpp_out=. -I${CMAKE_CURRENT_SOURCE_DIR}/pbnode/ ${PROTO_IN}
	DEPENDS ${PROTO_IN}
	)

add_custom_target(proto DEPENDS ${PROTO_OUT})


add_executable (HashUnitTests main.cpp ${HASH_TESTS_SRC} ${HASH_LIB_SRC} test_node.pb.cc)
add_dependencies(HashUnitTests proto)

target_link_libraries(HashUnitTests gtest pthread protobuf)

aux_source_directory (./http/ HTTP_TESTS_SRC)
aux_source_directory (../http/ HTTP_LIB_SRC)

add_executable (HttpUnitTests main.cpp ${HTTP_TESTS_SRC} ${HTTP_LIB_SRC})
target_link_libraries(HttpUnitTests gtest pthread)

aux_source_directory (./leveldb_wrapper/ LEVELDB_TESTS_SRC)
aux_source_directory (../leveldb_wrapper/ LEVELDB_LIB_SRC)

add_executable (LevelDBUnitTests main.cpp ${LEVELDB_TESTS_SRC} ${LEVELDB_LIB_SRC} test_node.pb.cc)
target_link_libraries(LevelDBUnitTests gtest pthread leveldb protobuf)

aux_source_directory (./encoder/ ENCODER_TESTS_SRC)

add_executable (EncoderUnitTests main.cpp ${ENCODER_TESTS_SRC} test_node.pb.cc)
target_link_libraries(EncoderUnitTests gtest pthread protobuf)

