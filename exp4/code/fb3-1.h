/*
 * �������Ĺ�������
 */
/*
 * @author JJYDXFS
 * @version NOV 22 2020
 */

/* ��ʷ��������ӿ� */
extern int yylineno;	// �кţ����Դʷ�������
void yyerror(char *s, ...);

/* ���ű� */
struct symbol{	/* ������ */
	char *name;
	double value;
	struct tree *func; /* ������ */
	struct symlist *syms; /* ����б� */
};

/* �̶���С�ķ��ű� */
#define NHASH 9991
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

/* �����б�����Ϊ�����б� */
struct symlist{
	struct symbol *sym;
	struct symlist *next;
};

struct symlist *nsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* �ڵ�����
 * + - * / | %
 * 0-6 �Ƚϲ�����
 * M ��Ŀ����
 * L ����ʽ��������б�
 * I IF ���
 * W WHILE ���
 * N ��������
 * = ��ֵ
 * S �����б�
 * F ���ú�������
 * U �û���������
 * E ��־����β��
 */

enum bifs{ /* ���ú��� */
	_sqrt = 1,
	_exp,
	_log,
	_sin,
	_cos,
	_rand,
	_pow,
	_print
};

/* �����﷨���ڵ� */
/* nodeType�ǹ��еĳ�ʼ�ڵ� */
struct tree {
	int nodeType;
	struct tree *l;
	struct tree *r;
};

struct fncall { /* ���ú��� */
	int nodeType;	/* ����F */
	struct tree *l;
	enum bifs funcType;
};

struct ufncall { /* �Զ��庯�� */
	int nodeType;	/* ����U */
	struct tree *l;
	struct symbol *s;
};

struct flow {
	int nodeType; /* ����I����W */
	struct tree *cond; /* ���� */
	struct tree *tl; /* ��֧ */
	struct tree *el; /* ��ѡelse */
};

struct numval {
	int nodeType;	/* ����C */
	double number;
};

struct symref {	/* �������� */
	int nodeType;	/* ���� N */
	struct symbol *s;
};

struct symasgn {	/* ��ֵ */
	int nodeType; /* ���� = */
	struct symbol *s;
	struct tree *v;
};

/* �����﷨�� */
struct tree * ntree(int nodeType, struct tree *l, struct tree *r);
struct tree * ncmp(int cmptype, struct tree *l, struct tree *r);
struct tree * nfunc(int functype, struct tree *l);
struct tree * ncall(struct symbol *s, struct tree *l);
struct tree * nref(struct symbol *s);
struct tree * nasgn(struct symbol *s, struct tree *v);
struct tree * nnum(double n);
struct tree * nflow(int nodeType, struct tree *cond, struct tree *tl, struct tree *tr);

/* ���庯�� */
void def(struct symbol *name, struct symlist *syms, struct tree *stmts);

/* ���������﷨�� */
double eval(struct tree *);
/* ��������ļ��㺯�� */
struct tree * veval(int opr, struct tree *v1, struct tree *v2);

/* �ͷ��﷨�� */
void freeTree(struct tree *);

/* ���ú������û��Զ��庯���ļ��� */
static double callbuiltin(struct fncall *);
static double calluser(struct ufncall *);