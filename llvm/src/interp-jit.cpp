#include "interp-jit.hpp"
#include <sstream>

using namespace std;
using namespace llvm;

static string generatePrefix() {
    static size_t s_count = 0;
    stringstream ss;
    ss << "_cerpscript_ " << s_count << "_";
    ++s_count;
    return ss.str();
}

InterpreterJit::InterpreterJit(int argc, char *argv[]) :
        m_stackProgram(argc, argv),
        m_scriptIndex(0),
        m_scriptPrefix(generatePrefix()) {
    sys::PrintStackTraceOnErrorSignal(argv[0]);
    atexit(llvm_shutdown);
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    cl::ParseCommandLineOptions(argc, argv, "Cerpent");
    m_targetMachine.reset(EngineBuilder().selectTarget());
    m_jit = std::make_unique<CerpentJit>(*m_targetMachine);
}

InterpreterJit::~InterpreterJit() = default;

Expected<string> InterpreterJit::compileDecl(string code, string idx) {
    auto module = m_jit->compileModule(code, m_context);
    if (!module) {
        return module.takeError();
    }
    m_jit->submitModule(move(*module));
    return idx;
}

Expected<string> InterpreterJit::compileScript(string code) {
    stringstream scriptName;
    scriptName << m_scriptPrefix << m_scriptIndex++;
    stringstream scriptCall;
    scriptCall
        << "extern \"C\" bool " << scriptName.str() << "(void) {\n"
        << "    " << code << "\n"
        << "    return true;\n"
        << "}\n";
    auto module = m_jit->compileModule(scriptCall.str(), m_context);
    if (!module) {
        return module.takeError();
    }
    m_jit->submitModule(move(*module));
    return scriptName.str();
}

Expected<bool> InterpreterJit::executeScript(string scriptName) {
    auto fcn = m_jit->getFunction<bool(void)>(scriptName);
    if (!fcn) {
        return fcn.takeError();
    }
    auto script = *fcn;
    return script();
}
