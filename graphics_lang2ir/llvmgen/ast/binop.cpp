#include "llvmgen/ast/binop.h"

namespace grlang
{

namespace llvmgen
{

llvm::Value *BinOpNode::LLVMGen(Context &ctx)
{
    auto *lhs = left_->LLVMGen(ctx);
    auto *rhs = right_->LLVMGen(ctx);
    auto builder = ctx.GetBuilder();

    switch (op_) {
        // Arithmetic
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

        // Comparison
        case BinOp::EQ:
            return builder->CreateZExt(builder->CreateICmpEQ(lhs, rhs), ctx.GetIntTy());
        case BinOp::NEQ:
            return builder->CreateZExt(builder->CreateICmpNE(lhs, rhs), ctx.GetIntTy());
        case BinOp::GT:
            return builder->CreateZExt(builder->CreateICmpSGT(lhs, rhs), ctx.GetIntTy());
        case BinOp::LT:
            return builder->CreateZExt(builder->CreateICmpSLT(lhs, rhs), ctx.GetIntTy());
        case BinOp::GTE:
            return builder->CreateZExt(builder->CreateICmpSGE(lhs, rhs), ctx.GetIntTy());
        case BinOp::LTE:
            return builder->CreateZExt(builder->CreateICmpSLE(lhs, rhs), ctx.GetIntTy());
        default:
            throw std::runtime_error("Unrecognized binary operation type");
            return nullptr;
    }
}

} // namespace llvmgen
} // namespace grlang
