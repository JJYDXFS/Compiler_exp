#include<iostream>
#include<stack>
#include<string>
using namespace std;

bool checkLegal(string check_string);	// ��������ʽ�Ƿ�Ϸ�
bool checkBrackets(string check_string);	// ��������ƥ�估�����ַ��Ϸ���
bool checkOperator(string check_string);	// ���������ʹ�úϷ���
string RemoveAddOp(string check_string);	// ȥ����������������
bool IsAlphameric(char c);	// �ж��Ƿ���ĸ����

bool IsOp(char c) {
	// �ж��Ƿ�Ϊ��������ú�������EtoNFA��
	return (c == '(' || c == ')' || c == '*' || c == '|' || c == '.');
}
bool IsUnaryOp(char c) {
	// �ж��Ƿ�Ϊ��Ŀ�����
	return (c == '*');
}
bool IsBinocularOp(char c) {
	// �ж��Ƿ�Ϊ˫Ŀ�����
	return (c == '|' || c == '.');
}

int main() {
	string e;

	do {
		cout << "����������ʽ��" << endl;
		e.clear();
		cin >> e;
	} while (!checkLegal(e));
	e = RemoveAddOp(e);
	cout << "������ȷ��" << endl;
	cout << "����������ʽ��" << e <<endl;

	return 0;
}

bool checkLegal(string check){	// ��������ʽ�Ƿ�Ϸ�

	// 1��2һ�α�����3һ�α�����4һ�α�����Ŀǰ����Ҫ��������
	// 0. ���Ȳ�����ת��
	// 1. ��������ַ��Ϸ��ԣ���ĸ����+�Ϸ��������������2��һ�����
	// 2. ��������Ƿ�ƥ���Լ�������()�����
	// 3. ��������ʹ���Ƿ���ȷ��һĿ��Ŀ�������
	// 4. ��������������.����ȥ�����е����������.���ڼ��麯����ִ�У�
	return (checkBrackets(check) && checkOperator(check));
}

bool checkBrackets(string check){	// ��������ƥ�估�����ַ��Ϸ���
	stack<int> STACK;	// ����ջ
	for (int i = 0; i < check.length(); i++)
	{
		if (!IsAlphameric(check[i]) && !IsOp(check[i]))
			return false;	// ���ַǷ��ַ�
		if (check[i] == '(') STACK.push(i);	// ��������ջ
		else if (check[i] == ')'){
			if (STACK.empty()) return false;	// ��������
			else if (check[i - 1] == '(') return false;	// ������
			else STACK.pop();	// ����������
		}
	}
	if (!STACK.empty()) return false;	// ��������
	return true;
}

bool checkOperator(string check) {
	// ���������ʹ�úϷ���
	/* ������Ϸ��Թ���
	1. ����������������������ܳ����ڿ�ʼλ��
	2. ˫Ŀ��������ܳ����ڽ�βλ��
	3. ��Ŀ��������ܳ�����˫Ŀ������Ҳࡾ��4�ȼۡ�
	4. ˫Ŀ��������ܳ����ڵ�Ŀ������Ҳ�
	5. ˫Ŀ���������������ܳ���˫Ŀ�����
	6. ˫Ŀ�����������������ţ��Ҳ��������� 
	*/
	if (IsUnaryOp(check[0]) || IsBinocularOp(check[0]))return false;	// ��Ŀ��˫Ŀ����������ܳ����ڿ�ͷ
	if (IsBinocularOp(check[check.length() - 1]))return false;	// ����˫Ŀ��������ܳ����ڽ�β
	for (int i = 1; i < check.length(); i++) {
		if (IsBinocularOp(check[i])) {
			if (IsUnaryOp(check[i + 1]))return false;
			if (IsBinocularOp(check[i - 1]) || IsBinocularOp(check[i + 1]))
				return false;
			if (check[i - 1] == '(' || check[i + 1] == ')') return false;
		}
	}
	return true;
}
string RemoveAddOp(string check) {
	// ȥ����������������.
	string e;
	for (int i = 0; i < check.length(); i++)
		if (check[i] != '.') 
			e.push_back(check[i]);
	return e;
}

bool IsAlphameric(char c){	// �ж��Ƿ���ĸ����
	if (c >= 'a' && c <= 'z' || c >= 'A'&& c<= 'Z' 
		|| c>='0' && c<='9')
		return true;
	return false;
}
