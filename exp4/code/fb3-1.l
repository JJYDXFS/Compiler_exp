%option noyywrap nodefault yylineno
%{
#include "fb3-1.h"
#include "fb3-1.tab.h"
%}

/* 浮点数指数 */
EXP ([Ee][-+]?[0-9]+)

%%
"+" |
"-" |
"*" |
"/" |
"%" |
"=" |
"|" |
"," |
";" |
"{" |
"}" |
"[" |
"]" |
"(" |
")" {return yytext[0];}

">" { yylval.fn = 1; return CMP; }
"<" { yylval.fn = 2; return CMP; }
"!=" { yylval.fn = 3; return CMP; }
"==" { yylval.fn = 4; return CMP; }
">=" { yylval.fn = 5; return CMP; }
"<=" { yylval.fn = 6; return CMP; }

"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"def" { return DEF; }

"sqrt" { yylval.fn = _sqrt; return FUNC; }
"exp" { yylval.fn = _exp; return FUNC; }
"log" { yylval.fn = _log; return FUNC; }
"sin" { yylval.fn = _sin; return FUNC; }
"cos" { yylval.fn = _cos; return FUNC; }
"rand"	{ yylval.fn = _rand; return FUNC; }
"pow" { yylval.fn = _pow; return FUNC; }
"print" { yylval.fn = _print; return FUNC; }

"PI"	{ yylval.n = 3.14159265 ; return NUMBER; }
"e"	{ yylval.n = 2.71828183 ; return NUMBER; }

[a-zA-Z][a-zA-Z0-9]* { yylval.s = lookup(yytext); return NAME; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.n = atof(yytext);return NUMBER; }

"//"[^\\\n]*
[ \t] { /* 空白字符和注释不读 */ }

\\\n { printf("..."); } /* 忽略续行符 */
\n { return EOL; }
. { yyerror("Mystery character %c\n", *yytext); }
%%
