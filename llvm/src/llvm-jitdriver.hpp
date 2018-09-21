#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Error.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

class CerpentDriver {
public:
    CerpentDriver();
    ~CerpentDriver();

    llvm::Expected<std::unique_ptr<llvm::Module>>
    compileTranslationUnit(std::string code, llvm::LLVMContext &context);

private:
    std::vector<std::function<void()>> m_sourceFileDeleters;
};
