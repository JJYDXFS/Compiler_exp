#include<iostream>
#include<stack>
#include<string>
using namespace std;

bool checkLegal(string check_string);	// 检验正则式是否合法
bool checkBrackets(string check_string);	// 检验括号匹配及输入字符合法性
bool checkOperator(string check_string);	// 检验运算符使用合法性
string RemoveAddOp(string check_string);	// 去掉多余的连接运算符
bool IsAlphameric(char c);	// 判断是否字母数字

bool IsOp(char c) {
	// 判断是否为运算符【该函数已在EtoNFA】
	return (c == '(' || c == ')' || c == '*' || c == '|' || c == '.');
}
bool IsUnaryOp(char c) {
	// 判断是否为单目运算符
	return (c == '*');
}
bool IsBinocularOp(char c) {
	// 判断是否为双目运算符
	return (c == '|' || c == '.');
}

int main() {
	string e;

	do {
		cout << "请输入正则式：" << endl;
		e.clear();
		cin >> e;
	} while (!checkLegal(e));
	e = RemoveAddOp(e);
	cout << "输入正确！" << endl;
	cout << "化简后的正规式：" << e <<endl;

	return 0;
}

bool checkLegal(string check){	// 检验正则式是否合法

	// 1、2一次遍历；3一次遍历；4一次遍历；目前共需要遍历三次
	// 0. 首先不考虑转义
	// 1. 检查输入字符合法性（字母数字+合法运算符），放在2中一起进行
	// 2. 检查括号是否匹配以及空括号()的情况
	// 3. 检查运算符使用是否正确（一目二目运算规则）
	// 4. 如果有连接运算符.，则去掉所有的连接运算符.（在检验函数外执行）
	return (checkBrackets(check) && checkOperator(check));
}

bool checkBrackets(string check){	// 检验括号匹配及输入字符合法性
	stack<int> STACK;	// 括号栈
	for (int i = 0; i < check.length(); i++)
	{
		if (!IsAlphameric(check[i]) && !IsOp(check[i]))
			return false;	// 出现非法字符
		if (check[i] == '(') STACK.push(i);	// 左括号入栈
		else if (check[i] == ')'){
			if (STACK.empty()) return false;	// 有右无左
			else if (check[i - 1] == '(') return false;	// 空括号
			else STACK.pop();	// 弹出左括号
		}
	}
	if (!STACK.empty()) return false;	// 有左无右
	return true;
}

bool checkOperator(string check) {
	// 检验运算符使用合法性
	/* 运算符合法性规则
	1. 除括号外所有运算符均不能出现在开始位置
	2. 双目运算符不能出现在结尾位置
	3. 单目运算符不能出现在双目运算符右侧【与4等价】
	4. 双目运算符不能出现在单目运算符右侧
	5. 双目运算符的两侧均不能出现双目运算符
	6. 双目运算符的左侧无左括号，右侧无右括号 
	*/
	if (IsUnaryOp(check[0]) || IsBinocularOp(check[0]))return false;	// 单目或双目运算符均不能出现在开头
	if (IsBinocularOp(check[check.length() - 1]))return false;	// 所有双目运算符不能出现在结尾
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
	// 去掉多余的连接运算符.
	string e;
	for (int i = 0; i < check.length(); i++)
		if (check[i] != '.') 
			e.push_back(check[i]);
	return e;
}

bool IsAlphameric(char c){	// 判断是否字母数字
	if (c >= 'a' && c <= 'z' || c >= 'A'&& c<= 'Z' 
		|| c>='0' && c<='9')
		return true;
	return false;
}
