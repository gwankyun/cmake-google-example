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

# 引入庫
find_package(glog 0.4.0 REQUIRED)
set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
find_package(Protobuf CONFIG REQUIRED)
find_package(benchmark REQUIRED)
set(GTEST_ROOT "C:/Program Files (x86)/googletest-distribution")
find_package(GTest REQUIRED)

cmake_print_variables(glog_FOUND)
cmake_print_variables(Protobuf_FOUND)
cmake_print_variables(benchmark_FOUND)
cmake_print_variables(GTest_FOUND)

cmake_print_variables(Protobuf_PROTOC_EXECUTABLE)

protobuf_generate_cpp(proto_srcs proto_hdrs src/man.proto)
cmake_print_variables(proto_srcs)
cmake_print_variables(proto_hdrs)

set(man man)
add_library(${man})
target_sources(${man} PRIVATE ${proto_srcs} ${proto_hdrs})
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

set(benchmark benchmark)
add_executable(${benchmark})
target_sources(${benchmark} PRIVATE
  src/benchmark.cpp
  ${proto_hdrs})
target_link_libraries(${benchmark}
  benchmark::benchmark
  glog::glog
  ${man})
target_include_directories(${benchmark} PRIVATE build)
target_link_options(${benchmark} PRIVATE ${link_options})
