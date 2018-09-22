#include "llvm-jit.hpp"

using namespace std;
using namespace std::placeholders;
using namespace llvm;

CerpentJit::CerpentJit(TargetMachine &targetMachine) :
        m_dataLayout(targetMachine.createDataLayout()),
        m_memoryManager(make_shared<SectionMemoryManager>()),
        m_symbolResolver(orc::createLambdaResolver(
            [&](std::string name) { return findSymbolJit(name); },
            [&](std::string name) { return findSymbolHost(name); })),
        m_objectLayer([this]() { return m_memoryManager; },
            bind(&CerpentJit::notifyObjectLoaded, this, _1, _2, _3)),
        m_compileLayer(m_objectLayer, IRCompiler(targetMachine)) {
    sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
    m_eventListener = JITEventListener::createGDBRegistrationListener();
}

void CerpentJit::submitModule(ModulePtr module) {
    cantFail(m_compileLayer.addModule(move(module), m_symbolResolver));
}

Expected<unique_ptr<Module>> CerpentJit::compileModule(string code, LLVMContext &context) {
    return m_driver.compileTranslationUnit(code, context);
}

Expected<JITTargetAddress> CerpentJit::getFunctionPtr(string name) {
    string mangledName = mangle(move(name));
    JITSymbol sym = findSymbolJit(mangledName);
    if (!sym) {
        return make_error<orc::JITSymbolNotFound>(mangledName);
    }
    return sym.getAddress();
}

JITSymbol CerpentJit::findSymbolJit(string mangledName) {
    return m_compileLayer.findSymbol(mangledName, false);
}

JITSymbol CerpentJit::findSymbolHost(string mangledName) {
    if (JITTargetAddress addr = RTDyldMemoryManager::getSymbolAddressInProcess(mangledName)) {
        return JITSymbol(addr, JITSymbolFlags::Exported);
    }
    return nullptr;
}

string CerpentJit::mangle(string name) {
    string buffer;
    raw_string_ostream ostream(buffer);
    Mangler::getNameWithPrefix(ostream, move(name), m_dataLayout);
    return ostream.str();
}

void CerpentJit::notifyObjectLoaded(
        ObjHandle,
        const ObjPtr &obj,
        const RuntimeDyld::LoadedObjectInfo &info) {
    m_eventListener->NotifyObjectEmitted(*obj->getBinary(), info);
}
