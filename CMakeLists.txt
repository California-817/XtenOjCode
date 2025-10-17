cmake_minimum_required(VERSION 3.10)

project(XtenOjCode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_STANDARD 17)

#构建服务器框架
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Xten)

find_package(yaml-cpp REQUIRED)
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)

include_directories(${YAML_CPP_INCLUDE_DIRS})
#头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Xten/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Xten/src)
include_directories("/usr/local/include")
link_directories("/usr/local/lib" "/usr/local/lib64")

# 定义通用库集合
set(COMMON_LIBS
    absl::check
    absl::flags
    absl::flags_parse
    absl::log
    absl::log_initialize
    ${_PROTOBUF_LIBPROTOBUF})

add_library(XtenOjCode SHARED
    XtenOjCode/util.cpp
    XtenOjCode/myModule.cpp
    XtenOjCode/rockClient.cpp
    XtenOjCode/ojServer/servlets/rootServlet.cpp
    XtenOjCode/proto/compileRun.pb.cc
)
add_library(CompileRun SHARED
    XtenOjCode/util.cpp
    XtenOjCode/rockModule.cpp
    XtenOjCode/compileRunServer/compile.cpp
    XtenOjCode/compileRunServer/run.cpp
    XtenOjCode/proto/compileRun.pb.cc
)
target_link_libraries(XtenOjCode
    ${COMMON_LIBS})
target_link_libraries(CompileRun
    ${COMMON_LIBS})

set_target_properties(XtenOjCode PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set_target_properties(XtenOjCode PROPERTIES
    OUTPUT_NAME "XtenOjCode"
)
set_target_properties(CompileRun PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set_target_properties(CompileRun PROPERTIES
    OUTPUT_NAME "CompileRun"
)