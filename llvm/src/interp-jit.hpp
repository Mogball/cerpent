#pragma once

#include "llvm-jit.hpp"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

class InterpreterJit {
public:
    InterpreterJit(int argc, char *argv[]);
    ~InterpreterJit();

    llvm::Expected<std::string> compileDecl(std::string code, std::string idx);
    llvm::Expected<std::string> compileScript(std::string code);
    llvm::Expected<bool> executeScript(std::string scriptName);

private:
    llvm::PrettyStackTraceProgram m_stackProgram;
    std::unique_ptr<llvm::TargetMachine> m_targetMachine;
    std::unique_ptr<CerpentJit> m_jit;

    llvm::LLVMContext m_context;

    std::size_t m_scriptIndex;
    std::string m_scriptPrefix;

    std::string wrapScript(std::string code);
};
