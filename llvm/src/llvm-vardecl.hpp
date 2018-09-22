#pragma once

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTConsumer.h>

class VarDeclConsumer;

class VarDeclVisitor :
    public clang::RecursiveASTVisitor<VarDeclVisitor> {
    friend class VarDeclConsumer;
public:
    explicit VarDeclVisitor(clang::ASTContext *context);
    bool TraverseVarDecl(clang::VarDecl *decl);

private:
    clang::ASTContext *m_context;
    VarDeclConsumer *m_consumer;
    bool m_validVarDecl;
    bool m_externVarDecl;
    std::string m_varDeclIdx;
};

class VarDeclConsumer : public clang::ASTConsumer {
public:
    explicit VarDeclConsumer(clang::ASTContext *context);
    virtual void HandleTranslationUnit(clang::ASTContext &context);

    bool isValidVarDecl();
    bool isExternVarDecl();
    std::string getVarDeclIdx();

private:
    VarDeclVisitor m_visitor;
};
