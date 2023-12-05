#ifndef GRLANG_LLVMGEN_CONTEXT_H
#define GRLANG_LLVMGEN_CONTEXT_H

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

    llvm::IRBuilder<> *GetBuilder()
    {
        return &builder_;
    }

private:
    llvm::LLVMContext context_;
    std::unique_ptr<llvm::Module> module_;
    llvm::IRBuilder<> builder_;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_CONTEXT_H
