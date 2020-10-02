#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "NFA.h"
using namespace std;
// 将空边的符号定义为'\0'
// 变量
vector<NFANode> NodeList;	// 保存所有结点
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
char Precede(char c1,char c2);	// 判断运算符优先级
NFA Calculate(NFA N1, char op, NFA N2);	// 按运算符op计算结果NFA

int main()
{
	string e; cin >> e;
	//// 加入连接符-测试部分
	//while (1) {
	//	cin >> e;
	//	vector<char>ee = Addoperator(e);
	//	for (int i = 0; i < ee.size(); i++)
	//		cout << ee[i];
	//	cout << endl;
	//}
	// NFA MyNFA = ExpToNFA(Addoperator(e));
	NFA n = Basic('a');

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
	
	for (int i = 0; i < exp.size();) {
		// 非运算符入栈
		if (!IsOp(exp[i])) 
			NFAstack.push(Basic(exp[i++]));
		else if (Op.empty())Op.push(exp[i]);
		else {
			char op1 = Op.top();
			// 判断栈顶和当前运算符的优先级关系
			switch (Precede(op1,exp[i]))
			{
			case'<':	// 栈顶元素优先级低
				Op.push(exp[i++]); break;
			case'=':
				Op.pop(); i++; break;
			case'>':
				if (op1 == '*') {

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
}

NFA Concatenation(NFA N1, NFA N2) {
	// 连接运算N1.N2（首尾连接）
	NFA ans; Edge tempe;
	ans.start = N1.start;
	ans.end = N2.end;
	tempe.info = '\0';
	;
}
NFA Closure(NFA N) {
	// 闭包运算N*
	;
}
NFA OR(NFA N1, NFA N2) {
	// 或运算N1|N2
	;
}
NFA Basic(char c) {
	// 基础运算R=a
	NFA n; NFANode S, E;
	// 初始化NFA
	n.start = status;
	n.end = status + 1;
	// 将结点存入列表
	NodeList.push_back(S);
	NodeList.push_back(E);
	AddEdge(n.start,n.end,c);	// 以c为转移条件加边
	// 更新status编号
	status = status + 2;
	return n;
}
bool IsOp(char c) {
	// 判断是否为运算符
	return (c == '(' || c == ')' || c == '*' || c == '|' || c == '.');
}
char Precede(char c1, char c2) {
	// c1为栈顶运算符【需要建立运算符优先级表格】
	// 返回比较运算符

}
NFA Calculate(NFA N1, char op, NFA N2) {
	// 按运算符op计算结果NFA
	if (op == '.')return Concatenation(N1, N2);
	if (op == '|')return OR(N1,N2);
}
void AddEdge(int start, int end, char c) {
	// 用c作为转移条件在start与end间加边
	Edge tempe; tempe.info = c;
	tempe.NodeIndex = end; 
	tempe.nextedge = NULL;
	Edge *p = NodeList[start].firstedge;
	// 遍历到表尾，在后面接上边【未完工】
	while (p->nextedge) p = p->nextedge;	

}