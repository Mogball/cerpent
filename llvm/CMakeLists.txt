cmake_minimum_required(VERSION 3.1.0)
project(cerp-llvm VERSION 1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cotire/CMake/cotire.cmake)

find_package(LLVM 6.0 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB SRCS src/*.cpp)
file(GLOB HDRS src/*.hpp)

add_executable(cerp ${SRCS} ${HDRS})

llvm_map_components_to_libnames(LLVM_LIBS
    native
    option
    )

target_link_libraries(cerp
    clangTooling
    clangFrontend
    clangDriver
    clangSerialization
    clangParse
    clangSema
    clangAnalysis
    clangEdit
    clangAST
    clangLex
    clangBasic
    ${LLVM_LIBS}
    )

cotire(cerp)
