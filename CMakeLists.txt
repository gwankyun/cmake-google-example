# cmake最低版本
cmake_minimum_required(VERSION 3.18)

# 項目名
set(project cmake-google-example)
project(${project})

# 包含模塊
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakePrintHelpers)

# 啟用IDE目錄功能
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 輸出目錄
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(link_options "$<$<CXX_COMPILER_ID:MSVC>:/ignore:4099>")

cmake_print_variables(CMAKE_SYSTEM_PREFIX_PATH)

set(GTEST_ROOT "C:/Program Files (x86)/googletest-distribution")

# 引入庫
find_package(glog 0.4.0 REQUIRED)
set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
find_package(Protobuf CONFIG REQUIRED)
find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(Boost_DIR "C:/local/boost_1_74_0/lib64-msvc-14.2/cmake/Boost-1.74.0")

find_package(Boost 1.74.0 REQUIRED COMPONENTS serialization)

if(Boost_FOUND)
  cmake_print_variables(Boost_INCLUDE_DIRS)
  cmake_print_variables(Boost_LIBRARY_DIRS)
  cmake_print_variables(Boost_LIBRARIES)
  cmake_print_variables(Boost_LIB_VERSION)
else()
  message(FATAL_ERROR "Boost not found")
endif()

cmake_print_variables(glog_FOUND)
cmake_print_variables(Protobuf_FOUND)
cmake_print_variables(Protobuf_VERSION)
cmake_print_variables(benchmark_FOUND)
cmake_print_variables(GTest_FOUND)
cmake_print_variables(gRPC_FOUND)
cmake_print_variables(gRPC_INCLUDE_DIRS)

cmake_print_variables(Protobuf_PROTOC_EXECUTABLE)

protobuf_generate_cpp(man_proto_srcs man_proto_hdrs src/man.proto)
cmake_print_variables(man_proto_srcs)
cmake_print_variables(man_proto_hdrs)

# Proto file
get_filename_component(hw_proto src/helloworld.proto ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")
add_custom_command(
  OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
  COMMAND "$<TARGET_FILE:protobuf::protoc>"
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${hw_proto_path}"
    --plugin=protoc-gen-grpc="$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
    "${hw_proto}"
  DEPENDS "${hw_proto}")

cmake_print_variables(hw_proto_srcs)
cmake_print_variables(hw_proto_hdrs)
cmake_print_variables(hw_grpc_srcs)
cmake_print_variables(hw_grpc_hdrs)

set(man man)
add_library(${man})
target_sources(${man} PRIVATE ${man_proto_srcs} ${man_proto_hdrs})
target_link_libraries(${man} protobuf::libprotobuf protobuf::libprotoc)
set_target_properties(${man} PROPERTIES FOLDER proto)
target_link_options(${man} PRIVATE ${link_options})

# 編譯器選項
add_compile_options(
  "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/W4;/MP;/Od;/Ob0;>"
  "$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/W4;/MP;/O2;/Ob2;>")

set(main main)
# 可執行文件
add_executable(${main})
# 源文件
target_sources(${main} PRIVATE src/main.cpp ${proto_hdrs})
# 引用庫
target_link_libraries(${main} glog::glog ${man})
# 引用目錄
target_include_directories(${main} PRIVATE build)
target_link_options(${main} PRIVATE ${link_options})
target_compile_options(${main}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4100;>")

set(test test)
add_executable(${test})
target_sources(${test} PRIVATE
  src/test/test.cpp
  src/test/case.h
  src/test/case.cpp
  ${proto_hdrs})
target_link_libraries(${test}
  GTest::GTest GTest::Main
  ${man})
target_include_directories(${test} PRIVATE build)
target_link_options(${test} PRIVATE ${link_options})
target_compile_options(${test}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4100;>")

set(benchmark benchmark)
add_executable(${benchmark})
target_sources(${benchmark} PRIVATE
  src/benchmark.cpp
  ${proto_hdrs})
target_link_libraries(${benchmark}
  benchmark::benchmark
  glog::glog
  ${man}
  Boost::serialization)
target_include_directories(${benchmark} PRIVATE
  build
    include)
target_link_options(${benchmark} PRIVATE ${link_options})
target_compile_options(${benchmark}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4100;/wd4505;>")

set(hw_proto hw_proto)
add_library(${hw_proto})
target_sources(${hw_proto} PRIVATE
  ${hw_proto_srcs} ${hw_proto_hdrs})
target_link_libraries(${hw_proto}
  PUBLIC
    gRPC::grpc++ gRPC::grpc++_reflection)
set_target_properties(${hw_proto} PROPERTIES FOLDER grpc)
target_compile_options(${hw_proto}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4127;/wd4100;>")

set(hw_grpc hw_grpc)
add_library(${hw_grpc})
target_sources(${hw_grpc} PRIVATE
  ${hw_grpc_srcs} ${hw_proto_hdrs})
target_link_libraries(${hw_grpc}
  PUBLIC
    gRPC::grpc++ gRPC::grpc++_reflection)
set_target_properties(${hw_grpc} PROPERTIES FOLDER grpc)
target_compile_options(${hw_grpc}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4127;/wd4100;/wd4267;>")

set(server server)
add_executable(${server})
target_sources(${server} PRIVATE
  src/greeter_server.cc
)
target_link_libraries(${server}
  PUBLIC
    ${hw_proto}
    ${hw_grpc}
)
target_include_directories(${server} PUBLIC build)
set_target_properties(${server} PROPERTIES FOLDER grpc)
target_compile_options(${server}
    PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4127;/wd4100;>")

set(client client)
add_executable(${client})
target_sources(${client} PRIVATE
  src/greeter_client.cc
)
target_link_libraries(${client}
  PUBLIC
    ${hw_proto}
    ${hw_grpc}
)
target_include_directories(${client} PUBLIC build)
set_target_properties(${client} PROPERTIES FOLDER grpc)
target_compile_options(${client}
  PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4127;/wd4100;>")
