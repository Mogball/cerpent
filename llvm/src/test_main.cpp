#include "llvm-jit.hpp"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;
using namespace llvm;

LLVM_ATTRIBUTE_NORETURN static void fatalError(Error err) {
    handleAllErrors(move(err), [&](const ErrorInfoBase &eib) {
        errs() << "Fatal Error: ";
        eib.log(errs());
        errs() << "\n";
        errs().flush();
    });
    exit(1);
}

template<typename T, size_t size>
constexpr unsigned arrayElements(T (&)[size]) { return size; }

int *customIntAllocator(unsigned items) {
    static int memory[128];
    static unsigned allocIdx = 0;
    if (allocIdx + items > arrayElements(memory)) {
        exit(-1);
    }
    int *block = memory + allocIdx;
    allocIdx += items;
    return block;
}

int main(int argc, char *argv[]) {
    sys::PrintStackTraceOnErrorSignal(argv[0]);
    PrettyStackTraceProgram stackProgram(argc, argv);
    atexit(llvm_shutdown);

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    cl::ParseCommandLineOptions(argc, argv, "Cerpent\n");

    int x[]{0, 1, 2};
    int y[]{3, 1, -1};

    auto targetMachine = EngineBuilder().selectTarget();
    auto jit = std::make_unique<CerpentJit>(*targetMachine);
    string sourceCode =
        "extern \"C\" int abs(int);"
        "extern int *customIntAllocator(unsigned items);"
        "extern \"C\" int *integerDistances(int *x, int *y, unsigned items) {"
            "int *results = customIntAllocator(items);"
            "for (int i = 0; i < items; ++i) {"
                "results[i] = abs(x[i] - y[i]);"
            "}"
            "return results;"
        "}";

    LLVMContext context;
    auto module = jit->compileModule(sourceCode, context);
    if (!module) {
        fatalError(module.takeError());
    }
    jit->submitModule(move(*module));
    auto jittedFcn = jit->getFunction<int *(int *, int *, unsigned)>("integerDistances");
    if (!jittedFcn) {
        fatalError(jittedFcn.takeError());
    }
    auto integerDistances = *jittedFcn;
    int *z = integerDistances(x, y, arrayElements(x));
    outs() << "Integer Distances: ";
    outs() << z[0] << ", " << z[1] << ", " << z[2] << "\n\n";
    outs().flush();
}
