cmake_minimum_required(VERSION 3.1.0)
project(interp)

file(GLOB SOURCES src/*.cpp src/*.c)
file(GLOB HEADERS src/*.hpp src/*.h)

add_executable(interp ${SOURCES} ${HEADERS})
set_property(TARGET interp PROPERTY CXX_STANDARD 17)
