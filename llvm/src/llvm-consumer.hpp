#pragma once

#include "llvm-visitor.hpp"
#include <clang/AST/ASTConsumer.h>

class InterpreterConsumer : public clang::ASTConsumer {
public:
    explicit InterpreterConsumer(clang::ASTContext *context);
    virtual void HandleTranslationUnit(clang::ASTContext &context);

private:
    InterpreterVisitor m_visitor;
};
