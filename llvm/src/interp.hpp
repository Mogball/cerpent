#pragma once

#include "error.h"
#include <string>
#include <vector>

class InterpreterJit;
namespace clang {
    class ASTContext;
}

class Interpreter {
public:
    Interpreter(int argc, char *argv[]);
    ~Interpreter();

    error_t processLine(std::string &&line);
    error_t processBuffer(const std::string &buf, std::string *remain);
    error_t processUnit(const std::string &unit);
    error_t processScript(const std::string &script);

    error_t declareVariable(
            unsigned type,
            const std::string &name,
            const std::string &val);

    bool isBufferEmpty();
    int getIndent();

private:
    std::shared_ptr<InterpreterJit> m_jit;
    std::vector<std::string> m_lineBuffer;
    int m_balance;
    bool m_unbalanced;
};
