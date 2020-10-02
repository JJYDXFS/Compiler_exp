#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "NFA.h"
using namespace std;
// ���ձߵķ��Ŷ���Ϊ'\0'
// ����
vector<NFANode> NodeList;	// �������н��
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
char Precede(char c1,char c2);	// �ж���������ȼ�
NFA Calculate(NFA N1, char op, NFA N2);	// �������op������NFA

int main()
{
	string e; cin >> e;
	//// �������ӷ�-���Բ���
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
	
	for (int i = 0; i < exp.size();) {
		// ���������ջ
		if (!IsOp(exp[i])) 
			NFAstack.push(Basic(exp[i++]));
		else if (Op.empty())Op.push(exp[i]);
		else {
			char op1 = Op.top();
			// �ж�ջ���͵�ǰ����������ȼ���ϵ
			switch (Precede(op1,exp[i]))
			{
			case'<':	// ջ��Ԫ�����ȼ���
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
	// ��������N1.N2����β���ӣ�
	NFA ans; Edge tempe;
	ans.start = N1.start;
	ans.end = N2.end;
	tempe.info = '\0';
	;
}
NFA Closure(NFA N) {
	// �հ�����N*
	;
}
NFA OR(NFA N1, NFA N2) {
	// ������N1|N2
	;
}
NFA Basic(char c) {
	// ��������R=a
	NFA n; NFANode S, E;
	// ��ʼ��NFA
	n.start = status;
	n.end = status + 1;
	// ���������б�
	NodeList.push_back(S);
	NodeList.push_back(E);
	AddEdge(n.start,n.end,c);	// ��cΪת�������ӱ�
	// ����status���
	status = status + 2;
	return n;
}
bool IsOp(char c) {
	// �ж��Ƿ�Ϊ�����
	return (c == '(' || c == ')' || c == '*' || c == '|' || c == '.');
}
char Precede(char c1, char c2) {
	// c1Ϊջ�����������Ҫ������������ȼ����
	// ���رȽ������

}
NFA Calculate(NFA N1, char op, NFA N2) {
	// �������op������NFA
	if (op == '.')return Concatenation(N1, N2);
	if (op == '|')return OR(N1,N2);
}
void AddEdge(int start, int end, char c) {
	// ��c��Ϊת��������start��end��ӱ�
	Edge tempe; tempe.info = c;
	tempe.NodeIndex = end; 
	tempe.nextedge = NULL;
	Edge *p = NodeList[start].firstedge;
	// ��������β���ں�����ϱߡ�δ�깤��
	while (p->nextedge) p = p->nextedge;	

}