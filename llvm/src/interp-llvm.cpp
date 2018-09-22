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

#define DISABLE_STDERR() \
    raw_fd_ostream *__pStderr = reinterpret_cast<raw_fd_ostream *>(&errs()); \
    int __save_err = dup(STDERR_FILENO); \
    __pStderr->close()
#define ENABLE_STDERR() \
    dup2(__save_err, STDERR_FILENO); \
    ::new(__pStderr) raw_fd_ostream(STDERR_FILENO, false, true)

static void logError(Error err) {
    handleAllErrors(move(err), [&](const ErrorInfoBase &eib) {
        outs() << "Error: ";
        eib.log(outs());
        outs() << "\n";
    });
}

error_t Interpreter::processUnit(const string &buf) {
    string unit = buf;

    DISABLE_STDERR();
    unique_ptr<ASTUnit> ast(tooling::buildASTFromCodeWithArgs(unit, {"-fsyntax-only"}));
    ENABLE_STDERR();

    //TranslationUnitDecl *dc = ast->getASTContext().getTranslationUnitDecl();
    //if (nullptr != dc) {
    //    dc->dump();
    //}

    VarDeclConsumer varDecl(&ast->getASTContext());
    varDecl.HandleTranslationUnit(ast->getASTContext());
    if (varDecl.isValidVarDecl()) {
        auto idx = m_jit->compileDecl(unit);
        if (!idx) {
            logError(idx.takeError());
        } else {
            //cout << "Variable declared: " << varDecl.getVarDeclIdx() << endl;
        }
        m_jit->appendDecl(unit, varDecl.isExternVarDecl());
        return OK;
    }
    FuncDeclConsumer funcDecl(&ast->getASTContext());
    funcDecl.HandleTranslationUnit(ast->getASTContext());
    if (funcDecl.isValidFuncDecl()) {
        if (!funcDecl.isExternFuncDecl()) {
            unit = "extern \"C\" " + unit;
        }
        auto idx = m_jit->compileDecl(unit);
        if (!idx) {
            logError(idx.takeError());
        } else {
            //cout << "Function declared: " << funcDecl.getFuncDeclIdx() << endl;
            if (funcDecl.isExternFuncDecl()) {
                m_jit->appendDecl(unit, true);
            } else {
                m_jit->appendDecl(unit.substr(0, unit.find_first_of('{')), true);
            }
            return OK;
        }
    }

    return processScript(unit);
}

error_t Interpreter::processScript(const string &unit) {
    auto idx = m_jit->compileScript(unit);
    if (!idx) {
        logError(idx.takeError());
    } else {
        //cout << "Running script: " << *idx << endl;
        auto ret = m_jit->executeScript(*idx);
        if (!ret) {
            logError(idx.takeError());
        }
    }
    return OK;
}
