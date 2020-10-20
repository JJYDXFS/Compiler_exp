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
	cout << "����First����Ϣ��" << endl;
	getAllFirst();
	cout << "����Follow����Ϣ��" << endl;
	getAllFollow();
	cout << "����Select����Ϣ��" << endl;
	getAllSelect();
	return 0;
}
*/

int main()
{
	InputGrammar();
	VerifyGrammar(G);

	cout << "����First����Ϣ��" << endl;
	getAllFirst();
	cout << "����Follow����Ϣ��" << endl;
	getAllFollow();
	cout << "����Select����Ϣ��" << endl;
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
	cout << "�����ķ���#��Ϊ���������������@��ʾ����" << endl;
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
	cout << "������ķ�Ϊ��" << endl;
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
		if (i != (*ig).right.length() - 1)//�������һλ��������һλ�����
		{
			if (isTerminal((*ig).right[i + 1]))//��һλ���ս����ֱ�Ӽ���follow��
			{
				f.insert((*ig).right[i + 1]);
			}
			else if (isNonTerminal((*ig).right[i + 1]))//��һλ�Ƿ��ս������������
			{
				string sub = (*ig).right.substr(i + 1, (*ig).right.length());
				set<char> subFirst = First(sub);//�ȰѺ������еĶ������ó����������First��
				if (subFirst.find('@') == subFirst.end())//������First�����޿��֣�ֱ�Ӳ�
				{
					set_union(subFirst.begin(), subFirst.end(), f.begin(), f.end(), inserter(f, f.begin()));
				}
				else//�п��֣�ֱ�Ӱ�left��Follow�� ��ȥ�����ֵ�First��sub���ӵ�c��Follow����
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
		else//�����һλ��ֱ�Ӱ�left��Follow���ӵ�c��
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


void GetInput() {	// ���Գ���
	CreateM();
	do {
		string str;
		cout << "��������Ҫ�������ַ�����" << endl;
		cin >> str;
		vector<pair<char, string>> Derivation;	// ����ʽ�б�
		bool flag = PredictiveAnalysis(str, Derivation);
		if (flag) {
			cout << "�ַ������ڸ��ķ������ľ��ӣ������������£�" << endl;
			string temp = Derivation[0].second;
			cout << start <<"->"<< temp << endl;
			// ����Ƶ�����
			for (int i = 1; i < Derivation.size(); i++) {
				temp.replace(temp.find(Derivation[i].first), 1, Derivation[i].second);	// չ�����ս��
				string::size_type epos = 0; 
				epos = temp.find("@");   //�ҵ��մ���λ��
				if (epos != string::npos) temp.replace(temp.find("@"), 1, "");	// �����ڿմ���ɾȥ
				cout << start << "->" << temp;	// �����ǰ�Ƶ����
				cout <<"   ("<<Derivation[i].first << "->" << Derivation[i].second <<")"<<endl;	// ���ʹ�õĲ���ʽ
			}
		}
		else
			cout << "�ַ��������ڸ�LL(1)�ķ������ľ���" << endl;
	} while (true);
}

void CreateM() {	// ����Ԥ�������
	for (int i = 0; i < G.size(); i++)
	{
		set<char> selectSet = Select(G[i].left, G[i].right);	// ��ȡ�����Ӧ��Select��
		set<char>::iterator it;
		for (it = selectSet.begin(); it != selectSet.end(); it++)
			M[make_pair(G[i].left, *it)] = G[i].right;
	}
}

bool PredictiveAnalysis(string & input, vector<pair<char, string>> & Derivation) {	// Ԥ�����
	stack<char>buffer, done;	// buffer:���뻺��ջ��done:չ��ʽջ
	buffer.push('#'); done.push('#'); done.push(start);

	for (int i = input.length() - 1; i >= 0; i--)
		buffer.push(input[i]);	// �����ַ���ջ

	char in = buffer.top();	// ��ǰ��ƥ��������ַ�
	char X = done.top();	// ��ǰչ�����ַ�

	while (X != '#') {	// δ�������βʱ
		// �Ƿ���Ҫ����м�������ڿ��ӻ�
		map<pair<char, char>, string>::iterator p;
		p = M.find(make_pair(X, in));	// ���Ҷ�Ӧ����ʽ

		if (X == in) {	// ջ����ȣ�ƥ��
			buffer.pop(); done.pop();
			in = buffer.top();
		}
		else if (isTerminal(X)) return false;
		else if (p == M.end()) return false;
		else {
			done.pop();
			if (p->second == "@") {
				Derivation.push_back(make_pair(X, p->second));	// �ռ�����ʽ
				X = done.top();
				continue;
			}
			for (int i = p->second.length() - 1; i >= 0; i--)
				done.push(p->second[i]);	// ����ʽ���ַ�������ջ
			Derivation.push_back(make_pair(X, p->second));	// �ռ�����ʽ
		}
		X = done.top();
	}
	return true;
}