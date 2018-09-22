#include "interp.hpp"
#include <iostream>
#include <cassert>

using namespace std;

static error_t interp_loop() {
    Interpreter interp;
    error_t err = OK;
    for (string line; getline(cin, line);) {
        err = interp.processLine(move(line));
        assert(line.empty());
        if (OK != err) {
            printf("ERROR: 0x%04x\n", err);
        }
    }
    return err;
}

/*int main(void) {
    //return static_cast<int>(interp_loop());
}*/
