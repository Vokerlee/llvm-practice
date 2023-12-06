#ifndef GRLANG_GRAMMAR_DRIVER_DRIVER_H
#define GRLANG_GRAMMAR_DRIVER_DRIVER_H

#include "llvmgen/ast/ast.h"
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

    void CodeGen()
    {
        auto print_func_type = llvm::FunctionType::get(ctx_.GetBuilder()->getVoidTy(), {ctx_.getIntTy()}, false);
        auto scan_func_type  = llvm::FunctionType::get(ctx_.getIntTy(), false);

        llvm::Function::Create(print_func_type, llvm::Function::ExternalLinkage,
                               grlang::llvmgen::IO_PRINT_FUNC_NAME, *ctx_.GetModule());
        llvm::Function::Create(scan_func_type,  llvm::Function::ExternalLinkage,
                               grlang::llvmgen::IO_SCAN_FUNC_NAME,  *ctx_.GetModule());

        global_scope_->CodeGen(ctx_);
    }

    void DumpIR(std::ostream &out) const
    {
        std::string buffer;
        llvm::raw_string_ostream os(buffer);
        ctx_.GetModule()->print(os, nullptr);
        os.flush();
        out << buf;
    }

private:
    void MakeGlobalScopeChild()
    {
        scope_cur_ = std::make_shared<grlang::llvmgen::FuncScopeNode>(global_scope_);
    }

    void MakeCurScopeChild()
    {
        auto par = scope_cur_;
        scope_cur_ = std::make_shared<grlang::llvmgen::FuncScopeNode>(par);
        par->PushScope(scope_cur_);
    }

    void ResetScope()
    {
        auto par = scope_cur_->GetParent();
        scope_cur_ = par == nullptr ? global_scope_ : par;
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
