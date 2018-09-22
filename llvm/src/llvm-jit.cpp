#include "llvm-jit.hpp"

using namespace std;
using namespace llvm;

CerpentJit::NotifyObjectLoaded::NotifyObjectLoaded(CerpentJit &jit) :
    m_jit(jit) {}

void CerpentJit::NotifyObjectLoaded::NotifyObjectLoaded::operator()(
        ObjHandle, ObjPtr &obj, const LoadedObjectInfo &info) {
    const auto &fixedInfo =
            static_cast<const RuntimeDyld::LoadedObjectInfo &>(info);
    m_jit.m_eventListener->NotifyObjectEmitted(*obj->getBinary(), fixedInfo);
}

CerpentJit::CerpentJit(TargetMachine &targetMachine) :
        m_dataLayout(targetMachine.createDataLayout()),
        m_memoryManager(make_shared<SectionMemoryManager>()),
