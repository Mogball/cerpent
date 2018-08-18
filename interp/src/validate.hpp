#pragma once

#include "error.h"
#include <string>
#include <vector>

typedef std::vector<std::string> string_vec;
typedef string_vec::const_iterator string_it;

error_t isIdentifier(const std::string &s);
error_t isTypeStart(const std::string &s);
error_t parseType(string_it it, string_it end, unsigned *ret);

error_t isVariableDeclare(
        const string_vec &toks,
        unsigned *type,
        string_it *name,
        string_it *val);
