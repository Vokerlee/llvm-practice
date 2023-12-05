#include "llvmgen/ast/func.h"

#include <sstream>

namespace grlang
{

namespace llvmgen
{

llvm::Value *FuncParamNode::CodeGen(Context &ctx)
{
    if (GetAlloca() != nullptr)
        return ctx.GetBuilder()->CreateLoad(GetTy(), GetAlloca());

    SetAlloca(ctx.GetBuilder()->CreateAlloca(GetTy()));
    ctx.GetBuilder()->CreateStore(arg_, GetAlloca());

    return nullptr;
}

void FuncProtNode::CreateFuncSignature(Context &ctx)
{
    // Already existent LLVM function declaration
    if (func_ != nullptr)
        return;

    if (ret_type_ == nullptr)
        ret_type_ = ctx.GetBuilder()->getVoidTy();

    std::vector<llvm::Type *> arg_types;
    for (auto &&param : params_)
        arg_types.push_back(param->GetTy());

    auto func_type = arg_types.empty() ? llvm::FunctionType::get(ret_type_, false) :
                                         llvm::FunctionType::get(ret_type_, arg_types, false);

    func_ = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                   GetName(), *ctx.GetModule());

    // Add all allocas to scope
    for (size_t i = 0; i < params_.size(); ++i)
        params_[i]->SetArg(func_->getArg(i));
}

llvm::Value *FuncProtNode::CodeGen(Context &ctx)
{
    CreateFuncSignature(ctx);

    if (is_just_decl_)
        return nullptr;

    auto bb_init = llvm::BasicBlock::Create(*ctx.GetLLVMContext(), "", func_);
    ctx.GetBuilder()->SetInsertPoint(bb_init);

    // Load all passed arguments in teh start of the function
    for (auto &&param : params_)
        param->CodeGen(ctx);

    body_->CodeGen(ctx);
    ctx.GetBuilder()->ClearInsertionPoint();

    return nullptr;
}

llvm::Value *FuncCallNode::CodeGen(Context &ctx)
{
    auto decl = std::dynamic_pointer_cast<FuncProtNode>(func_decl_.lock());

    std::vector<llvm::Value *> vals;
    for (auto &&arg : args_)
        vals.push_back(arg->CodeGen(ctx));

    auto *func = decl->GetFunc();

    return ctx.GetBuilder()->CreateCall(func, vals);
}

DeclPtr FuncScopeNode::FindName(const std::string &name) const
{
    auto it = symbol_table_.find(name);
    if (it != symbol_table_.end())
        return it->second;

    auto parent_func = parent_func_.lock();
    if (parent_func != nullptr && name == parent_func->GetName())
        return parent_func;

    parent_func.reset();

    auto parent_ptr = parent_.lock();
    if (parent_ptr != nullptr)
        return parent_ptr->FindName(name); // find in upper scope

    std::ostringstream ss;
    ss << "Can't find symbol with name '" << name << '\'';
    throw std::runtime_error(ss.str());
}

} // namespace llvmgen
} // namespace grlang
