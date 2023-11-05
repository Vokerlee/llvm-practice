#include "sgl/sgl.hpp"
#include "llvm_ir_gen_macros.h"

#include <vector>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

int main(int argc, char *argv[])
{
    LLVMContext context;

    Module *module = new Module("gameoflife.c", context);
    IRBuilder<> builder(context);

    // Declarations of all used functions

    FunctionType *emptyType =
        FunctionType::get(builder.getVoidTy(), ArrayRef<Type *>(), false);
    FunctionType *int32ArgType =
        FunctionType::get(builder.getInt32Ty(), ArrayRef<Type *>(), false);
    FunctionType *int32RetType =
        FunctionType::get(builder.getVoidTy(), ArrayRef<Type *>({builder.getInt32Ty()}), false);
    FunctionType *ptrArgType =
        FunctionType::get(builder.getVoidTy(), ArrayRef<Type *>({builder.getPtrTy()}), false);

    Function *gameOfLifeStartFunc =
        Function::Create(emptyType, Function::ExternalLinkage, "gameoflife_start", module);
    FunctionCallee sglRand =
        module->getOrInsertFunction("sgl_rand", int32ArgType);
    FunctionCallee sglSrand =
        module->getOrInsertFunction("sgl_srand", int32RetType);
    FunctionCallee sglUpdate =
        module->getOrInsertFunction("sgl_update", ptrArgType);

    // 2 buffer of pixels type

    ArrayType *innerArrayType  = ArrayType::get(builder.getInt32Ty(), SGL_WIDTH_DEFAULT);
    ArrayType *middleArrayType = ArrayType::get(innerArrayType, SGL_HEIGHT_DEFAULT);
    ArrayType *outerArrayType  = ArrayType::get(middleArrayType, 2);\

    // All basic blocks

    size_t nBasicBlocks = 16;
    std::vector<BasicBlock *> BB;
    for (size_t i = 0; i < nBasicBlocks; ++i)
        BB.push_back(BasicBlock::Create(context, "", gameOfLifeStartFunc));

// gameoflife_start: BB0

    builder.SetInsertPoint(BB[0]);

    AllocaInst *allocaInst = builder.CreateAlloca(outerArrayType, nullptr, "");
    allocaInst->setAlignment(Align(16));

    Value *gepInst1 = GEP_ROW(outerArrayType, allocaInst, GET_INT64(0, false), GET_INT64(1, false));

    CallInst *callInst1 = builder.CreateCall(sglSrand,
        ArrayRef<Value *>({GET_INT32(-21731286, true)}));
    callInst1->setTailCall();

    builder.CreateBr(BB[1]);

// gameoflife_start: BB1

    builder.SetInsertPoint(BB[1]);

    PHINode *phiNode1 = builder.CreatePHI(Type::getInt64Ty(context), 2, "");
    phiNode1->addIncoming(GET_INT64(0, false), BB[0]);

    builder.CreateBr(BB[3]);

// gameoflife_start: BB2

    builder.SetInsertPoint(BB[2]);

    PUT_ADD_NUW_NSW(addRes1, phiNode1, GET_INT64(1, false));
    phiNode1->addIncoming(addRes1, BB[2]);

    Value* icmpRes1 = builder.CreateICmpEQ(addRes1, GET_INT64(SGL_HEIGHT_DEFAULT, false), "");
    builder.CreateCondBr(icmpRes1, BB[4], BB[1]);

// gameoflife_start: BB3

    builder.SetInsertPoint(BB[3]);

    PHINode *phiNode2 = builder.CreatePHI(Type::getInt64Ty(context), 2, "");
    phiNode2->addIncoming(GET_INT64(0, false), BB[1]);

    CallInst *callInst2 = builder.CreateCall(sglRand, ArrayRef<Value *>());
    callInst2->setTailCall();

    Value* sremRes1 = builder.CreateSRem(callInst2, GET_INT32(2, false), "");
    Value* mulRes1  = builder.CreateNSWMul(sremRes1, GET_INT32(-16733645, true), "");

    Value *gepInst2 = GEP_ROW(innerArrayType, gepInst1, phiNode1, phiNode2);
    builder.CreateStore(mulRes1, gepInst2)->setAlignment(Align(4));

    PUT_ADD_NUW_NSW(addRes2, phiNode2, GET_INT64(1, false));
    phiNode2->addIncoming(addRes2, BB[3]);

    Value* icmpRes2 = builder.CreateICmpEQ(addRes2, GET_INT64(SGL_WIDTH_DEFAULT, false), "");
    builder.CreateCondBr(icmpRes2, BB[2], BB[3]);

// gameoflife_start: BB4

    builder.SetInsertPoint(BB[4]);
    CallInst *callInst3 = builder.CreateCall(sglUpdate, ArrayRef<Value *>({gepInst1}));

    builder.CreateBr(BB[5]);

// gameoflife_start: BB5

    builder.SetInsertPoint(BB[5]);

    PHINode *phiNode3 = builder.CreatePHI(Type::getInt32Ty(context), 2, "");
    PHINode *phiNode4 = builder.CreatePHI(Type::getInt32Ty(context), 2, "");
    phiNode3->addIncoming(GET_INT32(0, false), BB[4]);
    phiNode4->addIncoming(GET_INT32(0, false), BB[4]);

    PUT_SUB_NUW_NSW(addSub1, GET_INT32(1, false), phiNode3);
    Value *zextRes1 = builder.CreateZExt(addSub1, Type::getInt64Ty(context), "");

    Value *zextRes2 = builder.CreateZExt(phiNode3, Type::getInt64Ty(context), "");

    builder.CreateBr(BB[7]);

// gameoflife_start: BB6

    builder.SetInsertPoint(BB[6]);
    builder.CreateRetVoid();

// gameoflife_start: BB7

    builder.SetInsertPoint(BB[7]);

    PHINode *phiNode5 = builder.CreatePHI(Type::getInt64Ty(context), 2, "");
    phiNode5->addIncoming(GET_INT64(0, false), BB[5]);
    Value *truncRes1 = builder.CreateTrunc(phiNode5, Type::getInt32Ty(context), "");

    Value *addRes3   = builder.CreateAdd(truncRes1, GET_INT32(SGL_HEIGHT_DEFAULT - 1, false), "");
    Value *uremRes1  = builder.CreateURem(addRes3, GET_INT32(SGL_HEIGHT_DEFAULT, false), "");
    Value *zextRes3  = builder.CreateZExt(uremRes1, Type::getInt64Ty(context), "");
    Value *truncRes2 = builder.CreateTrunc(phiNode5, Type::getInt32Ty(context), "");

    Value *addRes4   = builder.CreateAdd(truncRes2, GET_INT32(SGL_HEIGHT_DEFAULT, false), "");
    Value *uremRes2  = builder.CreateURem(addRes4, GET_INT32(SGL_HEIGHT_DEFAULT, false), "");
    Value *zextRes4  = builder.CreateZExt(uremRes2, Type::getInt64Ty(context), "");
    Value *truncRes3 = builder.CreateTrunc(phiNode5, Type::getInt32Ty(context), "");

    Value *addRes5   = builder.CreateAdd(truncRes3, GET_INT32(SGL_HEIGHT_DEFAULT + 1, false), "");
    Value *zextRes5  = builder.CreateURem(addRes5, GET_INT32(SGL_HEIGHT_DEFAULT, false), "");
    Value *truncRes4 = builder.CreateZExt(zextRes5, Type::getInt64Ty(context), "");

    builder.CreateBr(BB[10]);

// gameoflife_start: BB8

    builder.SetInsertPoint(BB[8]);

    Value *gepInst3 = GEP_ROW(outerArrayType, allocaInst, GET_INT64(0, false), zextRes2);
    CallInst *callInst4 = builder.CreateCall(sglUpdate, ArrayRef<Value *>({gepInst3}));

    Value *andRes1 = builder.CreateAnd(phiNode3, GET_INT32(1, false), "");
    Value *xorRes1 = builder.CreateXor(andRes1, GET_INT32(1, false), "");

    PUT_ADD_NUW_NSW(addRes6, phiNode4, GET_INT32(1, false));
    Value *icmpRes3 = builder.CreateICmpEQ(addRes6, GET_INT32(100000, false), "");

    phiNode3->addIncoming(xorRes1, BB[8]);
    phiNode4->addIncoming(addRes6, BB[8]);

    builder.CreateCondBr(icmpRes3, BB[6], BB[5]);

// gameoflife_start: BB9

    builder.SetInsertPoint(BB[9]);

    PUT_ADD_NUW_NSW(addRes7, phiNode5, GET_INT64(1, false));
    Value* icmpRes4 = builder.CreateICmpEQ(addRes7, GET_INT64(SGL_HEIGHT_DEFAULT, false), "");

    phiNode5->addIncoming(addRes7, BB[9]);

    builder.CreateCondBr(icmpRes4, BB[8], BB[7]);

// gameoflife_start: BB10

    builder.SetInsertPoint(BB[10]);

    PHINode *phiNode6 = builder.CreatePHI(Type::getInt64Ty(context), 2, "");
    phiNode6->addIncoming(GET_INT64(0, false), BB[7]);

    PUT_ADD_NUW_NSW(addRes8, phiNode6, GET_INT64(SGL_WIDTH_DEFAULT, false));

    Value *truncRes5 = builder.CreateTrunc(phiNode6, Type::getInt32Ty(context), "");
    Value *addRes9   = builder.CreateAdd(truncRes5, GET_INT32(SGL_WIDTH_DEFAULT - 1, false), "");
    Value *uremRes3  = builder.CreateURem(addRes9, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes6  = builder.CreateZExt(uremRes3, Type::getInt64Ty(context), "");

    Value *gepInst4  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes3, zextRes6);
    PUT_LOAD_32(loadRes1, gepInst4);

    Value *icmpRes5  = builder.CreateICmpUGT(loadRes1, GET_INT32(16777215, false), "");
    Value *zextRes7  = builder.CreateZExt(icmpRes5, Type::getInt32Ty(context), "");
    Value *truncRes6 = builder.CreateTrunc(addRes8, Type::getInt32Ty(context), "");
    Value *uremRes4  = builder.CreateURem(truncRes6, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes8  = builder.CreateZExt(uremRes4, Type::getInt64Ty(context), "");

    Value *gepInst5  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes3, zextRes8);
    PUT_LOAD_32(loadRes2, gepInst5);

    Value *icmpRes6  = builder.CreateICmpUGT(loadRes2, GET_INT32(16777215, false), "");
    Value *zextRes9  = builder.CreateZExt(icmpRes6, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes10, zextRes7, zextRes9);
    Value *truncRes7 = builder.CreateTrunc(phiNode6, Type::getInt32Ty(context), "");
    Value *addRes11  = builder.CreateAdd(truncRes7, GET_INT32(SGL_WIDTH_DEFAULT + 1, false), "");
    Value *uremRes5  = builder.CreateURem(addRes11, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes10 = builder.CreateZExt(uremRes5, Type::getInt64Ty(context), "");

    Value *gepInst6  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes3, zextRes10);
    PUT_LOAD_32(loadRes3, gepInst6);

    Value *icmpRes7  = builder.CreateICmpUGT(loadRes3, GET_INT32(16777215, false), "");
    Value *zextRes11 = builder.CreateZExt(icmpRes7, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes12, addRes10, zextRes11);
    Value *truncRes8 = builder.CreateTrunc(phiNode6, Type::getInt32Ty(context), "");
    Value *addRes13  = builder.CreateAdd(truncRes8, GET_INT32(SGL_WIDTH_DEFAULT - 1, false), "");
    Value *uremRes6  = builder.CreateURem(addRes13, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes12 = builder.CreateZExt(uremRes6, Type::getInt64Ty(context), "");

    Value *gepInst7  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes4, zextRes12);
    PUT_LOAD_32(loadRes4, gepInst7);

    Value *icmpRes8  = builder.CreateICmpUGT(loadRes4, GET_INT32(16777215, false), "");
    Value *zextRes13 = builder.CreateZExt(icmpRes8, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes14, addRes12, zextRes13);
    Value *truncRes9 = builder.CreateTrunc(phiNode6, Type::getInt32Ty(context), "");
    Value *addRes15  = builder.CreateAdd(truncRes9, GET_INT32(SGL_WIDTH_DEFAULT + 1, false), "");
    Value *uremRes7  = builder.CreateURem(addRes15, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes14 = builder.CreateZExt(uremRes7, Type::getInt64Ty(context), "");

    Value *gepInst8  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes4, zextRes14);
    PUT_LOAD_32(loadRes5, gepInst8);

    Value *icmpRes9   = builder.CreateICmpUGT(loadRes5, GET_INT32(16777215, false), "");
    Value *zextRes15  = builder.CreateZExt(icmpRes9, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes16, addRes14, zextRes15);
    Value *truncRes10 = builder.CreateTrunc(phiNode6, Type::getInt32Ty(context), "");
    Value *addRes17   = builder.CreateAdd(truncRes10, GET_INT32(SGL_WIDTH_DEFAULT - 1, false), "");
    Value *uremRes8   = builder.CreateURem(addRes17, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes16  = builder.CreateZExt(uremRes8, Type::getInt64Ty(context), "");

    Value *gepInst9   = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes5, zextRes16);
    PUT_LOAD_32(loadRes6, gepInst9);

    Value *icmpRes10  = builder.CreateICmpUGT(loadRes6, GET_INT32(16777215, false), "");
    Value *zextRes17  = builder.CreateZExt(icmpRes10, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes18, addRes16, zextRes17);
    Value *truncRes11 = builder.CreateTrunc(addRes8, Type::getInt32Ty(context), "");
    Value *uremRes9   = builder.CreateURem(truncRes11, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes18  = builder.CreateZExt(uremRes9, Type::getInt64Ty(context), "");

    Value *gepInst10  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes5, zextRes18);
    PUT_LOAD_32(loadRes7, gepInst10);

    Value *icmpRes11  = builder.CreateICmpUGT(loadRes7, GET_INT32(16777215, false), "");
    Value *zextRes19  = builder.CreateZExt(icmpRes11, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes19, addRes18, zextRes19);
    Value *truncRes12 = builder.CreateTrunc(phiNode6, Type::getInt32Ty(context), "");
    Value *addRes20   = builder.CreateAdd(truncRes12, GET_INT32(SGL_WIDTH_DEFAULT + 1, false), "");
    Value *uremRes10  = builder.CreateURem(addRes20, GET_INT32(SGL_WIDTH_DEFAULT, false), "");
    Value *zextRes20  = builder.CreateZExt(uremRes10, Type::getInt64Ty(context), "");

    Value *gepInst11  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, zextRes5, zextRes20);
    PUT_LOAD_32(loadRes8, gepInst11);

    Value *icmpRes12  = builder.CreateICmpUGT(loadRes8, GET_INT32(16777215, false), "");
    Value *zextRes21  = builder.CreateZExt(icmpRes12, Type::getInt32Ty(context), "");
    PUT_ADD_NUW_NSW(addRes21, addRes19, zextRes21);

    Value *gepInst12  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes1, phiNode5, phiNode6);
    PUT_LOAD_32(loadRes9, gepInst12);

    Value *andRes2    = builder.CreateAnd(loadRes9, GET_INT32(-16777216, true), "");
    Value *icmpRes13  = builder.CreateICmpNE(andRes2, GET_INT32(0, true), "");
    Value *addRes22   = builder.CreateAdd(addRes21, GET_INT32(-4, true), "");
    cast<BinaryOperator>(addRes22)->setHasNoSignedWrap(true);

    Value *icmpRes14  = builder.CreateICmpULT(addRes22, GET_INT32(-2, true), "");
    Value *selectRes1 = builder.CreateSelect(icmpRes13, icmpRes14, builder.getFalse(), "");

    builder.CreateCondBr(selectRes1, BB[11], BB[12]);

// gameoflife_start: BB11

    builder.SetInsertPoint(BB[11]);

    Value *gepInst13  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes2, phiNode5, phiNode6);
    builder.CreateStore(GET_INT32(0, true), gepInst13)->setAlignment(Align(4));

    builder.CreateBr(BB[15]);

// gameoflife_start: BB12

    builder.SetInsertPoint(BB[12]);

    Value *icmpRes15 = builder.CreateICmpEQ(andRes2, GET_INT32(0, true), "");
    Value *icmpRes16 = builder.CreateICmpEQ(addRes21, GET_INT32(3, true), "");
    Value *selectRes2 = builder.CreateSelect(icmpRes15, icmpRes16, builder.getFalse(), "");

    Value *gepInst14  = GEP_ELEM(outerArrayType, allocaInst, GET_INT64(0, false), zextRes2, phiNode5, phiNode6);
    builder.CreateCondBr(selectRes2, BB[13], BB[14]);

// gameoflife_start: BB13

    builder.SetInsertPoint(BB[13]);

    builder.CreateStore(GET_INT32(-16733645, true), gepInst14)->setAlignment(Align(4));
    builder.CreateBr(BB[15]);

// gameoflife_start: BB14

    builder.SetInsertPoint(BB[14]);

    builder.CreateStore(loadRes9, gepInst14)->setAlignment(Align(4));
    builder.CreateBr(BB[15]);

// gameoflife_start: BB15

    builder.SetInsertPoint(BB[15]);

    PUT_ADD_NUW_NSW(addRes23, phiNode6, GET_INT64(1, false));
    Value *icmpRes17 = builder.CreateICmpEQ(addRes23, GET_INT64(SGL_WIDTH_DEFAULT, false), "");
    builder.CreateCondBr(icmpRes17, BB[9], BB[10]);

    phiNode6->addIncoming(addRes23, BB[15]);

// End

    // Dump LLVM IR
    module->print(outs(), nullptr);

    // Interpreter of LLVM IR
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
        if (fnName == "sgl_rand")
            return reinterpret_cast<void *>(sgl_rand);
        else if (fnName == "sgl_srand")
            return reinterpret_cast<void *>(sgl_srand);
        else if (fnName == "sgl_update")
            return reinterpret_cast<void *>(sgl_update);

        return nullptr;
    });

    ee->finalizeObject();

    sgl_initialize();

    ArrayRef<GenericValue> noargs;
    GenericValue v = ee->runFunction(gameOfLifeStartFunc, noargs);

    sgl_close();

    return EXIT_SUCCESS;
}
