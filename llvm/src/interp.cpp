#include "interp.hpp"
#include "strutil.hpp"
#include <iostream>
#include <cassert>

using namespace std;

error_t Interpreter::processLine(string &&line) {
    string val(move(line));
    trim(&val);
    if (val.empty()) {
        return OK;
    }
    for (char c : val) {
        if ('{' == c || '(' == c || '[' == c) {
            m_unbalanced = true;
            ++m_balance;
        } else if ('}' == c || ')' == c || ']' == c) {
            --m_balance;
        }
    }
    m_lineBuffer.push_back(move(val));
    assert(val.empty());
    if (0 == m_balance && (m_unbalanced || ';' == m_lineBuffer.back().back())) {
        m_unbalanced = false;
        string buf = join(m_lineBuffer, ' ');
        m_lineBuffer.clear();
        string remain;
        error_t ret = processBuffer(buf, &remain);
        trim(&remain);
        if (!remain.empty()) {
            m_lineBuffer.push_back(move(remain));
        }
    }
    return OK;
}

error_t Interpreter::processBuffer(const string &buf, string *remain) {
    int balance = 0;
    bool unbalanced = false;
    bool block = false;
    string unit;
    error_t ret = OK;
    for (char c : buf) {
        if ('{' == c || '(' == c) {
            unbalanced = true;
            ++balance;
        } else if ('}' == c || ')' == c) {
            --balance;
        }
        if (0 == balance && ')' == c) {
            block = true;
        }
        if (0 == balance && '}' == c) {
            block = false;
        }
        if (0 == balance && ';' == c) {
            block = false;
        }
        unit += c;
        if (0 == balance && (unbalanced || ';' == c) && !block) {
            if (';' != unit.back()) {
                unit += ';';
            }
            ret = processUnit(unit);
            unit.clear();
            if (OK != ret) {
                break;
            }
        }
    }
    *remain = unit;
    return ret;
}

bool Interpreter::isBufferEmpty() {
    return m_lineBuffer.empty();
}

int Interpreter::getIndent() {
    return m_balance;
}
