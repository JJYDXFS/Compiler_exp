#include <iterator>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <utility>
#include <map>
#include <stack>
using namespace std;
typedef struct grammar
{
	char left;
	string right;
	bool leftRecursion = false; //判断此条文法中的非终结符是否存在左递归
	set<char> select; //存放Select(left->right)
}grammar;
vector<grammar> G;

char start;//开始符
set<char> NonTerminal;//存所有的非终结符

map<pair<char, char>, string> M;    // 预测分析表
bool judgeLL1();//判断是不是LL1文法
bool isNonTerminal(char var);//判断是否为非终结符
bool isTerminal(char var);//判断是否为终结符
void InputGrammar();//文法输入
void setSelect();//求取并存储select集
void VerifyGrammar(vector<grammar> g);//输出文法
void getLeftRecursion();//得到所有存在左递归情况的非终结符
bool isLeftRecursion(char c);//判断某一非终结符是否是左递归的
bool indirect(char c);//判断是否是间接左递归
bool isEmpty(char var);
void printSet(set<char> s);
void getAllFirst();//输出所有First集
void getAllFollow();//输出所有Follow集
void getAllSelect();//输出所有Select集
set<char> First(string s);
set<char> Follow(char c);
set<char> Select(char c, string s);
set<char> delElement(set<char> s, char c);//删除集合s中的字符c
set<char> temp;//用于求Follow集时暂存可能导致自循环的非终结符
set<char> tempfirst;//用于求First集时暂存可能导致自循环的非终结符
void GetInput();    // 调试程序
void CreateM();    // 创建预测分析表
bool PredictiveAnalysis(string& input, vector<pair<char, string>>& Derivation);    //预测分析
void modified();    //由非LL1尝试转成LL1
vector<grammar> extract(vector<grammar>tmpG);     //提取左公因子
vector<string> longestCommonPrefix(vector<string> strs);
char judge_inNonTerminal_ornot(set<char>tmp_Notterminal); //找出一个不在给出范围的非终结符
string LongestCommonPrefix(vector<string>& strs); ////求出最长公共前缀
vector<grammar> EliminateleftRecursion(); //处理左递归
void remove_left_gene(char c); ////消除直接左递归
vector<grammar> ModifyRight(char c, set<string>Right); ////按照给定非终结符的右重构
bool judge_isLeftRecursionornot(); //判断左递归化简是否完成

//bool indirect(char c)
//{
//    vector<char> s;
//    s.push_back(c);
//    for(int i=0;i<s.size();i++)
//    {
////        cout<<"当前符号"<<s[i]<<endl;
//        vector<grammar>::iterator iter;
//        {
//            for(iter=G.begin();iter!=G.end();iter++)
//            {
//                if((*iter).left==s[i])
//                {
////                    cout<<"走到了"<<(*iter).right<<endl;
//                    if(isTerminal((*iter).right[0]))
//                        continue;
////                    cout<<"现在是"<<(*iter).right[0]<<endl;
//                    vector<char>::iterator it=find(s.begin(), s.end(), (*iter).right[0]);
//                    if (it != s.end())
//                    {
////                        cout<<(*iter).right[0]<<"是左递归"<<endl;
//                        return true;
//                    }
//                    else
//                    {
////                        cout<<"把"<<(*iter).right[0]<<"放进去"<<endl;
//                        s.push_back((*iter).right[0]);
//                    }
//                }
//            }
//        }
//    }
////    cout<<c<<"不是左递归"<<endl;
//    return false;
//}
//void getLeftRecursion()
//{
//    vector<grammar>::iterator it;
//    vector<grammar>::iterator iter;
//    for (it = G.begin(); it != G.end(); it++)
//    {
////        cout<<"现在判断"<<(*it).left<<"是不是左递归"<<endl;
////        setVisited();
//        if ((*it).left == (*it).right[0]||indirect((*it).left))
//        {
////            cout<<(*it).left<<"是左递归"<<endl;
//            for (iter = G.begin(); iter != G.end(); iter++)
//            {
//                if ((*iter).left == (*it).left)
//                {
//                    (*iter).leftRecursion = true;
////                    cout<<"已经修改左递归非终结符"<<(*iter).left<<endl;
//                }
//            }
//        }
//    }
//}
//bool isLeftRecursion(char c)
//{
//    vector<grammar>::iterator it;
//    for (it = G.begin(); it != G.end(); it++)
//    {
//        if ((*it).left == c && (*it).leftRecursion)
//        {
////            cout<<(*it).left<<"是左递归"<<endl;
//            return true;
//        }
//    }
//    return false;
//}


int main() {
	InputGrammar();//文法输入
//    printSet(NonTerminal);
	getLeftRecursion();//判断某一非终结符是否是左递归的
//    setSelect();
	VerifyGrammar(G);
	cout << "所有First集信息：" << endl;
	getAllFirst();
	cout << "所有Follow集信息：" << endl;
	getAllFollow();
	cout << "所有Select集信息：" << endl;
	getAllSelect();
	setSelect();

	
	if (judgeLL1())
	{
		GetInput();
	};


	modified();

	
	system("pause");
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
	if (isLeftRecursion(s[0]))
		tempfirst.insert(s[0]);
	//    cout<<"now"<<s<<endl;
	set<char> f;
	if (s == "@")
		f.insert('@');
	else if (s.length() == 1)//如果待求字符串长度为1
	{
		if (isTerminal(s[0]))//单个终结符，直接加
			f.insert(s[0]);
		else if (isNonTerminal(s[0]))//单个非终结符
		{
			set<string> temp;
			set<char> tempf;
			vector<grammar>::iterator iter;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[0])
				{
					if ((*iter).right[0] == s[0] && (*iter).right.length() == 1)//S->S的情况，直接略过
						continue;
					if (tempfirst.find((*iter).right[0]) != tempfirst.end() && (*iter).right.length() == 1)
						continue;
					tempf = First((*iter).right);
					set_union(f.begin(), f.end(), tempf.begin(), tempf.end(), inserter(f, f.begin()));
					tempf.clear();
				}
			}
		}
	}
	else if (isTerminal(s[0]))//待求字符串长度不为1，第一个是终结符，直接加
	{
		f.insert(s[0]);
	}
	else if (isNonTerminal(s[0]))//第一个是非终结符
	{
		if (isLeftRecursion(s[0]))//存在左递归
		{
			set<string> tempr;
			vector<grammar>::iterator iter;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[0])
				{
					tempr.insert((*iter).right);//用tempr暂存非终结符s[0]所有的产生式
				}
			}
			if (tempr.find("@") == tempr.end())//如果s[0]产生式无空串
			{
				//                                cout<<"no@"<<endl;
				set<string>::iterator is;//把带直接左递归的产生式都删掉
				for (is = tempr.begin(); is != tempr.end();)
				{
					if ((*is)[0] == s[0])
					{
						//                        cout<<"delete: "<<(*is)<<endl;
//                        cout<<"del"<<(*is)<<endl;
						tempr.erase(is++);
					}
					else
					{
						is++;

					}
				}
			}
			else    //如果有空串
			{
				//                cout<<"has@"<<endl;
								//带左递归的产生式去掉第一个非终结符
				set<string> newsub;
				set<string>::iterator is;
				for (is = tempr.begin(); is != tempr.end();)
				{
					if ((*is)[0] == s[0])
					{
						newsub.insert((*is).substr(1, (*is).length()));
						tempr.erase(is++);
					}
					else
						is++;
				}
				set_union(newsub.begin(), newsub.end(), tempr.begin(), tempr.end(), inserter(tempr, tempr.begin()));
			}
			//            cout<<"现在的tempr："<<endl;
			//            printContainer(tempr);
						//现在求每个产生式的First集再合并
			set<string>::iterator its;
			for (its = tempr.begin(); its != tempr.end(); its++)
			{
				//                cout<<(*its)<<endl;
				if (tempfirst.find((*its)[0]) != tempfirst.end())
					continue;
				set<char> f1 = First((*its));
				set_union(f1.begin(), f1.end(), f.begin(), f.end(), inserter(f, f.begin()));
			}
		}
		else//不存在左递归
		{
			set<char> tempf = First(string(1, s[0]));
			if (tempf.find('@') != tempf.end())
			{
				set<char> f1 = First(string(1, s[0]));
				set<char>::iterator it;
				for (it = f1.begin(); it != f1.end();)
				{
					if ((*it) == '@')
						f1.erase(it++);
					else
						it++;
				}
				set<char> f2 = First(string(1, s[1]));
				set_union(f1.begin(), f1.end(), f2.begin(), f2.end(), inserter(f, f.begin()));
			}
			else {
				f = First(string(1, s[0]));
			}
		}
	}
	return f;
}
set<char> Follow(char c)
{
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
				tempfirst.clear();
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
			if ((*ig).left == c || temp.find(c) != temp.end())
				continue;
			set<char> lf = Follow((*ig).left);
			set_union(lf.begin(), lf.end(), f.begin(), f.end(), inserter(f, f.begin()));
		}
	}
	temp.insert(c);
	return f;
}
set<char> Select(char c, string s)
{
	if (s == "@")
		return Follow(c);
	set<char> select;
	set<char> first = First(s);
	tempfirst.clear();
	if (first.find('@') == first.end())
	{
		select = first;
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
		tempfirst.clear();
		cout << "First(" << (*iter).right << "):";
		printSet(First((*iter).right));
		tempfirst.clear();
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
bool indirect(char c)
{
	vector<char> s;
	s.push_back(c);
	for (int i = 0; i < s.size(); i++)
	{
		//        cout<<"当前符号"<<s[i]<<endl;
		vector<grammar>::iterator iter;
		{
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[i])
				{
					//                    cout<<"走到了"<<(*iter).right<<endl;
					if (isTerminal((*iter).right[0]))
						continue;
					//                    cout<<"现在是"<<(*iter).right[0]<<endl;
					vector<char>::iterator it = find(s.begin(), s.end(), (*iter).right[0]);
					if (it != s.end())
					{
						//                        cout<<(*iter).right[0]<<"是左递归"<<endl;
						return true;
					}
					else
					{
						//                        cout<<"把"<<(*iter).right[0]<<"放进去"<<endl;
						s.push_back((*iter).right[0]);
					}
				}
			}
		}
	}
	//    cout<<c<<"不是左递归"<<endl;
	return false;
}
void getLeftRecursion()
{
	vector<grammar>::iterator it;
	vector<grammar>::iterator iter;
	for (it = G.begin(); it != G.end(); it++)
	{
		//        cout<<"现在判断"<<(*it).left<<"是不是左递归"<<endl;
		//        setVisited();
		if ((*it).left == (*it).right[0] || indirect((*it).left))
		{
			//            cout<<(*it).left<<"是左递归"<<endl;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == (*it).left)
				{
					(*iter).leftRecursion = true;
					//                    cout<<"已经修改左递归非终结符"<<(*iter).left<<endl;
				}
			}
		}
	}
}
bool isLeftRecursion(char c)
{
	vector<grammar>::iterator it;
	for (it = G.begin(); it != G.end(); it++)
	{
		if ((*it).left == c && (*it).leftRecursion)
		{
			//            cout<<(*it).left<<"是左递归"<<endl;
			return true;
		}
	}
	return false;
}
//void getLeftRecursion()
//{
//    vector<grammar>::iterator it;
//    vector<grammar>::iterator iter;
//    for (it = G.begin(); it != G.end(); it++)
//    {
//        if ((*it).left == (*it).right[0])
//        {
//            for (iter = G.begin(); iter != G.end(); iter++)
//            {
//                if ((*iter).left == (*it).left)
//                {
//                    (*iter).leftRecursion = true;
//                }
//            }
//        }
//    }
//}
//bool isLeftRecursion(char c)
//{
//    vector<grammar>::iterator it;
//    for (it = G.begin(); it != G.end(); it++)
//    {
//        if ((*it).left == c && (*it).leftRecursion)
//            return true;
//    }
//    return false;
//}
void GetInput() {    // 调试程序
	CreateM();
	string str;
	do {
		cout << "请输入需要分析的字符串（输入#表示不再输入）：" << endl;
		cin >> str;
		if (str == "#")
			break;
		vector<pair<char, string>> Derivation;    // 产生式列表
		bool flag = PredictiveAnalysis(str, Derivation);
		if (flag) {
			cout << "字符串属于该文法产生的句子！产生过程如下：" << endl;
			string temp = Derivation[0].second;
			cout << start << "->" << temp << endl;
			// 输出推导过程
			for (int i = 1; i < Derivation.size(); i++) {
				temp.replace(temp.find(Derivation[i].first), 1, Derivation[i].second);    // 展开非终结符
				string::size_type epos = 0;
				epos = temp.find("@");   //找到空串的位置
				if (epos != string::npos) temp.replace(temp.find("@"), 1, "");    // 若存在空串则删去
				cout << start << "->" << temp;    // 输出当前推导情况
				cout << "   (" << Derivation[i].first << "->" << Derivation[i].second << ")" << endl;    // 输出使用的产生式
			}
		}
		else
			cout << "字符串不属于该LL(1)文法产生的句子" << endl;
	} while (true);
	cout << "程序运行结束" << endl;
}

void CreateM() {    // 创建预测分析表
	for (int i = 0; i < G.size(); i++)
	{
		//        set<char> selectSet = Select(G[i].left, G[i].right);    // 获取规则对应的Select集
		//        set<char>::iterator it;
		//        for (it = selectSet.begin(); it != selectSet.end(); it++)
		set<char>::iterator it;
		for (it = G[i].select.begin(); it != G[i].select.end(); it++)
			M[make_pair(G[i].left, *it)] = G[i].right;
	}
}

bool PredictiveAnalysis(string& input, vector<pair<char, string>>& Derivation) {    // 预测分析
	stack<char>buffer, done;    // buffer:输入缓冲栈；done:展开式栈
	buffer.push('#'); done.push('#'); done.push(start);

	for (int i = input.length() - 1; i >= 0; i--)
		buffer.push(input[i]);    // 输入字符入栈

	char in = buffer.top();    // 当前待匹配的输入字符
	char X = done.top();    // 当前展开的字符

	while (X != '#') {    // 未到输入结尾时
		// 是否需要输出中间变量用于可视化
		map<pair<char, char>, string>::iterator p;
		p = M.find(make_pair(X, in));    // 查找对应产生式

		if (X == in) {    // 栈顶相等，匹配
			buffer.pop(); done.pop();
			in = buffer.top();
		}
		else if (isTerminal(X)) return false;
		else if (p == M.end()) return false;
		else {
			done.pop();
			if (p->second == "@") {
				Derivation.push_back(make_pair(X, p->second));    // 收集产生式
				X = done.top();
				continue;
			}
			for (int i = p->second.length() - 1; i >= 0; i--)
				done.push(p->second[i]);    // 产生式各字符倒序入栈
			Derivation.push_back(make_pair(X, p->second));    // 收集产生式
		}
		X = done.top();
	}
	return true;
}
void setSelect()
{
	for (int i = 0; i < G.size(); i++)
	{
		G[i].select = Select(G[i].left, G[i].right);
	}
}

bool judgeLL1()
{
	vector<grammar>::iterator iter;
	vector<grammar>::iterator iter1;
	set<char> w1, w2;
	set<char>::iterator char1;
	set<char>::iterator char2;
	for (iter = G.begin(); iter != G.end(); iter++)
	{
		for (iter1 = iter + 1; iter1 != G.end(); iter1++)
		{
			if (((*iter).left) == ((*iter1).left))
			{
				w1 = Select((*iter).left, (*iter).right);
				w2 = Select((*iter1).left, (*iter1).right);
				for (char1 = w1.begin(); char1 != w1.end(); char1++)
				{
					for (char2 = w2.begin(); char2 != w2.end(); char2++)
					{
						if ((*char1) == (*char2))
						{
							cout << endl << "这不是一个LL1文法 " << endl;
							return false;
						}
					}
				}
			}
		}
	}
	cout << endl << "这是一个LL1文法" << endl;
	return true;
}


//if (isLeftRecursion(s[0]))//存在左递归

//改造
void modified() {
	vector<grammar>tmpG;
	G = extract(tmpG);
	getLeftRecursion();
	setSelect();
	vector<grammar>::iterator it;
	cout << "提取左公因子后判断是否为LL1文法" << endl;
	for (it = G.begin(); it != G.end(); it++)
	{
		cout << (*it).left << "->" << (*it).right << endl;
	}

	cout << "所有First集信息：" << endl;
	getAllFirst();
	cout << "所有Follow集信息：" << endl;
	getAllFollow();
	cout << "所有Select集信息：" << endl;
	getAllSelect();


	if (judgeLL1())
	{
		GetInput();
	}
	else {


		while (judge_isLeftRecursionornot() == true)
		{
			G = EliminateleftRecursion();
			/*vector<grammar>::iterator iter;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				cout << (*iter).left << "->" << (*iter).right << endl;
			}*/
			//重新设置G的两个值
			getLeftRecursion();
			setSelect();
			vector<grammar>::iterator it;
			cout << "消除左递归后判断是否为LL1文法" << endl;
			for (it = G.begin(); it != G.end(); it++)
			{
				cout << (*it).left << "->" << (*it).right << endl;
			}
	
			cout << "所有First集信息：" << endl;
			getAllFirst();
			cout << "所有Follow集信息：" << endl;
			getAllFollow();
			cout << "所有Select集信息：" << endl;
			getAllSelect();


			if (judgeLL1())
			{
				GetInput();
			}
			
		}
	}
	//判断
}

//提取左公因子
vector<grammar> extract(vector<grammar>tmpG) {
	vector<string>prefix;
	set<char>::iterator iter;
	set<char>tmp_Notterminal;
	//vector<grammar>tmpG;
	tmp_Notterminal = NonTerminal;
	int haveprefix = 0;
	vector<string>set_str;
	vector<int>del_ele;
	//tmpG = G;
	for (iter = NonTerminal.begin(); iter != NonTerminal.end(); iter++) //遍历所有非终结符
	{
		haveprefix = 0;
		vector<grammar>::iterator it;
		vector<string> strs;
		vector<string>restrs;
		string longestprefix;

		char s;
		for (it = G.begin(); it != G.end(); it++)   //遍历所有输入        S->
		{
			if ((*it).left == (*iter)) {
				strs.push_back((*it).right);
				s = (*iter);

			}
		}


		restrs = longestCommonPrefix(strs);//求出最长前缀
		for (int i = 0; i < restrs.size(); i++) {//对于每个前缀
			string tmps = restrs[i];
			int len = tmps.length();
			int flag = 1;
			int count_it = 0;
			int count_suf = 0;
			int match = 0;
			char tmp_not;
			string suffix;
			char not_end;//暂存非终结符
			tmp_not = judge_inNonTerminal_ornot(tmp_Notterminal);    //得到一个不在已知范围内的非终结符
			for (it = G.begin(); it != G.end(); it++) {
				flag = 1;
				if ((*it).left == (*iter)) {//左非终结符相等时才执行
					for (int k = 0; k < len; k++) {
						if ((*it).right[k] != tmps[k]) {
							flag = 0;
							break;
						}
					}
					if (!flag) continue;
					else {  //匹配了
						count_suf++;
						suffix = (*it).right.substr(len);
						del_ele.push_back(count_it);
						//tmpG.erase(tmpG.begin() + count_it);
						set_str.push_back(suffix);
					}


				}
				count_it++;
			}

			int t, cnt = 0;
			//tmpG.erase(tmpG.begin() + del_ele[0]);
		 /*   for (int m = 0; m < del_ele.size()-1; m++) {
				cnt++;
				t = del_ele[m];
				if(del_ele[m]<del_ele[m+1])
				tmpG.erase(tmpG.begin() + del_ele[m]-1);
				else
				 tmpG.erase(tmpG.begin() + del_ele[m] - 1);
			}*/
			if (count_suf == 1) {
				grammar g_tmp;
				g_tmp.left = (*iter);
				g_tmp.right = tmps + set_str[0];
				tmpG.push_back(g_tmp);
			}
			else {
				tmp_Notterminal.insert(tmp_not);
				grammar g_tmp;
				g_tmp.left = (*iter);
				g_tmp.right = tmps + tmp_not;
				tmpG.push_back(g_tmp);
				for (int x = 0; x < set_str.size(); x++) {
					grammar gg;
					gg.left = tmp_not;
					if (set_str[x] == "")
						set_str[x] = '@';
					gg.right = set_str[x];
					tmpG.push_back(gg);
				}

			}//else
			set_str.clear();
		}


	}
	NonTerminal = tmp_Notterminal;
	vector<grammar>::iterator it;
	/* for (it = tmpG.begin(); it != tmpG.end(); it++)
	 {
		 cout << (*it).left << "->" << (*it).right << endl;
	 }
	 cout << endl;*/
	return tmpG;
}
char judge_inNonTerminal_ornot(set<char>tmp_Notterminal) {  //找出一个不在给出范围的非终结符
	set<char>::iterator iter;
	char tmps;

	int i = 0;
	while (1) {

		int flag = 1;
		for (iter = tmp_Notterminal.begin(); iter != tmp_Notterminal.end(); iter++) //遍历所有非终结符
		{
			tmps = (char)('A' + i);
			if ((*iter) == tmps) {
				flag = 0;
				break;
			}
		}
		if (flag == 1) break;
		else i++;
	}
	return tmps;
}
vector<string> longestCommonPrefix(vector<string> strs) {  //求出最长公共前缀
	int i = 0, j = 0;
	string tmps;
	vector<string> tmpstr;
	vector<string> restr;//返回的vector
	char s;
	sort(strs.begin(), strs.end());
	while (j < strs.size()) {
		s = strs[i][0]; //暂存每个字符串的第一个字符
		//tmpstr.push_back(strs[i]);
		while (j < strs.size() && strs[j][0] == s) {
			tmpstr.push_back(strs[j]);
			j++;
			i++;
		}

		if (tmpstr.size() == 1) tmps = tmpstr[0][0];
		else
			tmps = LongestCommonPrefix(tmpstr);
		restr.push_back(tmps);
		//cout << "暂存前缀为：" << tmps << endl;
		tmpstr.clear();

	}
	/*for (int i = 1; i < strs.size()-1; i++) {
		s = strs[i][0];
		int j = i + 1;
		while (strs[j][0] == s) {
			j++;
	   }

	}*/
	return restr;
}

string LongestCommonPrefix(vector<string>& strs) {  //求出最长公共前缀
	int i;

	if (strs.size() == 0)
		return "";
	string s = strs[0];
	for (i = 1; i < strs.size(); i++) {
		while (strs[i].find(s) != 0) {
			s = s.substr(0, s.size() - 1);
			if (s.empty())
				return "";
		}
	}
	return s;
}


//处理左递归
vector<grammar> EliminateleftRecursion() {
	set<char>::iterator iter;
	vector<grammar>tempG;
	tempG = G;
	string tempNonTerminal = "";
	for (iter = NonTerminal.begin(); iter != NonTerminal.end(); iter++) //遍历所有非终结符
	{
		tempNonTerminal += *iter;
	}

	for (int i = 0; i < tempNonTerminal.length(); i++) {
		set<string>NewPRight;
		vector<grammar>::iterator it;
		char pi = tempNonTerminal[i];
		for (it = tempG.begin(); it != tempG.end(); it++)   //遍历所有输入        
		{
			if ((*it).left == pi) {
				bool isget = 0;
				string right = (*it).right;
				for (int j = 0; j < i; j++) {
					char pj = tempNonTerminal[j];
					if (pj == right[0]) {
						isget = 1;
						for (auto it1 = G.begin(); it1 != G.end(); it1++) {
							if ((*it1).left == pj)
							{
								string s = (*it1).right + right.substr(1);
								NewPRight.insert(s);
							}
						}
					}
				}
				if (isget == 0) {
					NewPRight.insert(right);
				}
			}
		}

		if (i != 0)
			G = ModifyRight(pi, NewPRight);
		//P[pi] = NewPRight;
	 //cout << "我执行了:"  << endl;
		remove_left_gene(pi);
	}
	return G;
}

//消除直接左递归
void remove_left_gene(char c) {
	char Newtemp;
	vector<grammar>tempG;
	vector<grammar>::iterator it;
	set<char>tempNonTerminal = NonTerminal;
	Newtemp = judge_inNonTerminal_ornot(tempNonTerminal);    //得到一个不在已知范围内的非终结符
	bool isaddNewVn = 0;
	for (it = G.begin(); it != G.end(); it++)   //遍历所有输入        
	{
		if ((*it).left == c) {
			string right = (*it).right;
			if (right[0] == c) {
				isaddNewVn = 1;
				//cout << "我置1:" <<c << endl;
				break;
			}
		}
	}
	if (isaddNewVn) {
		tempG = G;
		set<string>NewPRight;
		set<string>NewPNewVn;
		for (auto it1 = tempG.begin(); it1 != tempG.end(); it1++) {
			if ((*it1).left == c)
			{
				string right = (*it1).right;
				if (right[0] != c) {
					right += Newtemp;
					NewPRight.insert(right);
				}
				else {
					right = right.substr(1);
					right += Newtemp;
					NewPNewVn.insert(right);
				}
				NewPNewVn.insert("@");
				tempNonTerminal.insert(Newtemp);
				NonTerminal.insert(Newtemp);
				G = ModifyRight(Newtemp, NewPNewVn);
				G = ModifyRight(c, NewPRight);
			}
		}
	}

}

//按照给定非终结符的右重构
vector<grammar> ModifyRight(char c, set<string>Right) {
	vector<grammar>tmpG;
	vector<grammar>::iterator it;
	grammar g_tmp;
	for (it = G.begin(); it != G.end(); it++)   //遍历所有输入        S->
	{
		if ((*it).left != c) {
			g_tmp.left = (*it).left;
			g_tmp.right = (*it).right;
			tmpG.push_back(g_tmp);
		}
		else
			;
	}
	set<string>::iterator iter;
	for (iter = Right.begin(); iter != Right.end(); iter++)
	{
		g_tmp.left = c;
		g_tmp.right = (*iter);
		tmpG.push_back(g_tmp);
	}
	return tmpG;
}

bool judge_isLeftRecursionornot() {
	vector<grammar>::iterator it;
	int flag = 1;
	for (it = G.begin(); it != G.end(); it++)
	{
		if ((*it).leftRecursion == true)
		{
			flag = 0;
			break;
		}
	}
	if (flag == 1)
		return false;
	else
		return true;
}