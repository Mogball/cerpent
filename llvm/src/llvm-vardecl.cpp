#include "llvm-vardecl.hpp"

using namespace std;
using namespace clang;

VarDeclVisitor::VarDeclVisitor(ASTContext *context) :
        m_context(context) {}

bool VarDeclVisitor::TraverseVarDecl(VarDecl *decl) {
    RecursiveASTVisitor<VarDeclVisitor>::TraverseVarDecl(decl);
    m_validVarDecl = !decl->isInvalidDecl() && nullptr != decl->evaluateValue();
    m_varDeclIdx = decl->getNameAsString();
    return m_validVarDecl;
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
