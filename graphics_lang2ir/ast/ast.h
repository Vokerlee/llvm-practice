#ifndef GRLANG_AST_H
#define GRLANG_AST_H

#include "common/config.h"
#include "common/macros.h"

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

class Context
{
public:
    Context() :
        context_(),
        module_(std::make_unique<llvm::Module>("top", context_)),
        builder_(context_)
    {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
    }

    NO_COPY_SEMANTIC(Context);
    NO_MOVE_SEMANTIC(Context);

    auto GetIntTy()
    {
        return builder_.getInt64Ty();
    }

    auto GetInt(Int val)
    {
        return builder_.getInt64(static_cast<uint64_t>(val));
    }

    static auto GetArrTy(llvm::Type *type, uint64_t size)
    {
        return llvm::ArrayType::get(type, size);
    }

private:
    llvm::LLVMContext context_;
    std::unique_ptr<llvm::Module> module_;
    llvm::IRBuilder<> builder_;
};

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

struct Node
{
    virtual llvm::Value *CodeGen(Context &) = 0;
    virtual ~Node() = default;
};

class DeclNode : public Node
{
public:
    DeclNode(const std::string &name) : name_(name)
    {}

    const std::string &GetName() const
    {
        return name_;
    }

    virtual llvm::Value *CodeGen(Context &) override = 0;

private:
    std::string name_;
};

class IntNode final : public Node
{
public:
    IntNode(Int value) : value_(value)
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

class BinOpNode : public Node
{
public:
    BinOpNode(std::shared_ptr<Node> left, BinOp op, std::shared_ptr<Node> right)
        : left_(left), right_(right), op_(op)
    {}

    llvm::Value *CodeGen(Context &ctx) override;

private:
    std::shared_ptr<Node> left_  {};
    std::shared_ptr<Node> right_ {};
    BinOp                 op_    {BinOp::INVALID};
};

// TODO: delete if it is unuseful
class PrintNode : public Node
{
public:
    PrintNode(std::shared_ptr<Node> expr) : expr_(expr)
    {}

    llvm::Value *CodeGen(Context &ctx) override;

private:
    std::shared_ptr<Node> expr_;
};

// TODO: delete if it is unuseful
class ScanNode : public Node
{
public:
    llvm::Value *CodeGen(Context &ctx) override;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_AST_H
