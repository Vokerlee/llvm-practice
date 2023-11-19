#ifndef GRASM_PARSER_H
#define GRASM_PARSER_H

#include "common/macros.h"
#include "CPU/CPU.h"
#include "CPU/instruction.h"
#include "CPU/instruction_exec.h"

#include <vector>
#include <fstream>

#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

namespace grasm
{

class Executor
{
public:
    NO_COPY_SEMANTIC(Executor);
    NO_MOVE_SEMANTIC(Executor);

    Executor() :
        builder_(context_)
    {
        module_ = new llvm::Module("top", context_);
    }

    ~Executor() = default;

    bool ParseAsmFile(const char *filename);
    bool Execute();

private:
    std::vector<Instruction> instrs_;

    llvm::LLVMContext context_;
    llvm::Module *module_ {nullptr};
    llvm::IRBuilder<> builder_;
};

}; // grasm

#endif // GRASM_PARSER_H
