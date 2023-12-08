%language "c++"
%skeleton "lalr1.cc"

%locations /* actually not necessary here (not used) */

%define parse.trace
%define parse.lac full
%define parse.error detailed
%define api.value.type variant

%param {Driver *driver}

%code requires {
    #include "llvmgen/ast/ast.h"

    #include <string>
    #include <memory>

    namespace yy
    {
        class Driver;
    }
}

%code {
    #include "grammar/driver/driver.h"

    namespace yy
    {
        parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *yylloc, Driver *driver);
    }
}

%token <std::string> NAME
%token <int> INT
%token ADD     SUB    MUL       DIV      MOD       OR         AND
       XOR     GT     LT        GTE      LTE       EQ         NEQ
       COMMA   COLON  SEMICOLON LP       RP        LB         RB
       LBT     RBT    ASSIGN    IF       THEN      ELSE       WHILE
       ROUTINE VAR    IS        LOOP     END       RETURN     ARRAY
       INTEGER SCAN   PRINT     SGL_RAND SGL_SRAND SGL_UPDATE SGL_INIT
       SGL_CLOSE

%nterm<grlang::llvmgen::DeclPtr>  RoutineDecl;
%nterm<std::string>               RoutineName;
%nterm<grlang::llvmgen::NodePtr>  Program     Parameters     ParamDecl   Statement      ReturnStatement
                                  Assignment  RoutineCall    Arguments   WhileStatement IfStatement
                                  Expression  ExpressionCMP  ExpressionT ExpressionP    Primary;

%nterm <grlang::llvmgen::VarPtr>         VarDecl GlobalVarDecl;
%nterm <grlang::llvmgen::StoreablePtr>   ModPrimary;
%nterm <grlang::llvmgen::FuncScopePtr>   TrueScope FalseScope;
%nterm <llvm::Type *>                    Type PrimitiveType ArrayType;

%right ASSIGN

%right ELSE THEN

%left EQ NEQ GT LT GTE LTE
%left ADD SUB MUL DIV MOD OR AND XOR

%%

/* Start */

Parse               : Program                                           { YYACCEPT; }

Program             : RoutineDecl                                       { driver->global_scope_->AddDecl($1, true); }
                    | GlobalVarDecl                                     { driver->global_scope_->AddDecl($1, true); }
                    | Program GlobalVarDecl                             { driver->global_scope_->AddDecl($2, true); }
                    | Program RoutineDecl                               { driver->global_scope_->AddDecl($2, true); }

/* Global scope description */

GlobalVarDecl       : VarDecl                                           { $$ = std::make_shared<grlang::llvmgen::GlobVarNode>($1); }

RoutineDecl         : RoutineHeader                                     {
                                                                            driver->scope_cur_->SetParentFuncDecl(driver->func_cur_);
                                                                            driver->func_params_cur_.clear();
                                                                        }
                      FuncBody                                          {
                                                                            $$ = driver->scope_cur_->GetParentFuncDecl();
                                                                        }

RoutineHeader       : RoutineName                                       {
                                                                            driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>(
                                                                                $1, driver->func_params_cur_, driver->scope_cur_);
                                                                        }
                    | RoutineName COLON PrimitiveType                   {
                                                                            driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>(
                                                                                $1, driver->func_params_cur_, driver->scope_cur_, $3);
                                                                        }
                    | RoutineName LP Parameters RP                      {
                                                                            driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>(
                                                                                $1, driver->func_params_cur_, driver->scope_cur_);
                                                                        }
                    | RoutineName LP Parameters RP COLON PrimitiveType  {
                                                                            driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>(
                                                                                $1, driver->func_params_cur_, driver->scope_cur_, $6);
                                                                        }

FuncBody            : IS Body END                                       {}
                    | SEMICOLON                                         { driver->func_cur_->MarkAsDecl(); }

RoutineName         : ROUTINE NAME                                      {
                                                                            driver->InheritGlobalScope();
                                                                            $$ = $2;
                                                                        }

Parameters          : ParamDecl                                         {}
                    | Parameters COMMA ParamDecl                        {}

ParamDecl           : NAME COLON Type                                   {
                                                                            driver->func_params_cur_.push_back(
                                                                                std::make_shared<grlang::llvmgen::FuncParamNode>($3, $1));
                                                                        }

/* Body scope description */

Body                : Statement                                         { driver->scope_cur_->PushNode($1); }
                    | VarDecl                                           { driver->scope_cur_->AddDecl($1, true); }
                    | Body Statement                                    { driver->scope_cur_->PushNode($2); }
                    | Body VarDecl                                      { driver->scope_cur_->AddDecl($2, true); }

Statement           : Assignment                     SEMICOLON          { $$ = $1; }
                    | RoutineCall                    SEMICOLON          { $$ = $1; }
                    | PRINT      Expression          SEMICOLON          { $$ = std::make_shared<grlang::llvmgen::PrintNode>($2); }
                    | SGL_SRAND  Expression          SEMICOLON          { $$ = std::make_shared<grlang::llvmgen::SGLSrandNode>($2); }
                    | SGL_INIT   LP Expression COMMA
                                    Expression RP    SEMICOLON          { $$ = std::make_shared<grlang::llvmgen::SGLInitNode>($3, $5); }
                    | SGL_CLOSE  LP RP               SEMICOLON          { $$ = std::make_shared<grlang::llvmgen::SGLCloseNode>(); }
                    | SGL_UPDATE LP ModPrimary RP    SEMICOLON          { $$ = std::make_shared<grlang::llvmgen::SGLUpdateNode>($3); }
                    | WhileStatement                                    { $$ = $1; }
                    | IfStatement                                       { $$ = $1; }
                    | ReturnStatement                SEMICOLON          { $$ = $1; }

Assignment          : ModPrimary ASSIGN Expression                      { $$ = std::make_shared<grlang::llvmgen::AssignNode>($1, $3); }

RoutineCall         : NAME LP RP                                        {
                                                                            $$ = std::make_shared<grlang::llvmgen::FuncCallNode>(
                                                                                driver->scope_cur_->FindName($1));
                                                                        }
                    | NAME LP Arguments RP                              {
                                                                            $$ = std::make_shared<grlang::llvmgen::FuncCallNode>(
                                                                                driver->scope_cur_->FindName($1), driver->args_cur_);
                                                                            driver->args_cur_.clear();
                                                                        }

ReturnStatement     : RETURN                                            { $$ = std::make_shared<grlang::llvmgen::FuncRetNode>(); }
                    | RETURN Expression                                 { $$ = std::make_shared<grlang::llvmgen::FuncRetNode>($2); }

VarDecl             : VAR NAME COLON Type IS Expression SEMICOLON       { $$ = std::make_shared<grlang::llvmgen::VarNode>($4, $2, $6); }
                    | VAR NAME COLON Type SEMICOLON                     { $$ = std::make_shared<grlang::llvmgen::VarNode>($4, $2); }

Arguments           : Expression                                        { driver->args_cur_.push_back($1); }
                    | Arguments COMMA Expression                        { driver->args_cur_.push_back($3); }

/* Control flow description */

WhileStatement      : WHILE Expression LOOP                             { driver->InheritCurScope(); }
                      Body END                                          {
                                                                            $$ = std::make_shared<grlang::llvmgen::WhileNode>(
                                                                                driver->scope_cur_->GetParent(), $2, driver->scope_cur_);
                                                                            driver->ResetScope();
                                                                        }

IfStatement         : IF Expression TrueScope END                       { $$ = std::make_shared<grlang::llvmgen::IfNode>(driver->scope_cur_, $2, $3); }
                    | IF Expression TrueScope FalseScope                { $$ = std::make_shared<grlang::llvmgen::IfNode>(driver->scope_cur_, $2, $3, $4); }

TrueScope           : THEN                                              { driver->InheritCurScope(); }
                      Body                                              { $$ = driver->scope_cur_; driver->ResetScope(); }

FalseScope          : ELSE                                              { driver->InheritCurScope(); }
                      Body END                                          { $$ = driver->scope_cur_; driver->ResetScope(); }

/* Types description */

Type                : PrimitiveType                                     { $$ = $1; }
                    | ArrayType                                         { $$ = $1; }

PrimitiveType       : INTEGER                                           { $$ = driver->ctx_.GetIntTy(); }

ArrayType           : ARRAY LBT INT RBT PrimitiveType                   { $$ = grlang::llvmgen::Context::GetArrTy($5, $3); }

/* Expression description */

Expression          : ExpressionCMP ADD Expression                      { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::ADD, $3); }
                    | ExpressionCMP SUB Expression                      { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::SUB, $3); }
                    | ExpressionCMP                                     { $$ = $1; }

ExpressionCMP       : ExpressionT EQ  ExpressionCMP                     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::EQ,  $3); }
                    | ExpressionT NEQ ExpressionCMP                     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::NEQ, $3); }
                    | ExpressionT GT  ExpressionCMP                     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::GT,  $3); }
                    | ExpressionT LT  ExpressionCMP                     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::LT,  $3); }
                    | ExpressionT LTE ExpressionCMP                     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::LTE, $3); }
                    | ExpressionT GTE ExpressionCMP                     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::GTE, $3); }
                    | ExpressionT                                       { $$ = $1; }

ExpressionT         : ExpressionP OR  ExpressionT                       { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::OR,  $3); }
                    | ExpressionP AND ExpressionT                       { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::AND, $3); }
                    | ExpressionP XOR ExpressionT                       { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::XOR, $3); }
                    | ExpressionP MUL ExpressionT                       { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::MUL, $3); }
                    | ExpressionP DIV ExpressionT                       { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::DIV, $3); }
                    | ExpressionP MOD ExpressionT                       { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::MOD, $3); }
                    | ExpressionP                                       { $$ = $1; }

ExpressionP         : LP Expression RP                                  { $$ = $2; }
                    | Primary                                           { $$ = $1; }

Primary             : INT                                               { $$ = std::make_shared<grlang::llvmgen::IntNode>($1); }
                    | ModPrimary                                        { $$ = $1; }
                    | SCAN                                              { $$ = std::make_shared<grlang::llvmgen::ScanNode>(); }
                    | SGL_RAND LP RP                                    { $$ = std::make_shared<grlang::llvmgen::SGLRandNode>(); }
                    | RoutineCall                                       { $$ = $1; }

ModPrimary          : NAME                                              {
                                                                            $$ = std::dynamic_pointer_cast<grlang::llvmgen::StoreableNode>(
                                                                                driver->scope_cur_->FindName($1));
                                                                        }
                    | NAME LBT Expression RBT                           {
                                                                            auto decl = std::dynamic_pointer_cast<grlang::llvmgen::VarNode>(
                                                                                driver->scope_cur_->FindName($1));
                                                                            $$ = std::make_shared<grlang::llvmgen::ArrayAccessNode>($1, decl, $3);
                                                                        }
%%

namespace yy
{

parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *yylloc, Driver *driver)
{
    (void) yylloc;
    return driver->yylex(yylval);
}

void parser::error(const parser::location_type &loc, const std::string &msg)
{
    std::cout << loc << ':' << msg << std::endl;
}

}
