#include "symboltable.hpp"
#include <iostream>

using namespace std;
using namespace clang;

bool SymbolTable::addSymbol(const string &id, APValue *val) {
    if (m_table.end() != m_table.find(id)) {
        return false;
    }
    m_table.emplace(id, *val);

    for (auto it = m_table.begin(); it != m_table.end(); ++it) {
        cout << it->first << " : ";
        switch (it->second.getKind()) {
        case APValue::ValueKind::Int:
            cout << it->second.getInt().getLimitedValue();
            break;
        case APValue::ValueKind::Float:
            cout << it->second.getFloat().convertToDouble();
            break;
        }
        cout << endl;
    }

    return true;
}
