#include "llvmgen/ast/binop.h"

namespace grlang
{

namespace llvmgen
{

llvm::Value *BinOpNode::CodeGen(Context &ctx)
{
    auto *lhs = left_->CodeGen(ctx);
    auto *rhs = right_->CodeGen(ctx);
    auto builder = ctx.GetBuilder();

    switch (op_) {
        /* Arithmetic */
        case BinOp::ADD:
            return builder->CreateAdd(lhs, rhs);
        case BinOp::SUB:
            return builder->CreateSub(lhs, rhs);
        case BinOp::MUL:
            return builder->CreateMul(lhs, rhs);
        case BinOp::DIV:
            return builder->CreateSDiv(lhs, rhs);
        case BinOp::MOD:
            return builder->CreateSRem(lhs, rhs);
        case BinOp::AND:
            return builder->CreateAnd(lhs, rhs);
        case BinOp::OR:
            return builder->CreateOr(lhs, rhs);
        case BinOp::XOR:
            return builder->CreateXor(lhs, rhs);

        /* Comparison */
        case BinOp::EQ:
            return builder->CreateICmpEQ(lhs, rhs);
        case BinOp::NEQ:
            return builder->CreateICmpNE(lhs, rhs);
        case BinOp::GT:
            return builder->CreateICmpSGT(lhs, rhs);
        case BinOp::LT:
            return builder->CreateICmpSLT(lhs, rhs);
        case BinOp::GTE:
            return builder->CreateICmpSGE(lhs, rhs);
        case BinOp::LTE:
            return builder->CreateICmpSLE(lhs, rhs);
        default:
            throw std::runtime_error("Unrecognized binary operation type");
            return nullptr;
    }
}

} // namespace llvmgen
} // namespace grlang
