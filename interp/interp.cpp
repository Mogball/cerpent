#include "interp.hpp"
#include "strutil.hpp"

#include <iostream>

#include <cassert>

static constexpr size_t LINEBUFFER_SIZE = 32;
static constexpr char SEMICOLON = ';';

Interpreter::Interpreter() :
    m_lineBuffer(LINEBUFFER_SIZE) {}

error_t Interpreter::processLine(std::string &&line) {
    string val(line);
    trim(&val);
    if (val.empty()) {
        return OK;
    }
    m_lineBuffer.push_back(move(val));
    if (SEMICOLON == m_lineBuffer.back().back()) {
        string buffer = join(m_lineBuffer, ' ');
        cout << buffer << endl;
    }

    assert(val.empty());

    return OK;
}
