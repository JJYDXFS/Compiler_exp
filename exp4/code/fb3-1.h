/*
 * 计算器的共用声明
 */
/*
 * @author JJYDXFS
 * @version NOV 22 2020
 */

/* 与词法分析器接口 */
extern int yylineno;	// 行号，来自词法分析器
void yyerror(char *s, ...);

/* 符号表 */
struct symbol{	/* 变量名 */
	char *name;
	double value;
	struct tree *func; /* 函数体 */
	struct symlist *syms; /* 虚参列表 */
};

/* 固定大小的符号表 */
#define NHASH 9991
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

/* 符号列表，作为参数列表 */
struct symlist{
	struct symbol *sym;
	struct symlist *next;
};

struct symlist *nsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* 节点类型
 * + - * / | %
 * 0-6 比较操作符
 * M 单目负号
 * L 表达式或者语句列表
 * I IF 语句
 * W WHILE 语句
 * N 符号引用
 * = 赋值
 * S 符号列表
 * F 内置函数调用
 * U 用户函数调用
 * E 标志向量尾部
 */

enum bifs{ /* 内置函数 */
	_sqrt = 1,
	_exp,
	_log,
	_sin,
	_cos,
	_rand,
	_pow,
	_print
};

/* 抽象语法树节点 */
/* nodeType是公有的初始节点 */
struct tree {
	int nodeType;
	struct tree *l;
	struct tree *r;
};

struct fncall { /* 内置函数 */
	int nodeType;	/* 类型F */
	struct tree *l;
	enum bifs funcType;
};

struct ufncall { /* 自定义函数 */
	int nodeType;	/* 类型U */
	struct tree *l;
	struct symbol *s;
};

struct flow {
	int nodeType; /* 类型I或者W */
	struct tree *cond; /* 条件 */
	struct tree *tl; /* 分支 */
	struct tree *el; /* 可选else */
};

struct numval {
	int nodeType;	/* 类型C */
	double number;
};

struct symref {	/* 符号引用 */
	int nodeType;	/* 类型 N */
	struct symbol *s;
};

struct symasgn {	/* 赋值 */
	int nodeType; /* 类型 = */
	struct symbol *s;
	struct tree *v;
};

/* 构造语法树 */
struct tree * ntree(int nodeType, struct tree *l, struct tree *r);
struct tree * ncmp(int cmptype, struct tree *l, struct tree *r);
struct tree * nfunc(int functype, struct tree *l);
struct tree * ncall(struct symbol *s, struct tree *l);
struct tree * nref(struct symbol *s);
struct tree * nasgn(struct symbol *s, struct tree *v);
struct tree * nnum(double n);
struct tree * nflow(int nodeType, struct tree *cond, struct tree *tl, struct tree *tr);

/* 定义函数 */
void def(struct symbol *name, struct symlist *syms, struct tree *stmts);

/* 遍历计算语法树 */
double eval(struct tree *);
/* 针对向量的计算函数 */
struct tree * veval(int opr, struct tree *v1, struct tree *v2);

/* 释放语法树 */
void freeTree(struct tree *);

/* 内置函数与用户自定义函数的计算 */
static double callbuiltin(struct fncall *);
static double calluser(struct ufncall *);
