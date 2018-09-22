#include "llvm-vardecl.hpp"

using namespace std;
using namespace clang;

VarDeclVisitor::VarDeclVisitor(ASTContext *context) :
        m_context(context),
        m_validVarDecl(false),
        m_varDeclIdx() {}

bool VarDeclVisitor::TraverseVarDecl(VarDecl *decl) {
    RecursiveASTVisitor<VarDeclVisitor>::TraverseVarDecl(decl);
    m_validVarDecl = !decl->isInvalidDecl();
    m_varDeclIdx = decl->getNameAsString();
    return false;
}

VarDeclConsumer::VarDeclConsumer(ASTContext *context) :
    m_visitor(context) {}

void VarDeclConsumer::HandleTranslationUnit(ASTContext &context) {
    m_visitor.TraverseDecl(context.getTranslationUnitDecl());
}

bool VarDeclConsumer::isValidVarDecl() {
    return m_visitor.m_validVarDecl;
}

string VarDeclConsumer::getVarDeclIdx() {
    return m_visitor.m_varDeclIdx;
}
