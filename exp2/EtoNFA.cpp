#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<char> Addoperator(string e);	// ����׺���ʽ�м������������+

int main()
{
	string e; 
	// ���Բ���
	while (1) {
		cin >> e;
		vector<char>ee = Addoperator(e);
		for (int i = 0; i < ee.size(); i++)
			cout << ee[i];
		cout << endl;
	}

	return 0;
}

vector<char> Addoperator(string e) {
	vector<char>expression;	// ����������ӷ���ı��ʽ
	for (int i = 0; i < e.length()-1; i++) {
		// ����������������� +
		expression.push_back(e[i]);
		if (e[i + 1] == '(' || e[i + 1] == ')' || e[i + 1] == '|' || e[i + 1] == '*')continue;
		else if(e[i] == '(' || e[i] == ')' || e[i] == '|' || e[i] == '*')continue;
		else expression.push_back('+');
	}
	// ���һ�����ź󲻼����������
	expression.push_back(e[e.length()-1]);
	return expression;
}