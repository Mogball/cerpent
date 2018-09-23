#include "interp.hpp"
#include <iostream>
#include <cassert>

using namespace std;

template<typename stream_t>
static error_t interp_loop(int argc, char *argv[], stream_t &s) {
    Interpreter interp(argc, argv);
    error_t err = OK;
    cout << "Cerpent v1.0" << endl;
    cout << "> ";
    for (string line; getline(s, line);) {
        err = interp.processLine(move(line));
        assert(line.empty());
        if (OK != err) {
            printf("ERROR: 0x%04x\n", err);
        }
        if (interp.isBufferEmpty()) {
            cout << "> ";
        } else {
            cout << "  ";
        }
        for (int i = 0; i < interp.getIndent(); ++i) {
            cout << "  ";
        }
    }
    return err;
}

int main(int argc, char *argv[]) {
    return static_cast<int>(interp_loop(argc, argv, cin));
}
