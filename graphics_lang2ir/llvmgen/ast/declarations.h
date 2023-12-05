#ifndef GRLANG_LLVMGEN_AST_DECLARATIONS_H
#define GRLANG_LLVMGEN_AST_DECLARATIONS_H

#include "llvmgen/ast/base.h"

namespace grlang
{

namespace llvmgen
{

class VarDeclNode : public StoreableNode
{
public:
    VarDeclNode(llvm::Type *type, const std::string &name, NodePtr init_expr = nullptr) :
        StoreableNode(name), type_(type), init_expr_(init_expr)
    {}

    DEFAULT_COPY_SEMANTIC(VarDeclNode);

    void Store(Context &ctx, llvm::Value *value) override
    {
        if (alloca_ == nullptr)
            throw std::runtime_error{"Trying to assign value to undeclared variable"};

        if (!type_->isIntegerTy())
            throw std::runtime_error{"Trying to assign value to non-scalar variable"};

        ctx.GetBuilder().CreateStore(value, alloca_);
    }

    llvm::Value *CodeGen(Context &ctx) override
    {
        // If CodeGen() wal already called, just return its value
        if (alloca_ != nullptr)
            return ctx.GetBuilder().CreateLoad(type_, alloca_);

        alloca_ = ctx.GetBuilder().CreateAlloca(type_);
        if (type_->isIntegerTy()) {
            if (init_expr_)
                ctx.GetBuilder().CreateStore(init_expr_->CodeGen(ctx), alloca_);

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

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_DECLARATIONS_H
