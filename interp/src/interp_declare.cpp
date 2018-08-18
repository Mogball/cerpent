#include "interp.hpp"
#include "types.hpp"
#include <cassert>

#include <cstdio>

using namespace std;

error_t Interpreter::declareVariable(unsigned type, const string &name, const string &val) {
    assert(!val.empty());

    // Handle character literal
    if (CHAR == type && '\'' == val.front()) {
        if (3 != val.size() || '\'' != val.back()) {
            return INVALID_CHAR_LITERAL;
        }
        char ret = val.at(1);
        return OK;
    }

    // Parse integer or float
    char *end = nullptr;
    if (type <= SIGNED_LONG_LONG) {
        long long ret = strtoll(val.c_str(), &end, 10);
    } else if (type <= UNSIGNED_LONG_LONG) {
        unsigned long long ret = strtoull(val.c_str(), &end, 10);
    } else {
        long double ret = strtold(val.c_str(), &end);
    }
    if ('\0' != *end) {
        return type < FLOAT ? INVALID_INT_LITERAL : INVALID_FLOAT_LITERAL;
    }
    return OK;
}
