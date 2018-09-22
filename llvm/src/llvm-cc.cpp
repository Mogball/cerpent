#include <llvm/Option/Arg.h>
#include <clang/CodeGen/ObjectFilePCHContainerOperations.h>
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
    IntrusiveRefCntPtr<DiagnosticIDs> diagId(new DiagnosticIDs());

    auto pchOps = clang->getPCHContainerOperations();
    pchOps->registerWriter(llvm::make_unique<ObjectFilePCHContainerWriter>());
    pchOps->registerReader(llvm::make_unique<ObjectFilePCHContainerReader>());

    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

    IntrusiveRefCntPtr<DiagnosticOptions> diagOpts = new DiagnosticOptions();
    TextDiagnosticBuffer *diagsBuffer = new TextDiagnosticBuffer();
    DiagnosticsEngine diags(diagId, &*diagOpts, diagsBuffer);
    bool success = CompilerInvocation::CreateFromArgs(
            clang->getInvocation(), argv.begin(), argv.end(), diags);

    if (clang->getHeaderSearchOpts().UseBuiltinIncludes &&
        clang->getHeaderSearchOpts().ResourceDir.empty()) {
        clang->getHeaderSearchOpts().ResourceDir =
            CompilerInvocation::GetResourcesPath(argv0, mainAddr);
    }

    clang->createDiagnostics();
    if (!clang->hasDiagnostics()) {
        return 1;
    }

    llvm::install_fatal_error_handler(
            LLVMErrorHandler,
            static_cast<void *>(&clang->getDiagnostics()));

    diagsBuffer->FlushDiagnostics(clang->getDiagnostics());
    if (!success) {
        return 1;
    }

    success = ExecuteCompilerInvocation(clang.get());
    llvm::TimerGroup::printAll(llvm::outs());
    llvm::remove_fatal_error_handler();
    if (clang->getFrontendOpts().DisableFree) {
        BuryPointer(move(clang));
    }
    return !success;
}
