#pragma once

#include "error.hpp"
#include <string>
#include <vector>

class Interpreter {
public:
    error_t processLine(std::string &&line);
    error_t processBuffer(const std::string &buf);

    error_t declareVariable(
            unsigned type,
            const std::string &name,
            const std::string &val);

private:
    std::vector<std::string> m_lineBuffer;
};
