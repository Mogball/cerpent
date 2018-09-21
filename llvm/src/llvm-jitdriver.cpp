#include "llvm-jitdriver.hpp"
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <vector>
#include <string>

using namespace std;
using namespace llvm;

CerpentDriver::CerpentDriver() = default;

CerpentDriver::~CerpentDriver() {
    for (auto deleter : m_sourceFileDeleters) {
        deleter();
    }
}

Expected<unique_ptr<Module>>
CerpentDriver::compileTranslationUnit(string code, LLVMContext &context) {

}
