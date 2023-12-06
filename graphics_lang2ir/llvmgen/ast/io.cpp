#include "llvmgen/ast/io.h"

#include <cstdlib>
#include <iostream>

extern "C" void __print_(grlang::Int value)
{
    std::cout << value << std::endl;
}

extern "C" grlang::Int __scan_()
{
    grlang::Int res;

    std::cin >> res;
    if (!std::cin) {
        std::cerr << "Problem while reading stdin in scan(). Exiting with failure...\n";
        std::exit(EXIT_FAILURE);
    }

    return res;
}

namespace grlang
{

namespace llvmgen
{

llvm::Value *PrintNode::LLVMGen(Context &ctx)
{
    auto *value = expr_->LLVMGen(ctx);

    auto print_func = ctx.GetModule()->getFunction(IO_PRINT_FUNC_NAME);
    if (print_func == nullptr)
        throw std::runtime_error{"Cannot find print() function"};

  return ctx.GetBuilder()->CreateCall(print_func, {value});
}

llvm::Value *ScanNode::LLVMGen(Context &ctx)
{
    auto scan_func = ctx.GetModule()->getFunction(IO_SCAN_FUNC_NAME);
    if (scan_func == nullptr)
        throw std::runtime_error{"Cannot find scan() function"};

    return ctx.GetBuilder()->CreateCall(scan_func);
}

} // namespace llvmgen
} // namespace grlang
