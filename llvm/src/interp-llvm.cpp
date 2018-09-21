#include "interp.hpp"
#include "llvm-action.hpp"
#include <clang/Tooling/Tooling.h>
#include <memory>

using namespace std;
using namespace clang;

error_t Interpreter::processUnit(const string &unit) {
    vector<string> args = {"-fsyntax-only"};
    unique_ptr<ASTUnit> ast(tooling::buildASTFromCodeWithArgs(unit, args));
    TranslationUnitDecl *dc = ast->getASTContext().getTranslationUnitDecl();
    if (nullptr != dc) {
        dc->dump();
    }
    InterpreterConsumer consumer(&ast->getASTContext());
    consumer.HandleTranslationUnit(ast->getASTContext());
    return OK;
}
