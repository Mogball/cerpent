#include "interp.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

template<typename stream_t>
static error_t interp_loop(int argc, char *argv[], stream_t &s, bool cmd) {
    Interpreter interp(argc, argv);
    error_t err = OK;
    if (cmd) {
        cout << "Cerpent v1.0" << endl;
        cout << "> ";
    }
    for (string line; getline(s, line);) {
        err = interp.processLine(move(line));
        assert(line.empty());
        if (OK != err) {
            printf("ERROR: 0x%04x\n", err);
        }
        if (cmd) {
            if (interp.isBufferEmpty()) {
                cout << "> ";
            } else {
                cout << "  ";
            }
            for (int i = 0; i < interp.getIndent(); ++i) {
                cout << "  ";
            }
        }
    }
    return err;
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        ifstream inFile;
        inFile.open(argv[1], ios::in);
        error_t ret = static_cast<int>(interp_loop(argc - 1, argv, inFile, false));
        inFile.close();
        return ret;
    } else if (argc > 2) {
        cout << "cerp [[script.cerp]]" << endl;
        return -1;
    }
    return static_cast<int>(interp_loop(argc, argv, cin, true));
}
