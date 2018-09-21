#include <llvm/Option/Arg.h>
#include <clang/CodeGen/ObjectFilePCHContainerOperations.h>
#include <clang/Config/config.h>
#include <clang/Basic/Stack.h>
#include <clang/Driver/DriverDiagnostic.h>
#include <clang/Driver/Options.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/FrontendDiagnostic.h>
#include <clang/Frontend/TextDiagnosticBuffer.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/Utils.h>
#include <clang/FrontendTool/Utils.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/Config/llvm-config.h>
#include <llvm/LinkAllPasses.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Option/OptTable.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Timer.h>
#include <llvm/Support/raw_ostream.h>
#include <cstdio>

using namespace std;
using namespace clang;
using namespace llvm::opt;

static void LLVMErrorHandler(void *userData, const string &message, bool genDiag) {
    DiagnosticsEngine &diags = *static_cast<DiagnosticsEngine *>(userData);
    diags.Report(diag::err_fe_error_backend) << message;
    llvm::sys::RunInterruptHandlers();
    exit(genDiag ? 70 : 1);
}

int ccMain(ArrayRef<const char *> argv, const char *argv0, void *mainAddr) {
    // TODO CompilerInstance::setVirtualFileSystem and inMemoryFileSystem
    unique_ptr<CompilerInstance> clang(new CompilerInstance());
    IntrusiveRefCntPtr<DiagnosticsIDs> diagId(new DiagnosticsIDs());

    auto pchOps = clang->getPCHContainerOperations();
}
