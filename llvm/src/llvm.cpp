#include "interp.hpp"
#include <clang/Tooling/Tooling.h>
#include <memory>

using namespace std;
using namespace clang;

error_t Interpreter::processUnit(const string &unit) {
    unique_ptr<ASTUnit> ast(tooling::buildASTFromCode(unit));
    TranslationUnitDecl *dc = ast->getASTContext().getTranslationUnitDecl();
    if (nullptr != dc) {
        dc->dump();
    }
    return OK;
}
