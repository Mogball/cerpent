#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>

class FuncDeclConsumer;

class FuncDeclVisitor :
    public clang::RecursiveASTVisitor<FuncDeclVisitor> {
    friend class FuncDeclConsumer;
public:
    explicit FuncDeclVisitor(clang::ASTContext *context);
    bool TraverseFunctionDecl(clang::FunctionDecl *decl);

private:
    clang::ASTContext *m_context;
    FuncDeclConsumer *m_consumer;
    bool m_validFuncDecl;
    bool m_externFuncDecl;
    std::string m_varDeclIdx;
};

class FuncDeclConsumer : public clang::ASTConsumer {
public:
    explicit FuncDeclConsumer(clang::ASTContext *context);
    virtual void HandleTranslationUnit(clang::ASTContext &context);

    bool isValidFuncDecl();
    bool isExternFuncDecl();
    std::string getFuncDeclIdx();

private:
    FuncDeclVisitor m_visitor;
};
