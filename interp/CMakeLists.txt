cmake_minimum_required(VERSION 3.1.0)
project(interp)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.hpp)
add_executable(interp ${SOURCES} ${HEADERS})
set_property(TARGET interp PROPERTY CXX_STANDARD 17)
