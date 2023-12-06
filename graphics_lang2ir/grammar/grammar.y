%language "c++"
%skeleton "lalr1.cc"

%locations

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

    namespace yy {
        parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *yylloc, Driver *driver);
    }
}


%token <std::string> NAME
%token <int> INT
%token ADD SUB MUL DIV MOD OR AND XOR GT LT GTE LTE EQ NEQ
       COMMA COLON SEMICOLON LP RP LB RB LBT RBT ASSIGN
       IF THEN ELSE WHILE ROUTINE VAR IS LOOP END RETURN ARRAY INTEGER
       SCAN PRINT

%nterm<grlang::llvmgen::DeclPtr>  RoutineDeclaration;
%nterm<std::string>               RoutineName;
%nterm<grlang::llvmgen::NodePtr>  Program     Parameters ParamDecl Statement   ReturnStatement Assignment
                                  RoutineCall Arguments  WhileLoop IfStatement Expression      ExpressionTerm Primary;

%nterm <std::shared_ptr<grlang::llvmgen::VarNode>> VariableDeclaration GVarDecl;
%nterm <grlang::llvmgen::StoreablePtr> ModPrimary;
%nterm <grlang::llvmgen::FuncScopePtr> TrueScope FalseScope;
%nterm <llvm::Type *> Type PrimitiveType ArrayType;

%right ASSIGN

%right ELSE THEN

%left EQ NEQ GT LT GTE LTE
%left ADD SUB MUL DIV MOD OR AND XOR

%%

Parse               : Program                                    { YYACCEPT; }

Program             : RoutineDeclaration                         { driver->global_scope_->AddDecl($1, true); }
                    | GVarDecl                                   { driver->global_scope_->AddDecl($1, true); }
                    | Program GVarDecl                           { driver->global_scope_->AddDecl($2, true); }
                    | Program RoutineDeclaration                 { driver->global_scope_->AddDecl($2, true); }

GVarDecl            : VariableDeclaration                        { $$ = std::make_shared<grlang::llvmgen::GlobVarNode>($1); }

VariableDeclaration : VAR NAME COLON Type IS Expression SEMICOLON   { $$ = std::make_shared<grlang::llvmgen::VarNode>($4, $2, $6); }
                    | VAR NAME COLON Type SEMICOLON                 { $$ = std::make_shared<grlang::llvmgen::VarNode>($4, $2); }

RoutineHeader       : RoutineName                                      { driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>($1, driver->func_params_cur_, driver->scope_cur_); }
                    | RoutineName COLON PrimitiveType                  { driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>($1, driver->func_params_cur_, driver->scope_cur_, $3); }
                    | RoutineName LP Parameters RP                     { driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>($1, driver->func_params_cur_, driver->scope_cur_); }
                    | RoutineName LP Parameters RP COLON PrimitiveType
                                                                 { driver->func_cur_ = std::make_shared<grlang::llvmgen::FuncProtNode>($1, driver->func_params_cur_, driver->scope_cur_, $6); }

RoutineDeclaration : RoutineHeader {
                                     driver->scope_cur_->SetParentFuncDecl(driver->func_cur_);
                                     driver->func_params_cur_.clear();
                                   } FuncBody {
                                                $$ = driver->scope_cur_->GetParentFuncDecl();
                                              }

FuncBody : IS Body END {}
         | SEMICOLON      { driver->func_cur_->MarkAsDecl(); }

RoutineName : ROUTINE NAME { driver->MakeGlobalScopeChild(); $$ = $2; }

Parameters : ParamDecl {}
           | Parameters COMMA ParamDecl {}

ParamDecl : NAME COLON Type { driver->func_params_cur_.push_back(std::make_shared<grlang::llvmgen::FuncParamNode>($3, $1)); }

Type : PrimitiveType { $$ = $1; }
     | ArrayType     { $$ = $1; }

PrimitiveType : INTEGER { $$ = driver->ctx_.GetIntTy(); }

ArrayType : ARRAY LBT INT RBT PrimitiveType { $$ = grlang::llvmgen::Context::GetArrTy($5, $3); }

Body : Statement                { driver->scope_cur_->PushNode($1); }
     | VariableDeclaration      { driver->scope_cur_->AddDecl($1, true); }
     | Body Statement           { driver->scope_cur_->PushNode($2); }
     | Body VariableDeclaration { driver->scope_cur_->AddDecl($2, true); }

Statement : Assignment SEMICOLON       { $$ = $1; }
          | RoutineCall SEMICOLON      { $$ = $1; }
          | PRINT Expression SEMICOLON { $$ = std::make_shared<grlang::llvmgen::PrintNode>($2); }
          | WhileLoop               { $$ = $1; }
          | IfStatement             { $$ = $1; }
          | ReturnStatement SEMICOLON  { $$ = $1; }

ReturnStatement : RETURN            { $$ = std::make_shared<grlang::llvmgen::FuncRetNode>(); }
                | RETURN Expression { $$ = std::make_shared<grlang::llvmgen::FuncRetNode>($2); }

Assignment : ModPrimary ASSIGN Expression { $$ = std::make_shared<grlang::llvmgen::AssignNode>($1, $3); }

RoutineCall : NAME LP RP { $$ = std::make_shared<grlang::llvmgen::FuncCallNode>(driver->scope_cur_->FindName($1)); }
            | NAME LP Arguments RP {
                                     $$ = std::make_shared<grlang::llvmgen::FuncCallNode>(driver->scope_cur_->FindName($1), driver->args_cur_);
                                     driver->args_cur_.clear();
                                   }

Arguments : Expression { driver->args_cur_.push_back($1); }
          | Arguments COMMA Expression { driver->args_cur_.push_back($3); }

WhileLoop : WHILE Expression LOOP {driver->MakeCurScopeChild(); } Body END { $$ = std::make_shared<grlang::llvmgen::WhileNode>($2, driver->scope_cur_, driver->scope_cur_->GetParent()); driver->ResetScope(); }

IfStatement : IF Expression TrueScope END { $$ = std::make_shared<grlang::llvmgen::IfNode>($3, $2, driver->scope_cur_); }
            | IF Expression TrueScope FalseScope { $$ = std::make_shared<grlang::llvmgen::IfNode>($3, $2, driver->scope_cur_, $4); }

TrueScope : THEN {driver->MakeCurScopeChild(); } Body { $$ = driver->scope_cur_; driver->ResetScope(); }

FalseScope : ELSE {driver->MakeCurScopeChild(); } Body END {$$ = driver->scope_cur_; driver->ResetScope(); }

Expression : Expression OR Expression      { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::OR,  $3); }
           | Expression AND Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::AND, $3); }
           | Expression XOR Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::XOR, $3); }
           | Expression EQ Expression      { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::EQ,  $3); }
           | Expression NEQ Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::NEQ, $3); }
           | Expression GT Expression      { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::GT,  $3); }
           | Expression LT Expression      { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::LT,  $3); }
           | Expression LTE Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::LTE, $3); }
           | Expression GTE Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::GTE, $3); }
           | Expression ADD Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::ADD, $3); }
           | Expression SUB Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::SUB, $3); }
           | Expression MUL Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::MUL, $3); }
           | Expression DIV Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::DIV, $3); }
           | Expression MOD Expression     { $$ = std::make_shared<grlang::llvmgen::BinOpNode>($1, grlang::llvmgen::BinOp::MOD, $3); }
           | ExpressionTerm                { $$ = $1; }

ExpressionTerm : LP Expression RP { $$ = $2; }
               | Primary { $$ = $1; }

Primary : INT         { $$ = std::make_shared<grlang::llvmgen::IntNode>($1); }
        | ModPrimary  { $$ = $1; }
        | SCAN        { $$ = std::make_shared<grlang::llvmgen::ScanNode>(); }
        | RoutineCall { $$ = $1; }

ModPrimary : NAME { $$ = std::dynamic_pointer_cast<grlang::llvmgen::StoreableNode>(driver->scope_cur_->FindName($1)); }
           | NAME LBT Expression RBT {
                                        auto decl = std::dynamic_pointer_cast<grlang::llvmgen::VarNode>(driver->scope_cur_->FindName($1));
                                        $$ = std::make_shared<grlang::llvmgen::ArrayAccessNode>($1, decl, $3);
                                      }


%%

namespace yy {

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
