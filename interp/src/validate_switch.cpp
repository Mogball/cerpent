#include "error.h"
#include "log.hpp"
#include "validate.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

error_t isVariableDeclare(
        const string_vec &toks,
        unsigned *type,
        string_it *name,
        string_it *val) {
    assert(nullptr != name);
    assert(nullptr != type);
    if (toks.size() < 4 || OK != isTypeStart(toks.front())) {
        return NOT_VARIABLE_DECL;
    }
    auto numEqualSign = count(toks.begin(), toks.end(), "=");
    if (1 != numEqualSign) {
        interp_error("Too many = in declaration\n");
        return INVALID_VARIABLE_DECL;
    }
    auto it = find(toks.begin(), toks.end(), "=");
    assert(toks.end() != it);
    --it;
    if (OK == isTypeStart(*it)) {
        interp_error("Missing identifier in declaration\n");
        return INVALID_VARIABLE_DECL;
    }
    error_t err = isIdentifier(*it);
    if (OK != err) {
        interp_error("Invalid identifier in declaration\n");
        return err;
    }
    unsigned var_type;
    err = parseType(toks.begin(), it, &var_type);
    if (OK != err) {
        return err;
    }
    *type = var_type;
    *name = it;
    it += 2;
    if (toks.end() == it) {
        interp_error("No value supplied in declaration\n");
        return INVALID_VARIABLE_DECL;
    }
    *val = it;
    ++it;
    if (toks.end() != it) {
        interp_error("Too many values provided in declaration\n");
        return INVALID_VARIABLE_DECL;
    }
    return OK;
}
