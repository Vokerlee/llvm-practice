#ifndef GRLANG_LLVMGEN_AST_BINOP_H
#define GRLANG_LLVMGEN_AST_BINOP_H

#include "common/config.h"
#include "common/macros.h"
#include "llvmgen/context.h"
#include "llvmgen/ast/base.h"

namespace grlang
{

namespace llvmgen
{

using BinOpPtr = std::shared_ptr<BinOpNode>;

enum class BinOp
{
    INVALID,

    // Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    XOR,

    // Comparison
    EQ,
    NEQ,
    GT,
    LT,
    GTE,
    LTE,
};

class BinOpNode : public Node
{
public:
    BinOpNode(NodePtr left, BinOp op, NodePtr right) :
        left_(left), right_(right), op_(op)
    {}

    llvm::Value *CodeGen(Context &ctx) override;

private:
    NodePtr left_  {};
    NodePtr right_ {};
    BinOp   op_    {BinOp::INVALID};
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_BINOP_H
