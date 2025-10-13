cmake_minimum_required(VERSION 3.10)

project(XtenOjCode)

set(CMAKE_CXX_STANDARD 17)

#构建服务器框架
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Xten)

find_package(yaml-cpp REQUIRED)

include_directories(${YAML_CPP_INCLUDE_DIRS})
#头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Xten/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Xten/src)


add_library(XtenOjCode SHARED
    XtenOjCode/myModule.cpp
    XtenOjCode/ojServer/servlets/rootServlet.cpp
)
add_library(CompileRun SHARED
    XtenOjCode/rockModule.cpp

)
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