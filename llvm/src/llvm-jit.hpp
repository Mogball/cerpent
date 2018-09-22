#pragma once

#include "llvm-jitdriver.hpp"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/Orc/OrcError.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/RuntimeDyld.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/DynamicLibrary.h>

#include <functional>
#include <memory>
#include <vector>

class CerpentJit {
    using ModulePtr = std::unique_ptr<llvm::Module>;
    using IRCompiler = llvm::orc::SimpleCompiler;
    using ObjectLayer = llvm::orc::RTDyldObjectLinkingLayer;
    using CompileLayer = llvm::orc::IRCompileLayer<ObjectLayer, IRCompiler>;
    using ObjHandle = llvm::orc::RTDyldObjectLinkingLayer::ObjHandleT;
    using ObjPtr = llvm::orc::RTDyldObjectLinkingLayer::ObjectPtr;

public:
    CerpentJit(llvm::TargetMachine &targetMachine);

    void submitModule(ModulePtr module);
    llvm::Expected<std::unique_ptr<llvm::Module>>
    compileModule(std::string code, llvm::LLVMContext &context);
    llvm::Expected<llvm::JITTargetAddress> getFunctionPtr(std::string name);

    template<class Signature>
    llvm::Expected<std::function<Signature>> getFunction(std::string name) {
        auto addr = getFunctionPtr(std::move(name));
        if (!addr) {
            return addr.takeError();
        }
        auto typedFuncPtr = reinterpret_cast<Signature *>(*addr);
        return std::function<Signature>(typedFuncPtr);
    }

private:
    llvm::DataLayout m_dataLayout;
    CerpentDriver m_driver;
    std::shared_ptr<llvm::RTDyldMemoryManager> m_memoryManager;
    std::shared_ptr<llvm::JITSymbolResolver> m_symbolResolver;
    llvm::JITEventListener *m_eventListener;

    ObjectLayer m_objectLayer;
    CompileLayer m_compileLayer;

    llvm::JITSymbol findSymbolJit(std::string mangledName);
    llvm::JITSymbol findSymbolHost(std::string mangledName);

    std::string mangle(std::string name);
    void notifyObjectLoaded(
            ObjHandle,
            const ObjPtr &obj,
            const llvm::RuntimeDyld::LoadedObjectInfo &info);
};
