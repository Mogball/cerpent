cmake_minimum_required(VERSION 3.1.0)
project(cerp-llvm VERSION 1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cotire/CMake/cotire.cmake)

find_package(LLVM 6.0 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB SRCS src/*.cpp)
file(GLOB HDRS src/*.hpp)

add_executable(cerp ${SRCS} ${HDRS})

macro (map_target_libs target_name target_libs)
    foreach (library ${target_libs})
        llvm_map_components_to_libnames(target_lib "${target_name}${library}")
        set(TARGET_LIBS ${TARGET_LIBS} ${target_lib})
    endforeach ()
endmacro ()

set(TARGET_LIBS)
foreach (target aarch64 amdgpu arm bpf hexagon lanai mips powerpc sparc systemz x86)
    map_target_libs(${target} "asmparser;codegen")
endforeach ()
foreach (target xcore nvptx msp430)
    map_target_libs(${target} "asmprinter;codegen")
endforeach ()

llvm_map_components_to_libnames(LLVM_LIBS
    coroutines
    coverage
    lto
    option
    )

target_link_libraries(cerp
    clangFrontendTool
    clangFrontend
    clangParse
    clangSema
    clangSerialization
    clangTooling
    clangDriver
    clangCodeGen
    clangBasic
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangAnalysis
    clangAST
    clangASTMatchers
    clangEdit
    clangARCMigrate
    clangLex
    clangRewriteFrontend
    clangRewrite
    ${LLVM_LIBS}
    ${TARGET_LIBS}
    )

target_include_directories(cerp PRIVATE include)

cotire(cerp)
