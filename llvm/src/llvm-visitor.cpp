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

#include "symboltable.hpp"

static SymbolTable table;

bool InterpreterVisitor::TraverseVarDecl(VarDecl *decl) {
    RecursiveASTVisitor<InterpreterVisitor>::TraverseVarDecl(decl);
    APValue *val = decl->evaluateValue();
    if (nullptr != val) {
        table.addSymbol(decl->getIdentifier()->getName(), val);
    }
    return true;
}
