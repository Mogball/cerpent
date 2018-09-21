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

extern vector<string> getClangArgs(StringRef cpp, StringRef bc);
extern int ccMain(ArrayRef<const char *> argv, const char *argv0, void *mainAddr);

static Error returnCodeError(StringRef message, int returnCode) {
    return make_error<StringError>(message, error_code(returnCode, system_category()));
}

static Expected<string> saveSourceFile(string content) {
    using llvm::sys::fs::createTemporaryFile;
    int fd;
    SmallString<128> name;
    if (auto ec = createTemporaryFile("cerpent_temp", "cpp", fd, name)) {
        return errorCodeToError(ec);
    }
    raw_fd_ostream os(fd, true, true);
    os << content;
    return name.str();
}

static string replaceExtension(StringRef name, StringRef ext) {
    return name.substr(0, name.find_last_of('.') + 1).str() + ext.str();
}

static Error compileCppToBitcode(vector<string> &&args) {
    vector<const char *> ccArgs;
    transform(args.begin(), args.end(), back_inserter(ccArgs),
            [](const string &s) { return s.c_str(); });
    if (int res = ccMain(ccArgs, "cc", nullptr)) {
        return returnCodeError("Clang cc compilation failed", res);
    }
    return Error::success();
}

static Expected<unique_ptr<Module>>
readModuleFromBitCode(StringRef bc, LLVMContext &context) {
    ErrorOr<unique_ptr<MemoryBuffer>> buffer = MemoryBuffer::getFile(bc);
    if (!buffer) {
        return errorCodeToError(buffer.getError());
    }
    return parseBitcodeFile(buffer.get()->getMemBufferRef(), context);
}

CerpentDriver::CerpentDriver() = default;

CerpentDriver::~CerpentDriver() {
    for (auto deleter : m_sourceFileDeleters) {
        deleter();
    }
}

Expected<unique_ptr<Module>>
CerpentDriver::compileTranslationUnit(string code, LLVMContext &context) {
    auto sourceFileName = saveSourceFile(code);
    if (!sourceFileName) {
        return sourceFileName.takeError();
    }
    string cpp = *sourceFileName;
    string bc = replaceExtension(cpp, "bc");
    Error err = compileCppToBitcode(getClangArgs(cpp, bc));
    if (err) {
        return move(err);
    }
    auto module = readModuleFromBitCode(bc, context);
    llvm::sys::fs::remove(bc);
    if (!module) {
        llvm::sys::fs::remove(cpp);
        return module.takeError();
    }
    m_sourceFileDeleters.push_back([cpp]() { llvm::sys::fs::remove(cpp); });
    return move(*module);
}
