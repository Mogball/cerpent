#pragma once

#include <clang/AST/RecursiveASTVisitor.h>

class InterpreterVisitor :
    public clang::RecursiveASTVisitor<InterpreterVisitor> {
public:
    explicit InterpreterVisitor(clang::ASTContext *context);

    bool TraverseDecl(clang::Decl *decl);
    bool TraverseStmt(clang::Stmt *stmt);
    bool TraverseType(clang::QualType type);

    bool TraverseVarDecl(clang::VarDecl *decl);
private:
    clang::ASTContext *m_context;
};
