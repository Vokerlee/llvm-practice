#ifndef LLVM_IR_GEN_MACROS_H
#define LLVM_IR_GEN_MACROS_H

#define GET_INT64(val, is_signed) \
    ConstantInt::get(Type::getInt64Ty(context), val, is_signed)

#define GET_INT32(val, is_signed) \
    ConstantInt::get(Type::getInt32Ty(context), val, is_signed)

#define PUT_ADD_NUW_NSW(res, val1, val2)                     \
    Value *res = builder.CreateAdd(val1, val2, "");          \
    cast<BinaryOperator>(res)->setHasNoUnsignedWrap(true);   \
    cast<BinaryOperator>(res)->setHasNoSignedWrap(true);

#define PUT_SUB_NUW_NSW(res, val1, val2)                     \
    Value *res = builder.CreateSub(val1, val2, "");          \
    cast<BinaryOperator>(res)->setHasNoUnsignedWrap(true);   \
    cast<BinaryOperator>(res)->setHasNoSignedWrap(true);

#define GEP_ROW(type, ptr, val1, val2)            \
    builder.CreateInBoundsGEP(type, ptr,          \
        ArrayRef<Value *>({val1, val2}), "")

#define GEP_ELEM(type, ptr, val1, val2, val3, val4)       \
    builder.CreateInBoundsGEP(type, ptr,                  \
        ArrayRef<Value *>({val1, val2, val3, val4}), "")

#define PUT_LOAD_32(res, ptr)                                         \
    Value *res = builder.CreateLoad(Type::getInt32Ty(context), ptr);  \
    LoadInst* res##_inst = cast<LoadInst>(res);                       \
    res##_inst->setAlignment(Align(4));

#define ZEXT_32_64()

#endif // LLVM_IR_GEN_MACROS_H
