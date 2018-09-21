#include "interp.hpp"
#include "strutil.hpp"
#include <iostream>
#include <cassert>

using namespace std;

static constexpr char SEMICOLON = ';';

error_t Interpreter::processLine(string &&line) {
    string val(move(line));
    trim(&val);
    if (val.empty()) {
        return OK;
    }
    m_lineBuffer.push_back(move(val));
    assert(val.empty());
    if (SEMICOLON == m_lineBuffer.back().back()) {
        string buf = join(m_lineBuffer, ' ');
        m_lineBuffer.clear();
        return processBuffer(buf);
    }
    return OK;
}

error_t Interpreter::processBuffer(const string &buf) {
    vector<string> toks = split(buf, ';');
    error_t ret;
    for (string &unit : toks) {
        trim(&unit);
        unit += ';';
        ret = processUnit(unit);
        if (OK != ret) {
            return ret;
        }
    }

    return OK;
}
