/*
 * @author JJYDXFS
 * @version NOV 22 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "fb3-1.h"

/* 符号表 */
/* 哈希一个符号 */
static unsigned symhash(char *sym){
	unsigned int hash = 0;
	unsigned c;
	
	while(c = *sym++) hash = hash*9 ^ c;

	return hash;
}

struct symbol * lookup(char* sym){
	struct symbol *sp = &symtab[symhash(sym)%NHASH];
	int scount = NHASH;	/* 符号表大小 */

	while(--scount >= 0) {
		if(sp->name && !strcmp(sp->name, sym)) { return sp; }

		if(!sp->name){	/* 新条目 */
			sp->name = strdup(sym);
			sp->value = 0;
			sp->func = NULL;
			sp->syms = NULL;
			return sp;
		}

		if(++sp >= symtab+NHASH) sp = symtab; /* 尝试下一条目 */
	}
	yyerror("symbol table overflow\n");
	abort();	// 符号表溢出
}

struct tree * ntree(int nodeType, struct tree *l, struct tree *r)
{
	struct tree *t=malloc(sizeof(struct tree));

	if(!t){
		yyerror("out of space");
		exit(0);
	}
	t->nodeType = nodeType;
	t->l = l;
	t->r = r;
	return t;
}

struct tree * nnum(double n)
{
	struct numval *nv = malloc(sizeof(struct numval));

	if(!nv){
		yyerror("out of space");
		exit(0);
	}
	nv->nodeType = 'C';
	nv->number = n;
	return (struct tree *)nv;
}

struct tree * ncmp(int cmpType, struct tree *l, struct tree *r){
	struct tree *t=malloc(sizeof(struct tree));

	if(!t){
		yyerror("out of space");
		exit(0);
	}
	t->nodeType = '0' +cmpType;
	t->l = l;
	t->r = r;
	return t;
}

struct tree * nfunc(int funcType, struct tree *l){
	struct fncall *a = malloc(sizeof(struct fncall));

	if(!a){
		yyerror("out of space");
		exit(0);
	}
	a->nodeType = 'F';
	a->l = l;
	a->funcType = funcType;
}

struct tree * ncall(struct symbol *s, struct tree *l){
	struct ufncall *a = malloc(sizeof(struct ufncall));

	if(!a){
		yyerror("out of space");
		exit(0);
	}
	a->nodeType = 'U';
	a->l = l;
	a->s = s;
	return (struct tree *)a;
}

struct tree * nref(struct symbol *s){
	struct symref *a = malloc(sizeof(struct symref));

	if(!a){
		yyerror("out of space");
		exit(0);
	}
	a->nodeType = 'N';
	a->s = s;
	return (struct tree *)a;
}

struct tree * nasgn(struct symbol *s, struct tree *v){
	struct symasgn *a = malloc(sizeof(struct symasgn));

	if(!a){
		yyerror("out of space");
		exit(0);
	}
	a->nodeType = '=';
	a->s = s;
	a->v = v;
	return (struct tree *)a;
}

struct tree * nflow(int nodeType, struct tree *cond, struct tree *tl, struct tree *el)
{
	struct flow *a = malloc(sizeof(struct flow));

	if(!a){
		yyerror("out of space");
		exit(0);
	}
	a->nodeType = nodeType;
	a->cond = cond;
	a->tl = tl;
	a->el = el;
	return (struct tree *)a;
}

void freeTree(struct tree*t)
{
	switch(t->nodeType){
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '1': case '2': case '3': case '4': case '5': case '6':
	case 'L':
		freeTree(t->r); // 以上删除右子树
	case '|':
	case 'M': case 'U': case 'F':
		freeTree(t->l);	// 以上删除左子树
	case 'C': case 'N':
		break;

	case '=':	// 赋值没有子树
		free( ((struct symasgn *)t)->v );
		break;
	case 'I': case 'W':	// 控制语句三棵树
		free( ((struct flow *)t)->cond );
		if(((struct flow *)t)->tl) freeTree(((struct flow *)t)->tl);
		if(((struct flow *)t)->el) freeTree(((struct flow *)t)->el);
		break;

	default: printf("internal error: free bad node %c\n", t->nodeType);
	}
	free(t);	// 释放自身
}

struct symlist * nsymlist(struct symbol *sym, struct symlist *next){
	struct symlist *sl = malloc(sizeof(struct symlist));

	if(!sl){
		yyerror("out of space");
		exit(0);
	}
	sl->sym = sym;
	sl->next = next;
	return sl;
}

void freeSymlist(struct symlist *sl){
	struct symlist *nsl;

	while(sl){
		nsl = sl->next;
		free(sl);
		sl = nsl;
	}
}

double eval(struct tree *t)
{
	double v; // 计算结果
	double v1_d, v2_d;	// 为求余运算预留的变量
	int v1, v2;
	
	if(!t){
		yyerror("internal error, null eval");
		return 0.0;
	}

	switch(t->nodeType){
		case 'C': v=((struct numval*)t)->number; break;
		case 'N': v=((struct symref*)t)->s->value; break;
		case '=': v=((struct symasgn*)t)->s->value = 
			  eval(((struct symasgn *)t)->v); break;
		/* 表达式计算 */
		case '+': v=eval(t->l)+eval(t->r); break;
		case '-': v=eval(t->l)-eval(t->r); break;
		case '*': v=eval(t->l)*eval(t->r); break;
		case '/': v=eval(t->l)/eval(t->r); break;
		case '%':
			  v1_d = eval(t->l);
			  v2_d = eval(t->r);
			  v1 = v1_d, v2 = v2_d;
			  if( fabs(v1-v1_d)<1e-6 && fabs(v2-v2_d)<1e-6){
			  	v = v1%v2;
				break;
			  } else {
			  	yyerror("invalid operands of types 'double' and 'double' to binary 'operator%'");
				return 0.0;
			  }

		case '|': 
			  v=eval(t->l); 
			  if(v<0) v=-v;
			  break;
		case 'M': v=-eval(t->l);break;

		/* 比较 */
		case '1': v=(eval(t->l) > eval(t->r))? 1 : 0; break;
		case '2': v=(eval(t->l) < eval(t->r))? 1 : 0; break;
		case '3': v=(eval(t->l) != eval(t->r))? 1 : 0; break;
		case '4': v=(eval(t->l) == eval(t->r))? 1 : 0; break;
		case '5': v=(eval(t->l) >= eval(t->r))? 1 : 0; break;
		case '6': v=(eval(t->l) <= eval(t->r))? 1 : 0; break;

		/* 控制流 */
		/* 包括语法中的空表达式 */

		/* if/else */
		case 'I':
			  if( eval(((struct flow *)t)->cond) != 0 ){
			  	if(((struct flow *)t)->tl){
					v = eval(((struct flow *)t)->tl);
				}
				else v = 0.0; /* 默认值 */
			  } else {
			  	if(((struct flow *)t)->el) {
					v = eval(((struct flow *)t)->el);
				}else v = 0.0; /* 默认值 */
			  }
			  break;
		case 'W':
			  v = 0.0;

			  if(((struct flow *)t)->tl){
			  	while(eval(((struct flow *)t)->cond) != 0)	// 计算条件
					v= eval(((struct flow *)t)->tl);	// 执行语句
			  }
			  break; /* 将最后一条语句的值作为while的值 */

		case 'L': eval(t->l); v = eval(t->r); break;
		case 'F': v = callbuiltin((struct fncall *)t); break;
		case 'U': v = calluser((struct ufncall *)t); break;
		default: printf("internal error: bad node %c\n", t->nodeType);
	}
	return v;
}

/* 向量计算函数 */
struct tree * veval(int opr, struct tree *v1, struct tree *v2){
	struct tree * v=malloc(sizeof(struct tree));	// 结果向量
	struct tree * p, * p1, * p2;	// 活动指针
	double value, value1, value2;	// v1和v2向量中的值

	p = v; p1 = v1; p2 = v2;
	while (p1 || p2) {
		if(!p1 || !p2){
			yyerror("vectors' length do not match");
			v = NULL;
			return v; 
		}

		if(p1->nodeType == 'L'){
			value1 = eval(p1->l);
			p1 = p1->r;
		} else {
			value1 = eval(p1);
			p1 = NULL;
		}

		if(p2->nodeType == 'L'){
			value2 = eval(p2->l);
			p2 = p2->r;
		} else {
			value2 = eval(p2);
			p2 = NULL;
		}

		switch(opr){
			case '+': value=value1+value2; break;
			case '-': value=value1-value2; break;
			case '*': value=value1*value2; break;
			case '/': value=value1/value2; break;
			default: printf("internal error: undefined operator %c\n", opr);return v;
		}
		p->l = nnum(value); 	// 新建节点
		p->r = malloc(sizeof(struct tree));
		p = p->r;
	}
	p->nodeType = 'E';	// 标记向量尾部
	return v;
}

static double callbuiltin(struct fncall *f){	// 内置函数
	enum bifs funcType = f->funcType;
	double v = eval(f->l);

	switch(funcType){
		case _sqrt: return sqrt(v);
		case _exp: return exp(v);
		case _log: return log(v);
		case _sin: return sin(v);
		case _cos: return cos(v);
		case _rand: return v*(1.0*rand()/RAND_MAX);
		case _pow: return pow(eval(f->l->l),eval(f->l->r));
		case _print: 
			   printf("%.10g\n", v);
			   return v;
		default:
			   yyerror("Unkonwn built-in function %d", funcType);
			   return 0.0;
	}
}

void def(struct symbol *name, struct symlist *syms, struct tree *func){
	if(name->syms) freeSymlist(name->syms);
	if(name->func) freeTree(name->func);
	name->syms = syms;
	name->func = func;
}

static double calluser(struct ufncall *f){
	struct symbol *fn = f->s;	// 函数名
	struct symlist *sl;	// 虚拟参数列表
	struct tree *args = f->l;	// 实参
	double *oldval, *newval;	// 参数值
	double v;
	int nargs;
	int i;

	if(!fn->func){
		yyerror("call to undefined function", fn->name);
		return 0;
	}

	sl = fn->syms;	// 计算参数个数
	for(nargs = 0; sl; sl = sl->next)
		nargs++;

	// 保存参数值
	oldval = (double *)malloc(nargs * sizeof(double));
	newval = (double *)malloc(nargs * sizeof(double));
	if(!oldval || !newval){
		yyerror("Out of space in %s", fn->name); return 0.0;
	}

	/* 计算参数值 */
	for(i = 0; i < nargs; i++){
		if(!args){
			yyerror("too few args in call to %s", fn->name);
			free(oldval);free(newval);
			return 0.0;
		}

		if(args->nodeType == 'L') // 判断节点类型是否为L（语句或表达式）
		{
			newval[i] = eval(args->l);
			args = args->r;	
		} else {	// 是否为列表尾
			newval[i] = eval(args);
			args = NULL;
		}
	}

	// 保存虚拟参数旧值，赋予新值
	sl = fn->syms;
	for(i = 0; i < nargs; i++){
		struct symbol *s = sl->sym;

		oldval[i] = s->value;
		s->value = newval[i];
		sl= sl->next;
	}

	free(newval);
		
	// 计算函数
	v = eval(fn->func);

	// 恢复虚拟参数的值
	sl = fn->syms;
	for(i = 0; i < nargs; i++){
		struct symbol *s = sl->sym;

		s->value = oldval[i];
		sl = sl->next;
	}

	free(oldval);
	return v;
}

void yyerror(char *s, ...)
{
	va_list ap;
	va_start(ap,s);

	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

int main()
{
	srand(time(0));	// 设置随机数种子
	printf("$ ");
	yyparse();
	system("pause");
	return 0;
}

