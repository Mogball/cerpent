#pragma once

#include "error.hpp"
#include <string>
#include <vector>

class Interpreter {
public:
    error_t processLine(std::string &&line);
    error_t processBuffer(const std::string &buf);

private:
    std::vector<std::string> m_lineBuffer;
};
