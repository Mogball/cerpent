#include "error.h"
#include <string>
#include <cassert>
#include <cctype>

using namespace std;

static constexpr char UNDERSCORE = '_';

static inline bool isValidStart(char c) {
    return isalpha(c) || UNDERSCORE == c;
}

static inline bool isValidChar(char c) {
    return isalnum(c) || UNDERSCORE == c;
}

error_t isIdentifier(const std::string &s) {
    assert(!s.empty());
    auto it = s.begin();
    if (!isValidStart(*it)) {
        return INVALID_IDENTIFIER_START;
    }
    ++it;
    for (; it != s.end(); ++it) {
        if (!isValidChar(*it)) {
            return INVALID_IDENTIFIER;
        }
    }
    return OK;
}
