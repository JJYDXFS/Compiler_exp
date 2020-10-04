#include <string>
#include <vector>
#include <stack>
#include "NFA.h"
using namespace std;
// ���ձߵķ��Ŷ���Ϊ'\0'
// ����
vector<NFANode> NodeList;	// ��������ͼ����
int status = 0;	// �����
// ��Ҫ����
vector<char> Addoperator(string e);	// ����׺���ʽ�м������������.
NFA ExpToNFA(vector<char>Exp);	// ���ӹ������׺���ʽת��ΪNFA
// ��������
NFA Basic(char c);	// ��������R=a
void AddEdge(int start, int end, char c);	// ��c��Ϊת�������ӱ�
NFA Concatenation(NFA N1, NFA N2);	// ��������N1.N2
NFA Closure(NFA N);	// �հ�����N*
NFA OR(NFA N1, NFA N2);	// ������N1|N2
bool IsOp(char c);	// �ж��Ƿ�Ϊ�����
char Precede(char op1,char op2);	// �ж���������ȼ�
NFA Calculate(NFA N1, char op, NFA N2);	// �������op������NFA
void Verify(NFA N);	// ���������㡿����������

int main()
{
	//// �������ӷ�-���Բ���
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
	vector<char>expression;	// ����������ӷ���ı��ʽ
	for (int i = 0; i < e.length()-1; i++) {
		// ����������������� .
		expression.push_back(e[i]);
		if (e[i + 1] == ')' || e[i + 1] == '|' || e[i + 1] == '*')continue;
		else if(e[i] == '(' || e[i] == '|')continue;
		else expression.push_back('.');
	}
	// ���һ�����ź󲻼����������
	expression.push_back(e[e.length()-1]);
	return expression;
}

NFA ExpToNFA(vector<char>exp) {
	// �����Զ���ջNFAstack�Ͳ�����ջOp
	stack<NFA> NFAstack;stack<char> Op;
	int i = 0;
	while(i < exp.size() || !Op.empty()) {
		// ���������ջ
		if (i < exp.size() && !IsOp(exp[i]))
			NFAstack.push(Basic(exp[i++]));
		else if (i < exp.size() && Op.empty())Op.push(exp[i++]);
		else {
			char op1 = Op.top();
			char c = i < exp.size() ?exp[i]:'#';
			// �ж�ջ���͵�ǰ����������ȼ���ϵ
			switch (Precede(op1,c))
			{
			case'<':	// ջ��Ԫ�����ȼ�����
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
	// ��������N1.N2����β���ӣ�
	NFA ans;
	ans.start = N1.start;
	ans.end = N2.end;
	// ����մ����Ӹ����㣬�����漰ɾ���ظ����
	AddEdge(N1.end, N2.start, '\0');
	return ans;
}
NFA Closure(NFA N) {
	// �հ�����N*
	NFA ans;
	// ��ʼ��NFA,����status���
	ans.start = status;
	ans.end = status + 1;
	status = status + 2;
	// ���������б�
	NodeList.push_back(*(new NFANode));
	NodeList.push_back(*(new NFANode));
	// �ӱ�
	AddEdge(ans.start, ans.end, '\0');
	AddEdge(ans.start, N.start, '\0');
	AddEdge(N.end, ans.end, '\0');
	AddEdge(N.end, N.start, '\0');
	return ans;
}
NFA OR(NFA N1, NFA N2) {
	// ������N1|N2
	NFA ans; 
	// ��ʼ��NFA,����status���
	ans.start = status;
	ans.end = status + 1;
	status = status + 2;
	// ���������б�
	NodeList.push_back(*(new NFANode));
	NodeList.push_back(*(new NFANode));
	AddEdge(ans.start, N1.start, '\0');
	AddEdge(ans.start, N2.start, '\0');
	AddEdge(N1.end, ans.end, '\0');
	AddEdge(N2.end, ans.end, '\0');
	return ans;
}
NFA Basic(char c) {
	// ��������R=a
	NFA n; 
	// ��ʼ��NFA,����status���
	n.start = status;
	n.end = status + 1;
	status = status + 2;
	// ���������б�
	NodeList.push_back(*(new NFANode));
	NodeList.push_back(*(new NFANode));
	AddEdge(n.start,n.end,c);	// ��cΪת�������ӱ�
	return n;
}
bool IsOp(char c) {
	// �ж��Ƿ�Ϊ�����
	return (c == '(' || c == ')' || c == '*' || c == '|' || c == '.');
}
char Precede(char op1, char op2) {
	// c1Ϊջ�����������Ҫ������������ȼ����
	// ���رȽ������
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
	// �������op������NFA
	if (op == '.')return Concatenation(N1, N2);
	if (op == '|')return OR(N1,N2);
}
void AddEdge(int start, int end, char c) {
	// ��c��Ϊת��������start��end��ӱ�
	Edge *tempe=new Edge(); 
	tempe->info = c;
	tempe->NodeIndex = end; 
	Edge *p = NodeList[start].firstedge;
	// �������������β
	if (!p) // û���ڱ�	
		NodeList[start].firstedge = tempe;
	else{	// ���ڱߣ��������һ���ڱߺ�
		while (p->nextedge) p = p->nextedge;
		p->nextedge = tempe;
	}
}

void Verify(NFA N) {
	// ����������
	cout <<"��㣺" <<N.start <<"\t"<<"�յ㣺"<<N.end << endl;
	cout << "�ߣ�" << endl;
	for (int i = 0; i < NodeList.size(); i++) {
		Edge *p = NodeList[i].firstedge;
		while (p) {
			cout <<i<<"--->"<< p->info<<"--->"<< p->NodeIndex <<endl;
			p = p->nextedge;
		}
		cout << endl;
	}
}