cmake_minimum_required(VERSION 3.1.0)
project(cerp-llvm VERSION 1.0 LANGUAGES CXX)

find_package(LLVM 6.0 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB SRCS src/*.cpp)
file(GLOB HDRS src/*.hpp)

add_executable(cerp ${SRCS} ${HDRS})

llvm_map_components_to_libnames(LLVM_LIBS
    core
    executionengine
    native
    object
    orcjit
    runtimedyld
    support
    option
    )

target_link_libraries(cerp
    clangTooling
    clangFrontend
    clangDriver
    clangSerialization
    clangCodeGen
    clangParse
    clangSema
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangAnalysis
    clangARCMigrate
    clangRewrite
    clangRewriteFrontend
    clangEdit
    clangAST
    clangLex
    clangBasic
    clang
    ${LLVM_LIBS}
    )
