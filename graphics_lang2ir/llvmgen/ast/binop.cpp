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
            return builder->CreateAdd(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::SUB:
            return builder->CreateSub(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::MUL:
            return builder->CreateMul(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::DIV:
            return builder->CreateSDiv(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::MOD:
            return builder->CreateSRem(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::AND:
            return builder->CreateAnd(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::OR:
            return builder->CreateOr(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));
        case BinOp::XOR:
            return builder->CreateXor(builder->CreateZExt(lhs, ctx.GetIntTy()), builder->CreateZExt(rhs, ctx.GetIntTy()));

        // Comparison
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
