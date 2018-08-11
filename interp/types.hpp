#pragma once

enum Type : unsigned {
    CHAR = 0x0000,

    SIGNED_CHAR  = 0x0100,
    SIGNED_SHORT = 0x0101,
    SIGNED_INT   = 0x0102,
    SIGNED_LONG  = 0x0103,
    SIGNED_LONG_LONG = 0x0104,

    UNSIGNED_CHAR  = 0x0200,
    UNSIGNED_SHORT = 0x0201,
    UNSIGNED_INT   = 0x0202,
    UNSIGNED_LONG  = 0x0203,
    UNSIGNED_LONG_LONG = 0x0204,

    FLOAT = 0x0300,

    DOUBLE = 0x0400,
    LONG_DOUBLE = 0x0401
}
