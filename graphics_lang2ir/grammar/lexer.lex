%option c++

%option noyywrap debug yylineno

%s COMMENT_ML

%{

#include "parser.h"

%}

DIGIT                [0-9]

LETTER               [a-zA-Z]
LETTER_D             [A-Za-z0-9_]

INT                  [1-9]{DIGIT}*|0
NAME                 {LETTER}{LETTER_D}*

WS                   [ \t\n\v\r]+
COMMENT_SL           "//".*"\n"

%%

{WS}                 /* skip whitespace symbols */
{COMMENT_SL}         /* skip single-line comments */

<INITIAL>"/*"        { BEGIN(COMMENT_ML); }
<COMMENT_ML>"*/"     { BEGIN(INITIAL); }

<COMMENT_ML>[^*\n]+  /* skip line in multi-line comments */
<COMMENT_ML>"*"      /* skip * in multi-line comments */

"+"                  return yy::parser::token_type::ADD;
"-"                  return yy::parser::token_type::SUB;
"*"                  return yy::parser::token_type::MUL;
"/"                  return yy::parser::token_type::DIV;
"%"                  return yy::parser::token_type::MOD;
"or"                 return yy::parser::token_type::OR;
"and"                return yy::parser::token_type::AND;
"xor"                return yy::parser::token_type::XOR;

">"                  return yy::parser::token_type::GT;
"<"                  return yy::parser::token_type::LT;
">="                 return yy::parser::token_type::GTE;
"<="                 return yy::parser::token_type::LTE;
"=="                 return yy::parser::token_type::EQ;
"!="                 return yy::parser::token_type::NEQ;

":="                 return yy::parser::token_type::ASSIGN;
","                  return yy::parser::token_type::COMMA;
":"                  return yy::parser::token_type::COLON;
";"                  return yy::parser::token_type::SEMICOLON;

"("                  return yy::parser::token_type::LP;
")"                  return yy::parser::token_type::RP;
"{"                  return yy::parser::token_type::LB;
"}"                  return yy::parser::token_type::RB;
"["                  return yy::parser::token_type::LBT;
"]"                  return yy::parser::token_type::RBT;

{INT}                return yy::parser::token_type::INT;
{NAME}               return yy::parser::token_type::NAME;

"if"                 return yy::parser::token_type::IF;
"then"               return yy::parser::token_type::THEN;
"else"               return yy::parser::token_type::ELSE;
"while"              return yy::parser::token_type::WHILE;
"routine"            return yy::parser::token_type::ROUTINE;
"var"                return yy::parser::token_type::VAR;
"is"                 return yy::parser::token_type::IS;
"loop"               return yy::parser::token_type::LOOP;
"end"                return yy::parser::token_type::END;
"return"             return yy::parser::token_type::RETURN;

"array"              return yy::parser::token_type::ARRAY;
"integer"            return yy::parser::token_type::INTEGER;

"?"                  return yy::parser::token_type::SCAN;
"print"              return yy::parser::token_type::PRINT;

.                    return yy::parser::token_type::YYerror;

%%
