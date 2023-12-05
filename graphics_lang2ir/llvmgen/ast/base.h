#ifndef GRLANG_LLVMGEN_AST_BASE_H
#define GRLANG_LLVMGEN_AST_BASE_H

#include "common/config.h"
#include "common/macros.h"
#include "llvmgen/context.h"
#include "llvmgen/ast/astdefs.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace grlang
{

namespace llvmgen
{

using NodePtr      = std::shared_ptr<Node>;
using DeclPtr      = std::shared_ptr<DeclNode>;
using StoreablePtr = std::shared_ptr<StoreableNode>;
using BinOpPtr     = std::shared_ptr<BinOpNode>;

struct Node
{
    virtual llvm::Value *CodeGen(Context &) = 0;
    virtual ~Node() = default;
};

class DeclNode : public Node
{
public:
    DeclNode(const std::string &name) :
        name_(name)
    {}

    const std::string &GetName() const
    {
        return name_;
    }

    virtual llvm::Value *CodeGen(Context &) override = 0;

private:
    std::string name_;
};

class StoreableNode : public DeclNode
{
public:
    StoreableNode(const std::string &name) : DeclNode(name)
    {}
    virtual void Store(Context &ctx, llvm::Value *value) = 0;
};

class AssignNode : public Node
{
public:
    AssignNode(StoreablePtr lhs, NodePtr rhs) :
        lhs_(lhs), rhs_(rhs)
    {}

    llvm::Value *CodeGen(Context &ctx) override
    {
        lhs_->Store(ctx, rhs_->CodeGen(ctx));

        return nullptr;
    }

private:
    StoreablePtr lhs_ {};
    NodePtr      rhs_ {};
};

class IntNode final : public Node
{
public:
    IntNode(Int value) :
        value_(value)
    {}

    auto GetVal() const
    {
        return value_;
    }

    llvm::Value *CodeGen(Context &ctx) override
    {
        return llvm::ConstantInt::get(ctx.GetIntTy(), static_cast<uint64_t>(value_), true);
    }

private:
    const Int value_;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_BASE_H
