#ifndef GRLANG_GRAMMAR_DRIVER_DRIVER_H
#define GRLANG_GRAMMAR_DRIVER_DRIVER_H

#include "llvmgen/ast/ast.h"
#include "stdlib/stdlib.h"
#include "parser.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <sstream>
#include <string_view>
#include <vector>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

namespace yy
{

class Driver
{
public:
    Driver(std::istream &in, std::ostream &out) :
        lexer_(std::make_unique<yyFlexLexer>(in, out)),
        global_scope_(std::make_shared<grlang::llvmgen::FuncScopeNode>()),
        scope_cur_(global_scope_)
    {}

    ~Driver() = default;

    parser::token_type yylex(parser::semantic_type *yylval) const
    {
        auto token = static_cast<parser::token_type>(lexer_->yylex());

        switch (token) {
            case parser::token_type::INT:
                yylval->as<int>() = std::atoi(lexer_->YYText());
                break;

            case parser::token_type::NAME:
                yylval->emplace<std::string>(lexer_->YYText());
                break;

            default:
                break;
        }

        return token;
    }

    [[nodiscard]] bool Parse()
    {
        parser par(this);
        return !par.parse();
    }

    void LLVMGen()
    {
        auto void_int_type  = llvm::FunctionType::get(ctx_.GetBuilder()->getVoidTy(), {ctx_.GetIntTy()}, false);
        auto int_void_type  = llvm::FunctionType::get(ctx_.GetIntTy(), false);
        auto void_2int_type = llvm::FunctionType::get(ctx_.GetBuilder()->getVoidTy(), {ctx_.GetIntTy(), ctx_.GetIntTy()}, false);
        auto void_void_type = llvm::FunctionType::get(ctx_.GetBuilder()->getVoidTy(), false);
        auto void_ptr_type  = llvm::FunctionType::get(ctx_.GetBuilder()->getVoidTy(), {ctx_.GetBuilder()->getPtrTy()}, false);

        llvm::Function::Create(void_int_type,  llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_PRINT_FUNC_NAME,      *ctx_.GetModule());
        llvm::Function::Create(int_void_type,  llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_SCAN_FUNC_NAME,       *ctx_.GetModule());

        llvm::Function::Create(void_2int_type, llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_SGL_INIT_FUNC_NAME,   *ctx_.GetModule());
        llvm::Function::Create(void_void_type, llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_SGL_CLOSE_FUNC_NAME,  *ctx_.GetModule());
        llvm::Function::Create(int_void_type,  llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_SGL_RAND_FUNC_NAME,   *ctx_.GetModule());
        llvm::Function::Create(void_int_type,  llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_SGL_SRAND_FUNC_NAME,  *ctx_.GetModule());
        llvm::Function::Create(void_ptr_type,  llvm::Function::ExternalLinkage,
                               grlang::llvmgen::STDLIB_SGL_UPDATE_FUNC_NAME, *ctx_.GetModule());

        global_scope_->LLVMGen(ctx_);
    }

    void DumpIR(std::ostream &out)
    {
        std::string buffer;
        llvm::raw_string_ostream os(buffer);
        ctx_.GetModule()->print(os, nullptr);
        os.flush();
        out << buffer;
    }

private:
    void InheritGlobalScope()
    {
        scope_cur_ = std::make_shared<grlang::llvmgen::FuncScopeNode>(global_scope_);
    }

    void InheritCurScope()
    {
        auto parent = scope_cur_;
        scope_cur_ = std::make_shared<grlang::llvmgen::FuncScopeNode>(parent);
        parent->PushScope(scope_cur_);
    }

    void ResetScope()
    {
        auto parent = scope_cur_->GetParent();
        scope_cur_ = parent == nullptr ? global_scope_ : parent;
    }

private:
    std::unique_ptr<yyFlexLexer>               lexer_           {};
    grlang::llvmgen::Context                   ctx_             {};

    grlang::llvmgen::FuncScopePtr              global_scope_    {};
    grlang::llvmgen::FuncScopePtr              scope_cur_       {};
    grlang::llvmgen::FuncProtPtr               func_cur_        {};

    std::vector<grlang::llvmgen::FuncParamPtr> func_params_cur_ {};
    std::vector<grlang::llvmgen::NodePtr>      args_cur_        {};

    friend parser;
};

} // namespace yy

#endif // GRLANG_GRAMMAR_DRIVER_DRIVER_H
