#include "interp.hpp"
#include "strutil.hpp"

#include <iostream>

#include <cassert>

using namespace std;

static constexpr char SEMICOLON = ';';

error_t Interpreter::processLine(std::string &&line) {
    string val(move(line));
    trim(&val);
    if (val.empty()) {
        return OK;
    }
    m_lineBuffer.push_back(move(val));
    if (SEMICOLON == m_lineBuffer.back().back()) {
        string buffer = join(m_lineBuffer, ' ');
        cout << buffer << endl;
        m_lineBuffer.clear();
    }
    assert(val.empty());
    return OK;
}
