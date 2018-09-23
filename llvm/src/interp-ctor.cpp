#include "interp.hpp"
#include "interp-jit.hpp"

Interpreter::Interpreter(int argc, char *argv[]) :
    m_jit(std::make_shared<InterpreterJit>(argc, argv)),
    m_balance(0),
    m_unbalanced(false) {}

Interpreter::~Interpreter() = default;
