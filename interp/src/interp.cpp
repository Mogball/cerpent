#include "interp.hpp"
#include "strutil.hpp"
#include "validate.hpp"
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
    assert(val.empty());
    if (SEMICOLON == m_lineBuffer.back().back()) {
        string buf = join(m_lineBuffer, ' ');
        m_lineBuffer.clear();
        return processBuffer(buf);
    }
    return OK;
}

error_t Interpreter::processBuffer(const std::string &buf) {
    vector<string> toks = split(buf, ' ');
    assert(SEMICOLON == toks.back().back());

    // Remove semicolon from last token
    toks.back().pop_back();
    if (toks.back().empty()) {
        // Remove single semicolon token
        toks.pop_back();
    }
    // Line was only semicolon
    if (toks.empty()) {
        return OK;
    }

    cout << buf << endl;
    cout << '[';
    {
        auto it = toks.begin();
        cout << *it;
        ++it;
        for (; it != toks.end(); ++it) {
            cout << ", " << *it;
        }
    }
    cout << ']' << endl;

    // Check for variable declaration
    unsigned type;
    string_it name;
    string_it val;
    error_t err = isVariableDeclare(toks, &type, &name, &val);
    if (NOT_VARIABLE_DECL != err) {
        if (OK != err) {
            return err;
        }
        return declareVariable(type, *name, *val);
    }

    return INVALID_LINE;
}
