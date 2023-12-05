#ifndef GRLANG_LLVMGEN_AST_IO_H
#define GRLANG_LLVMGEN_AST_IO_H

#include "common/config.h"
#include "common/macros.h"
#include "llvmgen/context.h"
#include "llvmgen/ast/base.h"

namespace grlang
{

namespace llvmgen
{

constexpr std::string_view IO_PRINT_FUNC_NAME = "__print_";
constexpr std::string_view IO_SCAN_FUNC_NAME  = "__scan_";

class PrintNode : public Node
{
public:
    PrintNode(NodePtr expr) :
        expr_(expr)
    {}

    llvm::Value *CodeGen(Context &ctx) override;

private:
    NodePtr expr_;
};

class ScanNode : public Node
{
public:
    llvm::Value *CodeGen(Context &ctx) override;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_IO_H
