#include "firstfollowselect.h"
//template<typename type>
//void printContainer(const type& container)
//{
//    for(auto& item:container){
//        cout<<item<<" ";
//    }
//    cout<<endl;
//}


/*
int main() {
	InputGrammar();
	VerifyGrammar(G);
	cout << "所有First集信息：" << endl;
	getAllFirst();
	cout << "所有Follow集信息：" << endl;
	getAllFollow();
	cout << "所有Select集信息：" << endl;
	getAllSelect();
	return 0;
}
*/

int main()
{
	InputGrammar();
	VerifyGrammar(G);

	cout << "所有First集信息：" << endl;
	getAllFirst();
	cout << "所有Follow集信息：" << endl;
	getAllFollow();
	cout << "所有Select集信息：" << endl;
	getAllSelect();

	GetInput();
	return 0;
}


void InputGrammar()
{
	int i = 0;
	char l = '\0';
	string r;
	grammar g;
	cout << "输入文法（#作为输入结束，空字用@表示）：" << endl;
	while (l != '0')
	{
		i++;
		cin >> l;
		if (l == '#')
			return;
		cin >> r;
		g.left = l;
		g.right = r;
		NonTerminal.insert(l);
		G.push_back(g);
		if (i == 1)
			start = l;
	}
}
void VerifyGrammar(vector<grammar> g)
{
	cout << "输入的文法为：" << endl;
	vector<grammar>::iterator it;
	for (it = g.begin(); it != g.end(); it++)
	{
		cout << (*it).left << "->" << (*it).right << endl;
	}
	cout << endl;
}
set<char> First(string s)
{
	set<char> f;
	if (s == "@")
		f.insert('@');
	else if (s.length() == 1)
	{
		if (isTerminal(s[0]))
			f.insert(s[0]);
		else if (isNonTerminal(s[0]))
		{
			set<string> temp;
			set<char> tempf;
			vector<grammar>::iterator iter;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[0])
				{
					tempf = First((*iter).right);
					set_union(f.begin(), f.end(), tempf.begin(), tempf.end(), inserter(f, f.begin()));
					tempf.clear();
				}
			}

		}
	}
	else if (isTerminal(s[0]))
	{
		f.insert(s[0]);
	}
	else if (isNonTerminal(s[0]))
	{
		set<char> tempf = First(string(1, s[0]));
		if (tempf.find('@') != tempf.end())
		{
			set<char> f1 = f = First(string(1, s[0]));
			set<char>::iterator it;
			for (it = f1.begin(); it != f1.end();)
			{
				if ((*it) == '@')
					f1.erase(it++);
				else
					it++;
			}
			set<char> f2 = f = First(string(1, s[1]));
			set_union(f1.begin(), f1.end(), f2.begin(), f2.end(), inserter(f, f.begin()));
		}
		else {
			f = First(string(1, s[0]));
		}
	}
	return f;
}
set<char> Follow(char c)
{
	temp.insert(c);
	//    cout<<c<<" ";
	set<char> f;
	if (c == start)
		f.insert('#');
	vector<grammar> tempg;
	vector<grammar>::iterator iter;
	for (iter = G.begin(); iter != G.end(); iter++)
	{
		bool flag = false;
		for (int i = 0; i < (*iter).right.length(); i++)
		{
			if ((*iter).right[i] == c)
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			tempg.push_back((*iter));
		}
	}
	vector<grammar>::iterator ig;
	for (ig = tempg.begin(); ig != tempg.end(); ig++)
	{
		int i = (*ig).right.find(c);
		if (i != (*ig).right.length() - 1)//不是最后一位，看后面一位的情况
		{
			if (isTerminal((*ig).right[i + 1]))//后一位是终结符，直接加入follow集
			{
				f.insert((*ig).right[i + 1]);
			}
			else if (isNonTerminal((*ig).right[i + 1]))//后一位是非终结符，分类讨论
			{
				string sub = (*ig).right.substr(i + 1, (*ig).right.length());
				set<char> subFirst = First(sub);//先把后面所有的东西都拿出来，并求出First集
				if (subFirst.find('@') == subFirst.end())//如果求的First集里无空字，直接并
				{
					set_union(subFirst.begin(), subFirst.end(), f.begin(), f.end(), inserter(f, f.begin()));
				}
				else//有空字，直接把left的Follow集 和去掉空字的First（sub）加到c的Follow集上
				{
					subFirst = delElement(subFirst, '@');
					set_union(subFirst.begin(), subFirst.end(), f.begin(), f.end(), inserter(f, f.begin()));
					if (temp.find(c) == temp.end())
						continue;
					set<char> lf = Follow((*ig).left);
					set_union(lf.begin(), lf.end(), f.begin(), f.end(), inserter(f, f.begin()));
				}
			}
		}
		else//是最后一位，直接把left的Follow集加到c上
		{
			if ((*ig).left == c)
				continue;
			set<char> lf = Follow((*ig).left);
			set_union(lf.begin(), lf.end(), f.begin(), f.end(), inserter(f, f.begin()));
		}
	}

	return f;
}
set<char> Select(char c, string s)
{
	if (s == "@")
		return Follow(c);
	set<char> select;
	set<char> first = First(s);
	if (first.find('@') == first.end())
	{
		select = First(s);
	}
	else
	{
		first = delElement(first, '@');
		set<char> follow = Follow(c);
		temp.clear();
		set_union(first.begin(), first.end(), follow.begin(), follow.end(), inserter(select, select.begin()));
	}
	return select;
}
//bool isNonTerminal(char var)
//{
//    if(var>='A'&&var<='Z')
//        return true;
//    else
//        return false;
//}
//bool isTerminal(char var)
//{
//    if(var>='a'&&var<='z')
//        return true;
//    else
//        return false;
//}
void printSet(set<char> s)
{
	set<char>::iterator it;
	for (it = s.begin(); it != s.end(); it++)
	{
		cout << (*it) << " ";
	}
	cout << endl;
}
set<char> delElement(set<char> s, char c)
{
	set<char>::iterator it;
	for (it = s.begin(); it != s.end();)
	{
		if ((*it) == '@')
			s.erase(it++);
		else
			it++;
	}
	return s;
}
void getAllFirst()
{
	vector<grammar>::iterator iter;
	for (iter = G.begin(); iter != G.end(); iter++)
	{
		cout << "First(" << (*iter).left << "):";
		printSet(First(string(1, (*iter).left)));
		cout << "First(" << (*iter).right << "):";
		printSet(First((*iter).right));
	}
}
void getAllFollow()
{
	set<char>::iterator iter;
	for (iter = NonTerminal.begin(); iter != NonTerminal.end(); iter++)
	{
		cout << "Follow(" << (*iter) << "):";
		printSet(Follow((*iter)));
		temp.clear();
	}
}
void getAllSelect()
{
	vector<grammar>::iterator iter;
	for (iter = G.begin(); iter != G.end(); iter++)
	{
		cout << "Select(" << (*iter).left << "->" << (*iter).right << "):";
		printSet(Select((*iter).left, (*iter).right));
	}
}
bool isNonTerminal(char var) {
	if (((int)var > 64) && ((int)var < 91)) {
		return true;
	}
	else {
		return false;
	}
}
bool isEmpty(char var) {
	if ((int)var == 64) {
		return true;
	}
	else {
		return false;
	}
}
bool isTerminal(char var) {
	if ((!isNonTerminal(var)) && (!isEmpty(var))) {
		return true;
	}
	else {
		return false;
	}
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
			cout << start <<"->"<< temp << endl;
			// 输出推导过程
			for (int i = 1; i < Derivation.size(); i++) {
				temp.replace(temp.find(Derivation[i].first), 1, Derivation[i].second);	// 展开非终结符
				string::size_type epos = 0; 
				epos = temp.find("@");   //找到空串的位置
				if (epos != string::npos) temp.replace(temp.find("@"), 1, "");	// 若存在空串则删去
				cout << start << "->" << temp;	// 输出当前推导情况
				cout <<"   ("<<Derivation[i].first << "->" << Derivation[i].second <<")"<<endl;	// 输出使用的产生式
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