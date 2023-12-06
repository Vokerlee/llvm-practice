#ifndef GRLANG_LLVMGEN_AST_DECLARATIONS_H
#define GRLANG_LLVMGEN_AST_DECLARATIONS_H

#include "llvmgen/ast/base.h"

namespace grlang
{

namespace llvmgen
{

using VarPtr = std::shared_ptr<VarNode>;

// Both for variables and arrays
class VarNode : public StoreableNode
{
public:
    VarNode(llvm::Type *type, const std::string &name, NodePtr init_expr = nullptr) :
        StoreableNode(name), type_(type), init_expr_(init_expr)
    {}

    DEFAULT_COPY_SEMANTIC(VarNode);

    void Store(Context &ctx, llvm::Value *value) override
    {
        if (alloca_ == nullptr)
            throw std::runtime_error("Trying to assign value to undeclared variable");

        if (!type_->isIntegerTy())
            throw std::runtime_error("Trying to assign value to non-scalar variable");

        ctx.GetBuilder()->CreateStore(value, alloca_);
    }

    llvm::Value *CodeGen(Context &ctx) override
    {
        // If CodeGen() wal already called, just return its value
        if (alloca_ != nullptr)
            return ctx.GetBuilder()->CreateLoad(type_, alloca_);

        alloca_ = ctx.GetBuilder()->CreateAlloca(type_);
        if (type_->isIntegerTy()) {
            if (init_expr_)
                ctx.GetBuilder()->CreateStore(init_expr_->CodeGen(ctx), alloca_);

            return nullptr;
        }

        if (type_->isArrayTy())
            return nullptr;

        throw std::runtime_error("Invalid type");
    }

    void SetAlloca(llvm::Value *alloca)
    {
        alloca_ = alloca;
    }

    auto GetAlloca() const
    {
        return alloca_;
    }

    auto GetTy() const
    {
        return type_;
    }

    auto GetInitExpr() const
    {
        return init_expr_;
    }

private:
    llvm::Value *alloca_    {};
    llvm::Type  *type_      {};
    NodePtr      init_expr_ {};
};

class ArrayAccessNode : public StoreableNode
{
public:
    ArrayAccessNode(const std::string &name, VarPtr var_ptr, NodePtr idx)
        : StoreableNode(name), array_(var_ptr), idx_(idx)
    {}

    auto MakeGEP(Context &ctx)
    {
        auto *idx = idx_->CodeGen(ctx);
        auto array = array_.lock();

        if (!(array->GetTy())->isArrayTy())
            throw std::runtime_error{"Trying to index a non-array symbol"};

        return ctx.GetBuilder()->CreateInBoundsGEP(array->GetTy(), array->GetAlloca(), {ctx.GetBuilder()->getInt32(0), idx});
    }

    llvm::Value *CodeGen(Context &ctx) override
    {
        return ctx.GetBuilder()->CreateLoad(array_.lock()->GetTy()->getArrayElementType(), MakeGEP(ctx));
    }

    void Store(Context &ctx, llvm::Value *value) override
    {
        ctx.GetBuilder()->CreateStore(value, MakeGEP(ctx));
    }

private:
    std::weak_ptr<VarNode> array_{};
    NodePtr idx_{};
};

class GlobVarNode : public VarNode
{
public:
    GlobVarNode(VarPtr var_ptr) :
        VarNode(*var_ptr)
    {}

    llvm::Value *CodeGen(Context &ctx) override
    {
        if (GetAlloca() != nullptr)
            return ctx.GetBuilder()->CreateLoad(GetTy(), GetAlloca());

        ctx.GetModule()->getOrInsertGlobal(GetName(), GetTy());
        auto *ptr = ctx.GetModule()->getNamedGlobal(GetName());
        ptr->setLinkage(llvm::GlobalVariable::ExternalLinkage);

        if (GetTy()->isArrayTy())
            ptr->setInitializer(llvm::ConstantAggregateZero::get(GetTy()));
        else if (GetTy()->isIntegerTy())
            ptr->setInitializer(llvm::ConstantInt::get(ctx.GetIntTy(), 0));
        else
            throw std::runtime_error("Invalid type");

        ptr->setConstant(false);
        SetAlloca(ptr);

        return ptr;
    }
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_DECLARATIONS_H
