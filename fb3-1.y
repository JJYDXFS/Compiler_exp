/* 基于AST的计算器 */
/*
 * @author JJYDXFS
 * @version NOV 22 2020
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "fb3-1.h"
%}

%union {
	struct tree *t;
	double n;
	struct symbol *s; /* 指定符号 */
	struct symlist *sl;
	int fn;	/* 指定函数 */
}

/* token */
%token <n> NUMBER
%token <s> NAME
%token <fn> FUNC
%token EOL
%token WHILE IF DEF

%nonassoc IFX
%nonassoc ELSE

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/' '%'
%nonassoc '|' UMINUS

%type <t> exp stmt list explist vector
%type <sl> symlist

%start calclist
%%
calclist:	/* 空 */
    | calclist stmt EOL {
	if($2!=NULL){
		printf("return = %.10g\n", eval($2));
		freeTree($2);
	}
	printf("$ ");
	}
    | calclist DEF NAME '(' symlist ')' '=' list EOL {
			def($3, $5, $8);
			printf("Defined %s\n$ ",$3->name); }
    | calclist vector ';' EOL {
	// 修改输出
	if ($2 == NULL) printf("[]\n$ ");
	else{
		printf("[");
		struct tree *p = $2;
		while(p->r->nodeType != 'E'){
			printf("%.10g,", eval(p->l));
			p = p->r;
		}
		printf("%.10g", eval(p->l));
		printf("]\n$ ");
	}
       }
    | calclist EOL { printf("$ "); }
    | calclist error EOL { yyerrok; printf("$ "); }
;

stmt: 
      ';'	{ $$ = NULL; }
    | IF '(' exp ')' stmt %prec IFX { $$ = nflow('I', $3, $5, NULL); }
    | IF '(' exp ')' stmt ELSE stmt { $$ = nflow('I', $3, $5, $7); }
    | WHILE '(' exp ')' stmt { $$ = nflow('W', $3, $5, NULL); }
    | exp ';'	{ $$ = $1; }
    | '{' list '}'	{ $$ = $2; }
;

list:
     stmt	{ $$ = $1 }
   | list stmt	{ if ($2 == NULL)
			$$ = $1;
		  else
			$$ = ntree('L', $1, $2);
		}
;

exp: exp CMP exp { $$ = ncmp($2, $1, $3); }
   | exp '+' exp { $$ = ntree('+', $1, $3); }
   | exp '-' exp { $$ = ntree('-', $1, $3); }
   | exp '*' exp { $$ = ntree('*', $1, $3); }
   | exp '/' exp { $$ = ntree('/', $1, $3); }
   | exp '%' exp { $$ = ntree('%', $1, $3); }
   | '|' exp { $$ = ntree('|', $2, NULL); }
   | '(' exp ')' { $$ = $2; }
   | '-' exp %prec UMINUS { $$ = ntree('M', $2, NULL); }
   | NUMBER { $$ = nnum($1); }
   | NAME { $$ = nref($1); }
   | NAME '=' exp { $$ = nasgn($1, $3); }
   | FUNC '(' explist ')' { $$ = nfunc($1, $3); }
   | NAME '(' explist ')' { $$ = ncall($1, $3); }
;

/* 向量运算，只支持两个向量间的运算，不支持级联运算 */
vector: '[' explist ']'	{ $$ = $2; }
   | vector '+' vector { $$ = veval('+', $1, $3); }
   | vector '-' vector { $$ = veval('-', $1, $3); }
   | vector '*' vector { $$ = veval('*', $1, $3); }
   | vector '/' vector { $$ = veval('/', $1, $3); }
;

explist: exp
   | exp ',' explist { $$ = ntree('L', $1, $3); }
;

symlist: NAME { $$ = nsymlist($1, NULL); }
   | NAME ',' symlist { $$ = nsymlist($1, $3); }
;
%%
