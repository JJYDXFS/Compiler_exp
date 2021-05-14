# <center>正规式转NFA</center>

[toc]

## 简要说明

代码分为两个文件：

- EToNFA.cpp（存放主要函数以及用于调试的main()函数）
- NFA.h（存放数据结构）

*调试前请分别在工程中手动建立上述两个文件，代码在附录中*

> 如果出现程序运行异常、崩溃等问题，请联系章同学
>
> 如果出现正规式运行结果错误，请联系鲍同学（在代码提交前，她已仔细验算过）

## 代码详细说明

### 几点前提

1. 在代码中，空串用字符串'\0'表示

2. 实现连接运算ab时，使用如下图的四状态结点形式表示。因为使用三结点将会产生冗余结点，合并不便实现；又考虑到在后续步骤中由连接产生的多余空边对复杂度不会产生太大影响，所以选择四状态

   ![2020-10-02_175857](http://img.joycez.xyz/md/2020-10-02_175857.jpg)

3. 假设输入的正规式是正确的，且暂不考虑出现转义等等其他情况

4. 代码中还有其他注释，有不清楚的地方请联系章同学

### 代码运行流程

![2020-10-02_175121](http://img.joycez.xyz/md/2020-10-02_175121.jpg)

### 使用的数据结构

除标准模板库外，对结点和边定义了类类型（不会对之前使用初版的结构体的代码产生影响），对NFA定义为结构体（只含初态下标和终态下标）。UML图如下：

![微信图片_20201002180603](http://img.joycez.xyz/md/微信图片_20201002180603.jpg)

### 主要变量说明

```c++
// 全局变量
vector<NFANode> NodeList;	// 保存NFA中所有顶点
vector<Edge> EdgeList;	// 保存NFA中所有边
int status = 0;	// NFA结点编号
```

### 主要函数说明

```c++
// 关键函数
vector<char> Addoperator(string e);	// 负责在中缀表达式中加入连接运算符'.'
NFA ExpToNFA(vector<char>Exp);	// 将加入连接运算符后的中缀表达式转换为NFA
// 辅助函数
NFA Basic(char c);	// 实现基础运算R=a
void AddEdge(int start, int end, char c);	// 用'c'作为转移条件加边
NFA Concatenation(NFA N1, NFA N2);	// 实现连接运算N1.N2
NFA Closure(NFA N);	// 实现闭包运算N*
NFA OR(NFA N1, NFA N2);	// 实现或运算N1|N2
bool IsOp(char c);	// 判断'c'是否为运算符
char Precede(char op1,char op2);	// 返回运算符优先级
NFA Calculate(NFA N1, char op, NFA N2);	// 按运算符op计算结果NFA
void Verify(NFA N);	// 【用于中间结果验算，最后展示中不必使用】遍历输出NFA结果
```

### 输出结果说明

起点和终点分别为最终NFA的初态和终态结点

边的表示如图：$0\rightarrow a\rightarrow 1$，表示边从状态0指向状态1，转移条件为输入字符串'a'；其中空串将输出空格

![微信图片_20201002182652](http://img.joycez.xyz/md/微信图片_20201002182652.png)

## 附录

### EToNFA.cpp

```c++
#include <string>
#include <vector>
#include <stack>
#include "NFA.h"
using namespace std;
// 将空边的符号定义为'\0'
// 全局变量
vector<NFANode> NodeList;	// 保存所有图顶点
vector<Edge> EdgeList;	// 保存所有边
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
		string e;cin >> e;
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
	EdgeList.push_back(*tempe);
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
```

### NFA.h

```c++
#pragma once
#include <iostream>
// 所有结点保存在一维列表NodeList中，通过下标访问
typedef int NType;
typedef char InfoType;
typedef struct NFA {	// NFA
	int start;	// 起始结点
	int end;	// 结束结点
}NFA;
class Edge	// 边
{
public:
	int NodeIndex;	// 终点下标
	Edge *nextedge;	// 下一条邻边
	InfoType info;	// 转移条件，即读取的字符
	Edge() {
		NodeIndex = 0;
		nextedge = NULL;
		info = '\0';
	}
	Edge(const Edge &E) {	// 拷贝构造函数
		NodeIndex = E.NodeIndex;
		nextedge = E.nextedge;
		info = E.info;
	}
};
class NFANode	// 状态结点
{
public:
	NType data;	// 保留信息，暂时没用到
	Edge *firstedge;	// 第一条邻边
	NFANode() {
		data = '\0';
		firstedge = NULL;
	}
	NFANode(const NFANode &N) {	// 拷贝构造函数
		data = N.data;
		firstedge = N.firstedge;
	}
};
```

