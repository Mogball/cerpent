#pragma once

typedef int error_t;
enum Error : error_t {
    // IDENTIFIER
    INVALID_IDENTIFIER =       0x0100,
    INVALID_IDENTIFIER_START = 0x0101,

    // PARSE TYPE
    INVALID_SIZE_QUALIFIER = 0x0200,
    INVALID_SIGN_QUALIFIER = 0x0201,
    UNEXPECTED_TYPE_TOKEN  = 0x0202,


    // OK
    OK = 0x0
};
