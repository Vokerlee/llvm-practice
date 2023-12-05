#ifndef GRLANG_COMMON_CONFIG_H
#define GRLANG_COMMON_CONFIG_H

#include <cstdint>
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

using Int = int64_t;

}; // grlang

#endif // GRLANG_COMMON_CONFIG_H
