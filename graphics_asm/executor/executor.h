#ifndef GRASM_PARSER_H
#define GRASM_PARSER_H

#include "common/macros.h"
#include "CPU/CPU.h"
#include "CPU/instruction.h"
#include "CPU/instruction_exec.h"

#include <vector>
#include <unordered_map>
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

    Executor(CPU *cpu) :
        cpu_(cpu),
        builder_(context_)
    {
        module_ = new llvm::Module("top", context_);

        llvm::FunctionType *void_type = llvm::FunctionType::get(builder_.getVoidTy(), false);
        main_func_ = llvm::Function::Create(void_type, llvm::Function::ExternalLinkage, "main", module_);

        bb_map_["__start"] = llvm::BasicBlock::Create(context_, "__start", main_func_);

        llvm::ArrayType *reg_file_type = llvm::ArrayType::get(builder_.getInt64Ty(), CPU::REG_FILE_SIZE);
        module_->getOrInsertGlobal("reg_file", reg_file_type);
    }

    ~Executor() = default;

    bool ParseAsmFile(const char *filename);
    bool Execute();

private:
    CPU *cpu_ {nullptr};

    std::vector<Instruction> instrs_;
    std::unordered_map<std::string, llvm::BasicBlock *> bb_map_;

    llvm::LLVMContext context_;
    llvm::Module *module_ {nullptr};
    llvm::IRBuilder<> builder_;

    llvm::Function *main_func_ {nullptr};
};

}; // grasm

#endif // GRASM_PARSER_H
