// 新增三个头文件
#include <utility>
#include <map>
#include <stack>

// 增加一个全局变量和三个函数
map<pair<char, char>, string> M;	// 预测分析表

void GetInput();	// 调试程序
void CreateM();	// 创建预测分析表
bool PredictiveAnalysis(string & input, vector<pair<char, string>> & Derivation);	// 预测分析

// 调试主程序
int main()
{
	InputGrammar();
	VerifyGrammar(G);
	
	GetInput();
	return 0;
}

void GetInput() {	// 调试程序
	CreateM();
	do {
		string str;
		cout << "请输入需要分析的字符串：" << endl;
		cin >> str;
		vector<pair<char, string>> Derivation;	// 产生式列表
		bool flag = PredictiveAnalysis(str, Derivation);
		if (flag) {
			cout << "字符串属于该文法产生的句子！产生过程如下：" << endl;
			string temp = Derivation[0].second;
			cout << start << "->" << temp << endl;
			// 输出推导过程
			for (int i = 1; i < Derivation.size(); i++) {
				temp.replace(temp.find(Derivation[i].first), 1, Derivation[i].second);	// 展开非终结符
				string::size_type epos = 0;
				epos = temp.find("@");   //找到空串的位置
				if (epos != string::npos) temp.replace(temp.find("@"), 1, "");	// 若存在空串则删去
				cout << start << "->" << temp;	// 输出当前推导情况
				cout << "   (" << Derivation[i].first << "->" << Derivation[i].second << ")" << endl;	// 输出使用的产生式
			}
		}
		else
			cout << "字符串不属于该LL(1)文法产生的句子" << endl;
	} while (true);
}

void CreateM() {	// 创建预测分析表
	for (int i = 0; i < G.size(); i++)
	{
		set<char> selectSet = Select(G[i].left, G[i].right);	// 获取规则对应的Select集
		set<char>::iterator it;
		for (it = selectSet.begin(); it != selectSet.end(); it++)
			M[make_pair(G[i].left, *it)] = G[i].right;
	}
}

bool PredictiveAnalysis(string & input, vector<pair<char, string>> & Derivation) {	// 预测分析
	stack<char>buffer, done;	// buffer:输入缓冲栈；done:展开式栈
	buffer.push('#'); done.push('#'); done.push(start);

	for (int i = input.length() - 1; i >= 0; i--)
		buffer.push(input[i]);	// 输入字符入栈

	char in = buffer.top();	// 当前待匹配的输入字符
	char X = done.top();	// 当前展开的字符

	while (X != '#') {	// 未到输入结尾时
		// 是否需要输出中间变量用于可视化
		map<pair<char, char>, string>::iterator p;
		p = M.find(make_pair(X, in));	// 查找对应产生式

		if (X == in) {	// 栈顶相等，匹配
			buffer.pop(); done.pop();
			in = buffer.top();
		}
		else if (isTerminal(X)) return false;
		else if (p == M.end()) return false;
		else {
			done.pop();
			if (p->second == "@") {
				Derivation.push_back(make_pair(X, p->second));	// 收集产生式
				X = done.top();
				continue;
			}
			for (int i = p->second.length() - 1; i >= 0; i--)
				done.push(p->second[i]);	// 产生式各字符倒序入栈
			Derivation.push_back(make_pair(X, p->second));	// 收集产生式
		}
		X = done.top();
	}
	return true;
}
