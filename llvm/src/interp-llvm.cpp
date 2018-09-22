#include "interp.hpp"
#include "interp-jit.hpp"
#include "llvm-vardecl.hpp"
#include <llvm/Support/Error.h>
#include <clang/Tooling/Tooling.h>
#include <memory>

using namespace std;
using namespace llvm;
using namespace clang;

static void logError(Error err) {
    handleAllErrors(move(err), [&](const ErrorInfoBase &eib) {
        errs() << "Fatal Error: ";
        eib.log(errs());
        errs() << "\n";
        errs().flush();
    });
}

error_t Interpreter::processUnit(const string &unit) {
    vector<string> args = {"-fsyntax-only"};
    unique_ptr<ASTUnit> ast(tooling::buildASTFromCodeWithArgs(unit, args));
    TranslationUnitDecl *dc = ast->getASTContext().getTranslationUnitDecl();
    if (nullptr != dc) {
        dc->dump();
    }
    VarDeclConsumer consumer(&ast->getASTContext());
    consumer.HandleTranslationUnit(ast->getASTContext());
    if (consumer.isValidVarDecl()) {
        auto idx = m_jit->compileDecl(unit, consumer.getVarDeclIdx());
        if (!idx) {
            logError(idx.takeError());
        } else {
            outs() << "Variable declared: " << *idx << "\n";
        }
    }
    return OK;
}
