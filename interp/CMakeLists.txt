cmake_minimum_required(VERSION 3.1.0)
project(interp)

set(SOURCES
    interp.cpp
    main.cpp
    strutil.cpp
    validate_identifier.cpp
    validate_typename.cpp
    )
set(HEADERS
    const_hash.hpp
    error.hpp
    interp.hpp
    parameter.hpp
    strutil.hpp
    validate.hpp
    )
add_executable(interp ${SOURCES} ${HEADERS})
set_property(TARGET interp PROPERTY CXX_STANDARD 17)
