#include <llvm/ADT/StringRef.h>
#include <string>
#include <vector>

#define STRINGIFY_DET(X) #X
#define STRINGIFY(X) STRINGIFY_DET(X)

using namespace std;

vector<string> getClangArgs(StringRef cpp, StringRef bc) {
    vector<string> args = {
        "-emit-llvm",
        "-emit-llvm-bc",
        "-emit-llvm-uselists",
        "-main-file-name",
        cpp.data(),
        "-std=gnu++17",
        "-disable-free",
        "-fdeprecated-macro",
        "-fmath-errno",
        "-fuse-init-array",
        "-mrelocation-module",
        "static",
        "-mthread-model",
        "posix",
        "-masm-verbose",
        "-mconstructor-aliases",
        "-munwind-tables",
        "-dwarf-column-info",
        "-debugger-tuning=gdb",
        "-O3",
        "-mdisable-fp-elim",
        "-momit-leaf-frame-pointer",
        "-vectorize-loops",
        "-vectorize-slp",

        "-resource-dir",
        "/usr/lib/clang/6.0.0",
        "-internal-system",
        "/usr/lib/clang/6.0.0/include",

        "-o",
        bc.data(),
        "-x",
        "c++",
        cpp.data()
    };

    return args;
}
