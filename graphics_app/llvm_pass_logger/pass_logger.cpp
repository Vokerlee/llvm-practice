#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace
{

struct SimpleLogger : public FunctionPass
{
    static char ID;
    SimpleLogger() : FunctionPass(ID) {}

    bool isFuncLogger(StringRef name)
	{
        return name == "op_logger" || name == "call_logger";
    }

    virtual bool runOnFunction(Function &F)
	{
        if (isFuncLogger(F.getName()))
            return false;

        outs() << "In a function called " << F.getName() << "\n\n";

        for (auto &B : F)
		{
            for (auto &I : B)
			{
                outs() << "Instruction: " << (uint64_t)(&I) << "\n";
                I.print(outs(), true);
                outs() << "\n";
            }

            outs() << "\n";
        }

        // Prepare builder for IR modification
        LLVMContext &context = F.getContext();
        IRBuilder<> builder(context);

        FunctionType *loggerType =
            FunctionType::get(builder.getVoidTy(),
                              ArrayRef<Type *>({builder.getInt8Ty()->getPointerTo()}),
                              false);

        FunctionCallee opLogFunc =
            F.getParent()->getOrInsertFunction("op_logger", loggerType);

        FunctionCallee callLogFunc =
			F.getParent()->getOrInsertFunction("call_logger", loggerType);

        // Insert loggers for call, binOpt and ret instructions
        for (auto &B : F)
		{
            for (auto &I : B)
			{
                builder.SetInsertPoint(&I);

                if (auto *phi = dyn_cast<PHINode>(&I))
                    continue;

                if (auto *call = dyn_cast<CallInst>(&I))
                {
                    Function *callee = call->getCalledFunction();
                    if (callee && isFuncLogger(callee->getName()))
                        continue;
                    else if (callee)
                    {
                        Value *funcName = builder.CreateGlobalStringPtr(callee->getName());
                        builder.CreateCall(callLogFunc, {funcName});
                        continue;
                    }
                }

                Value *opName = builder.CreateGlobalStringPtr(I.getOpcodeName());
                // outs() << I.getOpcodeName() << "\n";
                if (opName)
                    builder.CreateCall(opLogFunc, {opName});
            }
        }

        return true;
    }
};
} // namespace

char SimpleLogger::ID = 0;

static RegisterPass<SimpleLogger> X("logger", "Simple Logger Pass", false, false);
