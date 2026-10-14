cmake_minimum_required(VERSION 4.3)
project(BufferProcessor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(CoreProcessor STATIC src/processor.cpp)
target_include_directories(CoreProcessor PUBLIC ${PROJECT_SOURCE_DIR}/include)
set_target_properties(CoreProcessor PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    target_compile_options(CoreProcessor PRIVATE /O2 /W4)
else()
    target_compile_options(CoreProcessor PRIVATE -O3 -Wall -Wextra -Wpedantic)
endif()

find_package(node-addon-api QUIET)

execute_process(
    COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\"" "" NODE_ADDON_API_DIR "${NODE_ADDON_API_DIR}")

add_definitions(-DNAPI_VERSION=3)
include("${NODE_ADDON_API_DIR}/../node_addon_api.cmake" OPTIONAL)

add_library(NativeModule SHARED src/node_binding.cpp)
target_link_libraries(NativeModule PRIVATE CoreProcessor ${CMAKE_JS_LIB})
target_include_directories(NativeModule PRIVATE
    ${CMAKE_JS_INC}
    ${NODE_ADDON_API_DIR}
)

target_compile_definitions(NativeModule PRIVATE NAPI_DISABLE_CPP_EXCEPTIONS=0)
set_target_properties(NativeModule PROPERTIES
    PREFIX ""
    SUFFIX ".node"
)