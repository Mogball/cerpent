#include "llvm-action.hpp"

using namespace std;
using namespace llvm;
using namespace clang;

unique_ptr<ASTConsumer> InterpreterAction::CreateASTConsumer(
        CompilerInstance &compiler,
        StringRef inFile) {
    return unique_ptr<ASTConsumer>(
            new InterpreterConsumer(&compiler.getASTContext()));
}

