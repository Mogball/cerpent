#pragma once

#include "error.hpp"
#include <string>
#include <vector>

error_t isIdentifier(const std::string &s);
error_t parseType(
        std::vector<std::string>::const_iterator it,
        std::vector<std::string>::const_iterator end);
