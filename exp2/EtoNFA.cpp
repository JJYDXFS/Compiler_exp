#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<char> Addoperator(string e);	// 在中缀表达式中加入连接运算符+

int main()
{
	string e; 
	// 测试部分
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
	vector<char>expression;	// 定义加入连接符后的表达式
	for (int i = 0; i < e.length()-1; i++) {
		// 遍历加入连接运算符 +
		expression.push_back(e[i]);
		if (e[i + 1] == '(' || e[i + 1] == ')' || e[i + 1] == '|' || e[i + 1] == '*')continue;
		else if(e[i] == '(' || e[i] == ')' || e[i] == '|' || e[i] == '*')continue;
		else expression.push_back('+');
	}
	// 最后一个符号后不加连接运算符
	expression.push_back(e[e.length()-1]);
	return expression;
}