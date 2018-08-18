#include "macro.h"
#include "const_hash.hpp"
#include "const_sort.hpp"
#include "error.h"
#include "strutil.hpp"
#include "to_array.hpp"
#include "validate.hpp"
#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

#define make_hash(x) const_hash(str(x))

using namespace std;

static constexpr uint32_t s_builtinTypes[] = {
    make_hash(char),
    make_hash(int),
    make_hash(float),
    make_hash(double)
};

static constexpr uint32_t s_signedTypes[] = {
    make_hash(signed),
    make_hash(unsigned)
};

static constexpr uint32_t s_sizeTypes[] = {
    make_hash(short),
    make_hash(long)
};

static constexpr auto s_builtinTypesSorted = const_sort(to_array(s_builtinTypes));
static constexpr auto s_signedTypesSorted  = const_sort(to_array(s_signedTypes));
static constexpr auto s_sizeTypesSorted    = const_sort(to_array(s_sizeTypes));

enum Sign {
    NONE     = 0,
    SIGNED   = 1,
    UNSIGNED = 2
};

enum Size {
    SHORT     = -1,
    NORMAL    =  0,
    LONG      =  1,
    LONG_LONG =  2
};

enum Type : unsigned {
    CHAR   = 0b0100,
    INT    = 0b1101,
    FLOAT  = 0b0010,
    DOUBLE = 0b1011
};

template<size_t N>
static inline uint32_t valueLookup(const array<uint32_t, N> &arr, uint32_t val) {
    for (const auto &el : arr) {
        if (el == val) {
            return el;
        }
    }
    return 0;
}

static inline uint32_t builtinType(uint32_t val) {
    return valueLookup(s_builtinTypesSorted, val);
}

static inline uint32_t signedType(uint32_t val) {
    return valueLookup(s_signedTypesSorted, val);
}

static inline uint32_t sizeType(uint32_t val) {
    return valueLookup(s_sizeTypesSorted, val);
}


error_t isTypeStart(const std::string &s) {
    uint32_t val = det::fnv1a_32(s.c_str(), s.size());
    bool found =
        0 != builtinType(val) ||
        0 != signedType(val) ||
        0 != sizeType(val);
    return found ? OK : INVALID_TYPENAME;
}

error_t parseType(string_it it, string_it end, unsigned *ret) {
    assert(nullptr != ret);
    Sign sign = NONE;
    Size size = NORMAL;
    Type type = INT;
    size_t i = 0;
    for (; it != end; ++it, ++i) {
        uint32_t val = det::fnv1a_32(it->c_str(), it->size());
        uint32_t tok = 0;
        switch (i) {
        case 0:
            if (0 != (tok = signedType(val))) {
                sign = tok == s_signedTypes[1] ? UNSIGNED : SIGNED;
                break;
            }
            ++i;
            // fallthrough
        case 1:
            if (0 != (tok = sizeType(val))) {
                size = tok == s_sizeTypes[1] ? LONG : SHORT;
                break;
            }
            ++i;
            // fallthrough
        case 2:
            if (0 != (tok = sizeType(val))) {
                if (tok != s_sizeTypes[1] || size != LONG) {
                    return INVALID_SIZE_QUALIFIER;
                }
                size = LONG_LONG;
                break;
            }
            ++i;
            // fallthrough
        case 3:
            if (0 != (tok = builtinType(val))) {
                if (tok == s_builtinTypes[0]) {
                    type = CHAR;
                } else if (tok == s_builtinTypes[2]) {
                    type = FLOAT;
                } else if (tok == s_builtinTypes[3]) {
                    type = DOUBLE;
                }
                break;
            }
            // fallthrough
        default:
            return UNEXPECTED_TYPE_TOKEN;
        }
    }
    if (NONE != sign) {
        if (0 == type & 0b0100) {
            return INVALID_SIGN_QUALIFIER;
        }
    } else if (CHAR != type) {
        sign = SIGNED;
    }
    if (NORMAL != size) {
        if (0 == type & 0b1000 ||
                (LONG != size && DOUBLE == type)) {
            return INVALID_SIZE_QUALIFIER;
        }
    }
    switch (type) {
    case CHAR:
        *ret = sign << 8;
        break;
    case INT:
        *ret = (sign << 8) | (0x02 + size);
        break;
    case FLOAT:
        *ret = 0x0300;
        break;
    case DOUBLE:
        *ret = 0x0400 | size;
    }
    return OK;
}
