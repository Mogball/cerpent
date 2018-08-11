#pragma once

#include "error.hpp"

#include <string>
#include <vector>

class Interpreter {
public:
    Interpreter();

    error_t processLine(std::string &&line);

private:
    std::vector<std::string> m_lineBuffer;
};
