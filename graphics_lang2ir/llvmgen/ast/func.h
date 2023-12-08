#ifndef GRLANG_LLVMGEN_AST_FUNC_H
#define GRLANG_LLVMGEN_AST_FUNC_H

#include "llvmgen/ast/base.h"
#include "llvmgen/ast/vars.h"

#include <iostream>

namespace grlang
{

namespace llvmgen
{

using FuncParamPtr = std::shared_ptr<FuncParamNode>;
using FuncScopePtr = std::shared_ptr<FuncScopeNode>;
using FuncProtPtr  = std::shared_ptr<FuncProtNode>;

class FuncParamNode : public VarNode
{
public:
    FuncParamNode(llvm::Type *type, const std::string &name) :
        VarNode(type, name)
    {}

    DEFAULT_COPY_SEMANTIC(FuncParamNode);

    void SetArg(llvm::Argument *arg)
    {
        arg_ = arg;
    }

    llvm::Value *LLVMGen(Context &ctx) override;

private:
    llvm::Argument *arg_ = nullptr;
};

class FuncScopeNode : public Node
{
public:
    FuncScopeNode() = default;
    FuncScopeNode(FuncScopePtr parent) :
        parent_(parent), parent_func_(parent->parent_func_)
    {}

    void SetParentFuncDecl(FuncProtPtr func)
    {
        parent_func_ = func;
    }

    auto GetParentFuncDecl() const
    {
        return parent_func_.lock();
    }

    auto GetParent() const
    {
        return parent_.lock();
    }

    void PushNode(NodePtr child)
    {
        children_.push_back(child);
    }

    void PushScope(FuncScopePtr child)
    {
        children_scopes_.push_back(child);
    }

    void AddDecl(DeclPtr decl, bool to_push = false)
    {
        symbol_table_[decl->GetName()] = decl;
        if (to_push == true)
            PushNode(decl);
    }

    DeclPtr FindName(const std::string &name) const;

    llvm::Value *LLVMGen(Context &ctx) override
    {
        for (auto &&child : children_)
        {
            auto bb = ctx.GetBuilder()->GetInsertBlock();
            if (bb == nullptr || bb->getTerminator() == nullptr)
                child->LLVMGen(ctx);
        }

        return nullptr;
    }

private:
    std::unordered_map<std::string, DeclPtr> symbol_table_ {};

    std::vector<NodePtr>      children_        {};
    std::vector<FuncScopePtr> children_scopes_ {};

    std::weak_ptr<FuncScopeNode> parent_      {};
    std::weak_ptr<FuncProtNode>  parent_func_ {};
};

class FuncProtNode : public DeclNode
{
public:
    FuncProtNode(const std::string &name, const std::vector<FuncParamPtr> &params,
                FuncScopePtr body, llvm::Type *ret = nullptr) :
        DeclNode(name), ret_type_(ret), params_(params), body_(body)
    {
        for (auto &&param : params_)
            body_->AddDecl(param);
    }

    DEFAULT_COPY_SEMANTIC(FuncProtNode);

    llvm::Value *LLVMGen(Context &ctx) override;

    void MarkAsDecl()
    {
        is_just_decl_ = true;
    }

    bool HasDefinition() const
    {
        return !is_just_decl_;
    }

    auto GetFunc() const
    {
        return func_;
    }

private:
    void CreateFuncSignature(Context &ctx);

private:
    llvm::Function *func_     {};
    llvm::Type     *ret_type_ {};

    bool is_just_decl_ {false};

    std::vector<FuncParamPtr> params_;
    FuncScopePtr              body_ {};
};

class FuncCallNode : public Node
{
public:
    FuncCallNode(NodePtr decl, const std::vector<NodePtr> &args = {}) :
        func_decl_(decl), args_(args)
    {}

    llvm::Value *LLVMGen(Context &ctx) override;

private:
    std::weak_ptr<Node>  func_decl_ {};
    std::vector<NodePtr> args_      {};
};

class FuncRetNode : public Node
{
public:
    FuncRetNode(NodePtr expr = nullptr) :
        expr_(expr)
    {}

    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto *expr = expr_ ? ctx.GetBuilder()->CreateZExt(expr_->LLVMGen(ctx), ctx.GetIntTy()) : nullptr;
        return ctx.GetBuilder()->CreateRet(expr);
    }

private:
    NodePtr expr_;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_FUNC_H
