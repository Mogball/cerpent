#include "llvm-consumer.hpp"

using namespace clang;

InterpreterConsumer::InterpreterConsumer(ASTContext *context) :
    m_visitor(context) {}

void InterpreterConsumer::HandleTranslationUnit(ASTContext &context) {
    m_visitor.TraverseDecl(context.getTranslationUnitDecl());
}
