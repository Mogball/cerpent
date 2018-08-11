cmake_minimum_required(VERSION 3.1.0)
project(interp)

set(SOURCES
    main.cpp
    interp.cpp
    strutil.cpp
    )
set(HEADERS
    error.hpp
    interp.hpp
    strutil.hpp
    )
add_executable(interp ${SOURCES} ${HEADERS})
set_property(TARGET interp PROPERTY CXX_STANDARD 17)
