#include "interp.hpp"

#include <iostream>

#include <cassert>

using namespace std;

static error_t interp_loop() {
    Interpreter interp;
    error_t err = OK;
    for (string line; OK == err && getline(cin, line);) {
        interp.processLine(move(line));
        assert(line.empty());
    }
    return err;
}

int main(void) {
    return static_cast<int>(interp_loop());
}
