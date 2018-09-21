#pragma once

#include "llvm-consumer.hpp"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>

class InterpreterAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance &compiler,
            llvm::StringRef inFile);
};
