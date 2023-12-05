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

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

namespace grlang
{

class CodegenContext
{
public:
    CodegenContext() :
        context_(),
        module_(std::make_unique<llvm::Module>("top", context_)),
        builder_(context_)
    {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
    }

    auto getIntTy()
    {
        return builder_.getInt64Ty();
    }

    auto getInt(Int val)
    {
        return builder_.getInt64(static_cast<uint64_t>(val));
    }

    static auto getArrTy(llvm::Type *type, uint64_t size)
    {
        return llvm::ArrayType::get(type, size);
    }

private:
    llvm::LLVMContext context_;
    std::unique_ptr<llvm::Module> module_;
    llvm::IRBuilder<> builder_;
};

// enum class BinOp
// {
//     INVALID,

//     // Arithmetic
//     ADD,
//     SUB,
//     MUL,
//     DIV,
//     MOD,
//     AND,
//     OR,
//     XOR,

//     // Comparison
//     EQ,
//     NEQ,
//     GT,
//     LT,
//     GTE,
//     LTE,
// };

// struct INode
// {
//     virtual llvm::Value *CodeGen(CodegenContext &) = 0;
//     virtual ~INode() = default;
// };

// class DeclNode : public INode
// {
// public:
//     DeclNode(const std::string &name) : name_(name)
//     {}

//     const std::string &GetName() const
//     {
//         return name_;
//     }

//     virtual llvm::Value *CodeGen(CodegenContext &) override = 0;

// private:
//     std::string name_;
// };

} // namespace grlang

#endif // GRLANG_AST_H
