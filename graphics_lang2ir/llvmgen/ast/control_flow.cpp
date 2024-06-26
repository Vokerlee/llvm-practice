#include "llvmgen/ast/control_flow.h"

namespace grlang
{

namespace llvmgen
{

llvm::Value *IfNode::LLVMGen(Context &ctx)
{
    auto llvm_context = ctx.GetLLVMContext();
    auto builder      = ctx.GetBuilder();

    auto cond = condition_->LLVMGen(ctx);
    auto parent_func = parent_scope_.lock()->GetParentFuncDecl();

    auto bb_true    = llvm::BasicBlock::Create(*llvm_context, "true",  parent_func->GetFunc());
    auto bb_false   = llvm::BasicBlock::Create(*llvm_context, "false", parent_func->GetFunc());
    auto bb_cmp_end = llvm::BasicBlock::Create(*llvm_context, "next",  parent_func->GetFunc());

    builder->CreateCondBr(cond, bb_true, bb_false);

    builder->SetInsertPoint(bb_true);
    scope_true_->LLVMGen(ctx);

    if (builder->GetInsertBlock()->getTerminator() == nullptr)
        builder->CreateBr(bb_cmp_end);

    builder->SetInsertPoint(bb_false);

    if (scope_false_ != nullptr)
        scope_false_->LLVMGen(ctx);

    if (builder->GetInsertBlock()->getTerminator() == nullptr)
        builder->CreateBr(bb_cmp_end);

    builder->SetInsertPoint(bb_cmp_end);

  return nullptr;
}

llvm::Value *WhileNode::LLVMGen(Context &ctx)
{
    auto llvm_context = ctx.GetLLVMContext();
    auto builder      = ctx.GetBuilder();

    auto parent_func = parent_scope_.lock()->GetParentFuncDecl();

    auto bb_cond    = llvm::BasicBlock::Create(*llvm_context, "cond", parent_func->GetFunc());
    auto bb_body    = llvm::BasicBlock::Create(*llvm_context, "body", parent_func->GetFunc());
    auto bb_cmp_end = llvm::BasicBlock::Create(*llvm_context, "next", parent_func->GetFunc());

    builder->CreateBr(bb_cond);
    builder->SetInsertPoint(bb_cond);

    auto cond = condition_->LLVMGen(ctx);
    builder->CreateCondBr(cond, bb_body, bb_cmp_end);

    builder->SetInsertPoint(bb_body);
    body_->LLVMGen(ctx);

    if (builder->GetInsertBlock()->getTerminator() == nullptr)
        builder->CreateBr(bb_cond);

    builder->SetInsertPoint(bb_cmp_end);

    return nullptr;
}

} // namespace llvmgen
} // namespace grlang
