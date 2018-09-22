#include "llvm-funcdecl.hpp"

using namespace std;
using namespace clang;

FuncDeclVisitor::FuncDeclVisitor(ASTContext *context) :
        m_context(context),
        m_validFuncDecl(false),
        m_externFuncDecl(false),
        m_varDeclIdx() {}

bool FuncDeclVisitor::TraverseFunctionDecl(FunctionDecl *decl) {
    RecursiveASTVisitor<FuncDeclVisitor>::TraverseFunctionDecl(decl);
    m_validFuncDecl = !decl->isInvalidDecl();
    m_externFuncDecl = decl->isExternC();
    m_varDeclIdx = decl->getNameAsString();
    return false;
}

FuncDeclConsumer::FuncDeclConsumer(ASTContext *context) :
    m_visitor(context) {}

void FuncDeclConsumer::HandleTranslationUnit(ASTContext &context) {
    m_visitor.TraverseDecl(context.getTranslationUnitDecl());
}

bool FuncDeclConsumer::isValidFuncDecl() {
    return m_visitor.m_validFuncDecl;
}

bool FuncDeclConsumer::isExternFuncDecl() {
    return m_visitor.m_externFuncDecl;
}

string FuncDeclConsumer::getFuncDeclIdx() {
    return m_visitor.m_varDeclIdx;
}
