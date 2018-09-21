#include "llvm-visitor.hpp"

using namespace clang;

InterpreterVisitor::InterpreterVisitor(ASTContext *context) :
    m_context(context) {}

bool InterpreterVisitor::TraverseDecl(Decl *decl) {
    RecursiveASTVisitor<InterpreterVisitor>::TraverseDecl(decl);
    return true;
}

bool InterpreterVisitor::TraverseStmt(Stmt *stmt) {
    RecursiveASTVisitor<InterpreterVisitor>::TraverseStmt(stmt);
    return true;
}

bool InterpreterVisitor::TraverseType(QualType type) {
    RecursiveASTVisitor<InterpreterVisitor>::TraverseType(type);
    return true;
}

bool InterpreterVisitor::TraverseVarDecl(VarDecl *decl) {
    RecursiveASTVisitor<InterpreterVisitor>::TraverseVarDecl(decl);
    decl->dump();
    return true;
}
