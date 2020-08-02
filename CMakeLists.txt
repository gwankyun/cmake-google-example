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

set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")

find_package(glog 0.4.0 REQUIRED)

find_package(Protobuf CONFIG REQUIRED)

cmake_print_variables(Protobuf_PROTOC_EXECUTABLE)

protobuf_generate_cpp(proto_srcs proto_hdrs src/test.proto)
cmake_print_variables(proto_srcs)
cmake_print_variables(proto_hdrs)

set(GTEST_ROOT "C:/Program Files (x86)/googletest-distribution")
find_package(GTest REQUIRED)
cmake_print_variables(GTEST_FOUND)

# 可執行文件
set(main main)
add_executable(${main})

set(compile_options
  "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/MTd;/Od;/Ob0;>"
  "$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/MT;/O2;/Ob2;>")

# 編譯器選項
target_compile_options(${main} PUBLIC ${compile_options})

# 源文件
target_sources(${main} PRIVATE src/main.cpp ${proto_srcs} ${proto_hdrs})

# 引用庫
target_link_libraries(${main} glog::glog protobuf::libprotobuf protobuf::libprotoc)

set(test test)
add_executable(${test})
target_sources(${test} PRIVATE
  src/test/test.cpp
  src/test/case.h
  src/test/case.cpp
  ${proto_srcs} ${proto_hdrs})
target_compile_options(${test} PUBLIC ${compile_options})
target_link_libraries(${test}
  GTest::GTest GTest::Main
  protobuf::libprotobuf protobuf::libprotoc)

find_package(benchmark REQUIRED)
set(benchmark benchmark)
add_executable(${benchmark})
target_sources(${benchmark} PRIVATE
  src/benchmark.cpp)
target_link_libraries(${benchmark} benchmark::benchmark)
