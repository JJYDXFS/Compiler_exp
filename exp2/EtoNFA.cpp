#include <string>
#include <vector>
#include <stack>
#include "NFA.h"
using namespace std;
// 将空边的符号定义为'\0'
// 变量
vector<NFANode> NodeList;	// 保存所有图顶点
int status = 0;	// 结点编号
// 主要函数
vector<char> Addoperator(string e);	// 在中缀表达式中加入连接运算符.
NFA ExpToNFA(vector<char>Exp);	// 将加工后的中缀表达式转换为NFA
// 辅助函数
NFA Basic(char c);	// 基础运算R=a
void AddEdge(int start, int end, char c);	// 用c作为转移条件加边
NFA Concatenation(NFA N1, NFA N2);	// 连接运算N1.N2
NFA Closure(NFA N);	// 闭包运算N*
NFA OR(NFA N1, NFA N2);	// 或运算N1|N2
bool IsOp(char c);	// 判断是否为运算符
char Precede(char op1,char op2);	// 判断运算符优先级
NFA Calculate(NFA N1, char op, NFA N2);	// 按运算符op计算结果NFA
void Verify(NFA N);	// 【用于验算】遍历输出结果

int main()
{
	//// 加入连接符-测试部分
	//while (1) {
	//	cin >> e;
	//	vector<char>ee = Addoperator(e);
	//	for (int i = 0; i < ee.size(); i++)
	//		cout << ee[i];
	//	cout << endl;
	//}
	// NFA MyNFA = ExpToNFA(Addoperator(e));
	string e;
	cin >> e;
	Verify(ExpToNFA(Addoperator(e)));
	

	return 0;
}

vector<char> Addoperator(string e) {
	vector<char>expression;	// 定义加入连接符后的表达式
	for (int i = 0; i < e.length()-1; i++) {
		// 遍历加入连接运算符 .
		expression.push_back(e[i]);
		if (e[i + 1] == ')' || e[i + 1] == '|' || e[i + 1] == '*')continue;
		else if(e[i] == '(' || e[i] == '|')continue;
		else expression.push_back('.');
	}
	// 最后一个符号后不加连接运算符
	expression.push_back(e[e.length()-1]);
	return expression;
}

NFA ExpToNFA(vector<char>exp) {
	// 建立自动机栈NFAstack和操作符栈Op
	stack<NFA> NFAstack;stack<char> Op;
	int i = 0;
	while(i < exp.size() || !Op.empty()) {
		// 非运算符入栈
		if (i < exp.size() && !IsOp(exp[i]))
			NFAstack.push(Basic(exp[i++]));
		else if (i < exp.size() && Op.empty())Op.push(exp[i++]);
		else {
			char op1 = Op.top();
			char c = i < exp.size() ?exp[i]:'#';
			// 判断栈顶和当前运算符的优先级关系
			switch (Precede(op1,c))
			{
			case'<':	// 栈顶元素优先级更低
				Op.push(c); i++; break;
			case'=':
				Op.pop(); i++; break;
			case'>':
				if (op1 == '*') {
					Op.pop(); NFA N;
					N = NFAstack.top(); NFAstack.pop();
					NFAstack.push(Closure(N));
				}
				else {
					Op.pop(); NFA N1, N2;
					N2 = NFAstack.top(); NFAstack.pop();
					N1 = NFAstack.top(); NFAstack.pop();
					NFAstack.push(Calculate(N1, op1, N2));
				}
				break;
			default:
				break;
			}
		}
	}
	return NFAstack.top();
}

NFA Concatenation(NFA N1, NFA N2) {
	// 连接运算N1.N2（首尾连接）
	NFA ans;
	ans.start = N1.start;
	ans.end = N2.end;
	// 加入空串连接更方便，否则涉及删除重复结点
	AddEdge(N1.end, N2.start, '\0');
	return ans;
}
NFA Closure(NFA N) {
	// 闭包运算N*
	NFA ans;
	// 初始化NFA,更新status编号
	ans.start = status;
	ans.end = status + 1;
	status = status + 2;
	// 将结点存入列表
	NodeList.push_back(*(new NFANode));
	NodeList.push_back(*(new NFANode));
	// 加边
	AddEdge(ans.start, ans.end, '\0');
	AddEdge(ans.start, N.start, '\0');
	AddEdge(N.end, ans.end, '\0');
	AddEdge(N.end, N.start, '\0');
	return ans;
}
NFA OR(NFA N1, NFA N2) {
	// 或运算N1|N2
	NFA ans; 
	// 初始化NFA,更新status编号
	ans.start = status;
	ans.end = status + 1;
	status = status + 2;
	// 将结点存入列表
	NodeList.push_back(*(new NFANode));
	NodeList.push_back(*(new NFANode));
	AddEdge(ans.start, N1.start, '\0');
	AddEdge(ans.start, N2.start, '\0');
	AddEdge(N1.end, ans.end, '\0');
	AddEdge(N2.end, ans.end, '\0');
	return ans;
}
NFA Basic(char c) {
	// 基础运算R=a
	NFA n; 
	// 初始化NFA,更新status编号
	n.start = status;
	n.end = status + 1;
	status = status + 2;
	// 将结点存入列表
	NodeList.push_back(*(new NFANode));
	NodeList.push_back(*(new NFANode));
	AddEdge(n.start,n.end,c);	// 以c为转移条件加边
	return n;
}
bool IsOp(char c) {
	// 判断是否为运算符
	return (c == '(' || c == ')' || c == '*' || c == '|' || c == '.');
}
char Precede(char op1, char op2) {
	// c1为栈顶运算符【需要建立运算符优先级表格】
	// 返回比较运算符
	if (op2 == '#')return '>';
	switch (op1)
	{
	case'|':
	{
		if (op2 == '(' || op2 == '*' || op2 == '.')
			return '<';
		else return '>';
		break;
	}
	case'.':
	{
		if (op2 == ')' || op2 == '|' || op2 == '.')
			return '>';
		else return '<';
		break;
	}
	case'*':
	{
		if (op2 == ')' || op2 == '|' || op2 == '.' || op2 == '*')
			return '>';
		else return '<';
		break;
	}
	case'(':
	{
		if (op2 == ')')
			return '=';
		else return '<';
		break;
	}
	case')':
	{
		if (op2 == '(')
			return '!';
		else return '>';
		break;
	}
	}
}
NFA Calculate(NFA N1, char op, NFA N2) {
	// 按运算符op计算结果NFA
	if (op == '.')return Concatenation(N1, N2);
	if (op == '|')return OR(N1,N2);
}
void AddEdge(int start, int end, char c) {
	// 用c作为转移条件在start与end间加边
	Edge *tempe=new Edge(); 
	tempe->info = c;
	tempe->NodeIndex = end; 
	Edge *p = NodeList[start].firstedge;
	// 遍历到单链表表尾
	if (!p) // 没有邻边	
		NodeList[start].firstedge = tempe;
	else{	// 有邻边，加在最后一条邻边后
		while (p->nextedge) p = p->nextedge;
		p->nextedge = tempe;
	}
}

void Verify(NFA N) {
	// 遍历输出结果
	cout <<"起点：" <<N.start <<"\t"<<"终点："<<N.end << endl;
	cout << "边：" << endl;
	for (int i = 0; i < NodeList.size(); i++) {
		Edge *p = NodeList[i].firstedge;
		while (p) {
			cout <<i<<"--->"<< p->info<<"--->"<< p->NodeIndex <<endl;
			p = p->nextedge;
		}
		cout << endl;
	}
}