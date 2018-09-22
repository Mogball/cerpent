#include "interp.hpp"
#include "interp-jit.hpp"
#include "llvm-funcdecl.hpp"
#include "llvm-vardecl.hpp"
#include <llvm/Support/Error.h>
#include <clang/Tooling/Tooling.h>
#include <unistd.h>
#include <memory>

using namespace std;
using namespace llvm;
using namespace clang;

static void logError(Error err) {
    handleAllErrors(move(err), [&](const ErrorInfoBase &eib) {
        outs() << "Fatal Error: ";
        eib.log(outs());
        outs() << "\n";
    });
}

error_t Interpreter::processUnit(const string &buf) {
    string unit = buf;
    vector<string> args = {"-fsyntax-only"};

    raw_fd_ostream *pStderr = reinterpret_cast<raw_fd_ostream *>(&errs());
    int save_err = dup(STDERR_FILENO);
    pStderr->close();
    unique_ptr<ASTUnit> ast(tooling::buildASTFromCodeWithArgs(unit, args));
    dup2(save_err, STDERR_FILENO);
    ::new(pStderr) raw_fd_ostream(STDERR_FILENO, false, true);

    TranslationUnitDecl *dc = ast->getASTContext().getTranslationUnitDecl();
    if (nullptr != dc) {
        //dc->dump();
    }

    VarDeclConsumer varDecl(&ast->getASTContext());
    varDecl.HandleTranslationUnit(ast->getASTContext());
    if (varDecl.isValidVarDecl()) {
        auto idx = m_jit->compileDecl(unit, false);
        if (!idx) {
            logError(idx.takeError());
        } else {
            cout << "Variable declared: " << varDecl.getVarDeclIdx() << endl;
        }
        return OK;
    }
    FuncDeclConsumer funcDecl(&ast->getASTContext());
    funcDecl.HandleTranslationUnit(ast->getASTContext());
    if (funcDecl.isValidFuncDecl()) {
        if (!funcDecl.isExternFuncDecl()) {
            unit = "extern \"C\" " + unit;
        }
        auto idx = m_jit->compileDecl(unit, funcDecl.isExternFuncDecl());
        if (!idx) {
            logError(idx.takeError());
        } else {
            cout << "Function declared: " << funcDecl.getFuncDeclIdx() << endl;
        }
        return OK;
    }

    auto idx = m_jit->compileScript(unit);
    if (!idx) {
        logError(idx.takeError());
    } else {
        cout << "Running script: " << *idx << endl;
        auto ret = m_jit->executeScript(*idx);
        if (!ret) {
            logError(idx.takeError());
        }
    }
    return OK;
}
