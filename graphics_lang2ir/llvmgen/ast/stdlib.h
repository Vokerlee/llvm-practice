#ifndef GRLANG_LLVMGEN_AST_IO_H
#define GRLANG_LLVMGEN_AST_IO_H

#include "common/config.h"
#include "common/macros.h"
#include "llvmgen/context.h"
#include "llvmgen/ast/base.h"

namespace grlang
{

namespace llvmgen
{

constexpr std::string_view STDLIB_PRINT_FUNC_NAME      = "__print_";
constexpr std::string_view STDLIB_SCAN_FUNC_NAME       = "__scan_";
constexpr std::string_view STDLIB_SGL_INIT_FUNC_NAME   = "__sgl_initialize_";
constexpr std::string_view STDLIB_SGL_CLOSE_FUNC_NAME  = "__sgl_close_";
constexpr std::string_view STDLIB_SGL_RAND_FUNC_NAME   = "__sgl_rand_";
constexpr std::string_view STDLIB_SGL_SRAND_FUNC_NAME  = "__sgl_srand_";
constexpr std::string_view STDLIB_SGL_UPDATE_FUNC_NAME = "__sgl_update_";

class PrintNode : public Node
{
public:
    PrintNode(NodePtr expr) :
        expr_(expr)
    {}

    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto *value = ctx.GetBuilder()->CreateZExt(expr_->LLVMGen(ctx), ctx.GetIntTy());

        auto func = ctx.GetModule()->getFunction(STDLIB_PRINT_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find print() function"};

        return ctx.GetBuilder()->CreateCall(func, {value});
    }

private:
    NodePtr expr_;
};

class ScanNode : public Node
{
public:
    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto func = ctx.GetModule()->getFunction(STDLIB_SCAN_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find scan() function"};

        return ctx.GetBuilder()->CreateCall(func);
    }
};

class SGLInitNode : public Node
{
public:
    SGLInitNode(NodePtr expr1, NodePtr expr2) :
        expr1_(expr1),
        expr2_(expr2)
    {}

    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto *value1 = ctx.GetBuilder()->CreateZExt(expr1_->LLVMGen(ctx), ctx.GetIntTy());
        auto *value2 = ctx.GetBuilder()->CreateZExt(expr2_->LLVMGen(ctx), ctx.GetIntTy());

        auto func = ctx.GetModule()->getFunction(STDLIB_SGL_INIT_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find sgl_initialize() function"};

        return ctx.GetBuilder()->CreateCall(func, {value1, value2});
    }

private:
    NodePtr expr1_;
    NodePtr expr2_;
};

class SGLCloseNode : public Node
{
public:
    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto func = ctx.GetModule()->getFunction(STDLIB_SGL_CLOSE_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find sgl_close() function"};

        return ctx.GetBuilder()->CreateCall(func);
    }
};

class SGLRandNode : public Node
{
public:
    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto func = ctx.GetModule()->getFunction(STDLIB_SGL_RAND_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find sgl_rand() function"};

        return ctx.GetBuilder()->CreateCall(func);
    }
};

class SGLSrandNode : public Node
{
public:
    SGLSrandNode(NodePtr expr) :
        expr_(expr)
    {}

    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto *value = ctx.GetBuilder()->CreateZExt(expr_->LLVMGen(ctx), ctx.GetIntTy());

        auto func = ctx.GetModule()->getFunction(STDLIB_SGL_SRAND_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find sgl_srand() function"};

        return ctx.GetBuilder()->CreateCall(func, {value});
    }

private:
    NodePtr expr_;
};

class SGLUpdateNode : public Node
{
public:
    SGLUpdateNode(StoreablePtr array) :
        array_(array)
    {}

    llvm::Value *LLVMGen(Context &ctx) override
    {
        auto *value = array_->LLVMGen(ctx);

        auto func = ctx.GetModule()->getFunction(STDLIB_SGL_UPDATE_FUNC_NAME);
        if (func == nullptr)
            throw std::runtime_error{"Cannot find sgl_update() function"};

        return ctx.GetBuilder()->CreateCall(func, {value});
    }

private:
    StoreablePtr array_;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_IO_H
