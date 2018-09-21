#pragma once

#include <clang/AST/APValue.h>
#include <string>
#include <map>

class SymbolTable {
public:
    bool addSymbol(const std::string &id, clang::APValue *val);

private:
    std::map<std::string, clang::APValue> m_table;
};
