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
	bool leftRecursion = false; //�жϴ����ķ��еķ��ս���Ƿ������ݹ�
	set<char> select; //���Select(left->right)
}grammar;
vector<grammar> G;

char start;//��ʼ��
set<char> NonTerminal;//�����еķ��ս��

map<pair<char, char>, string> M;    // Ԥ�������
bool judgeLL1();//�ж��ǲ���LL1�ķ�
bool isNonTerminal(char var);//�ж��Ƿ�Ϊ���ս��
bool isTerminal(char var);//�ж��Ƿ�Ϊ�ս��
void InputGrammar();//�ķ�����
void setSelect();//��ȡ���洢select��
void VerifyGrammar(vector<grammar> g);//����ķ�
void getLeftRecursion();//�õ����д�����ݹ�����ķ��ս��
bool isLeftRecursion(char c);//�ж�ĳһ���ս���Ƿ�����ݹ��
bool indirect(char c);//�ж��Ƿ��Ǽ����ݹ�
bool isEmpty(char var);
void printSet(set<char> s);
void getAllFirst();//�������First��
void getAllFollow();//�������Follow��
void getAllSelect();//�������Select��
set<char> First(string s);
set<char> Follow(char c);
set<char> Select(char c, string s);
set<char> delElement(set<char> s, char c);//ɾ������s�е��ַ�c
set<char> temp;//������Follow��ʱ�ݴ���ܵ�����ѭ���ķ��ս��
set<char> tempfirst;//������First��ʱ�ݴ���ܵ�����ѭ���ķ��ս��
void GetInput();    // ���Գ���
void CreateM();    // ����Ԥ�������
bool PredictiveAnalysis(string& input, vector<pair<char, string>>& Derivation);    //Ԥ�����
void modified();    //�ɷ�LL1����ת��LL1
vector<grammar> extract(vector<grammar>tmpG);     //��ȡ������
vector<string> longestCommonPrefix(vector<string> strs);
char judge_inNonTerminal_ornot(set<char>tmp_Notterminal); //�ҳ�һ�����ڸ�����Χ�ķ��ս��
string LongestCommonPrefix(vector<string>& strs); ////��������ǰ׺
vector<grammar> EliminateleftRecursion(); //������ݹ�
void remove_left_gene(char c); ////����ֱ����ݹ�
vector<grammar> ModifyRight(char c, set<string>Right); ////���ո������ս�������ع�
bool judge_isLeftRecursionornot(); //�ж���ݹ黯���Ƿ����

//bool indirect(char c)
//{
//    vector<char> s;
//    s.push_back(c);
//    for(int i=0;i<s.size();i++)
//    {
////        cout<<"��ǰ����"<<s[i]<<endl;
//        vector<grammar>::iterator iter;
//        {
//            for(iter=G.begin();iter!=G.end();iter++)
//            {
//                if((*iter).left==s[i])
//                {
////                    cout<<"�ߵ���"<<(*iter).right<<endl;
//                    if(isTerminal((*iter).right[0]))
//                        continue;
////                    cout<<"������"<<(*iter).right[0]<<endl;
//                    vector<char>::iterator it=find(s.begin(), s.end(), (*iter).right[0]);
//                    if (it != s.end())
//                    {
////                        cout<<(*iter).right[0]<<"����ݹ�"<<endl;
//                        return true;
//                    }
//                    else
//                    {
////                        cout<<"��"<<(*iter).right[0]<<"�Ž�ȥ"<<endl;
//                        s.push_back((*iter).right[0]);
//                    }
//                }
//            }
//        }
//    }
////    cout<<c<<"������ݹ�"<<endl;
//    return false;
//}
//void getLeftRecursion()
//{
//    vector<grammar>::iterator it;
//    vector<grammar>::iterator iter;
//    for (it = G.begin(); it != G.end(); it++)
//    {
////        cout<<"�����ж�"<<(*it).left<<"�ǲ�����ݹ�"<<endl;
////        setVisited();
//        if ((*it).left == (*it).right[0]||indirect((*it).left))
//        {
////            cout<<(*it).left<<"����ݹ�"<<endl;
//            for (iter = G.begin(); iter != G.end(); iter++)
//            {
//                if ((*iter).left == (*it).left)
//                {
//                    (*iter).leftRecursion = true;
////                    cout<<"�Ѿ��޸���ݹ���ս��"<<(*iter).left<<endl;
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
////            cout<<(*it).left<<"����ݹ�"<<endl;
//            return true;
//        }
//    }
//    return false;
//}


int main() {
	InputGrammar();//�ķ�����
//    printSet(NonTerminal);
	getLeftRecursion();//�ж�ĳһ���ս���Ƿ�����ݹ��
//    setSelect();
	VerifyGrammar(G);
	cout << "����First����Ϣ��" << endl;
	getAllFirst();
	cout << "����Follow����Ϣ��" << endl;
	getAllFollow();
	cout << "����Select����Ϣ��" << endl;
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
	if (isLeftRecursion(s[0]))
		tempfirst.insert(s[0]);
	//    cout<<"now"<<s<<endl;
	set<char> f;
	if (s == "@")
		f.insert('@');
	else if (s.length() == 1)//��������ַ�������Ϊ1
	{
		if (isTerminal(s[0]))//�����ս����ֱ�Ӽ�
			f.insert(s[0]);
		else if (isNonTerminal(s[0]))//�������ս��
		{
			set<string> temp;
			set<char> tempf;
			vector<grammar>::iterator iter;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[0])
				{
					if ((*iter).right[0] == s[0] && (*iter).right.length() == 1)//S->S�������ֱ���Թ�
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
	else if (isTerminal(s[0]))//�����ַ������Ȳ�Ϊ1����һ�����ս����ֱ�Ӽ�
	{
		f.insert(s[0]);
	}
	else if (isNonTerminal(s[0]))//��һ���Ƿ��ս��
	{
		if (isLeftRecursion(s[0]))//������ݹ�
		{
			set<string> tempr;
			vector<grammar>::iterator iter;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[0])
				{
					tempr.insert((*iter).right);//��tempr�ݴ���ս��s[0]���еĲ���ʽ
				}
			}
			if (tempr.find("@") == tempr.end())//���s[0]����ʽ�޿մ�
			{
				//                                cout<<"no@"<<endl;
				set<string>::iterator is;//�Ѵ�ֱ����ݹ�Ĳ���ʽ��ɾ��
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
			else    //����пմ�
			{
				//                cout<<"has@"<<endl;
								//����ݹ�Ĳ���ʽȥ����һ�����ս��
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
			//            cout<<"���ڵ�tempr��"<<endl;
			//            printContainer(tempr);
						//������ÿ������ʽ��First���ٺϲ�
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
		else//��������ݹ�
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
				tempfirst.clear();
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
		//        cout<<"��ǰ����"<<s[i]<<endl;
		vector<grammar>::iterator iter;
		{
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == s[i])
				{
					//                    cout<<"�ߵ���"<<(*iter).right<<endl;
					if (isTerminal((*iter).right[0]))
						continue;
					//                    cout<<"������"<<(*iter).right[0]<<endl;
					vector<char>::iterator it = find(s.begin(), s.end(), (*iter).right[0]);
					if (it != s.end())
					{
						//                        cout<<(*iter).right[0]<<"����ݹ�"<<endl;
						return true;
					}
					else
					{
						//                        cout<<"��"<<(*iter).right[0]<<"�Ž�ȥ"<<endl;
						s.push_back((*iter).right[0]);
					}
				}
			}
		}
	}
	//    cout<<c<<"������ݹ�"<<endl;
	return false;
}
void getLeftRecursion()
{
	vector<grammar>::iterator it;
	vector<grammar>::iterator iter;
	for (it = G.begin(); it != G.end(); it++)
	{
		//        cout<<"�����ж�"<<(*it).left<<"�ǲ�����ݹ�"<<endl;
		//        setVisited();
		if ((*it).left == (*it).right[0] || indirect((*it).left))
		{
			//            cout<<(*it).left<<"����ݹ�"<<endl;
			for (iter = G.begin(); iter != G.end(); iter++)
			{
				if ((*iter).left == (*it).left)
				{
					(*iter).leftRecursion = true;
					//                    cout<<"�Ѿ��޸���ݹ���ս��"<<(*iter).left<<endl;
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
			//            cout<<(*it).left<<"����ݹ�"<<endl;
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
void GetInput() {    // ���Գ���
	CreateM();
	string str;
	do {
		cout << "��������Ҫ�������ַ���������#��ʾ�������룩��" << endl;
		cin >> str;
		if (str == "#")
			break;
		vector<pair<char, string>> Derivation;    // ����ʽ�б�
		bool flag = PredictiveAnalysis(str, Derivation);
		if (flag) {
			cout << "�ַ������ڸ��ķ������ľ��ӣ������������£�" << endl;
			string temp = Derivation[0].second;
			cout << start << "->" << temp << endl;
			// ����Ƶ�����
			for (int i = 1; i < Derivation.size(); i++) {
				temp.replace(temp.find(Derivation[i].first), 1, Derivation[i].second);    // չ�����ս��
				string::size_type epos = 0;
				epos = temp.find("@");   //�ҵ��մ���λ��
				if (epos != string::npos) temp.replace(temp.find("@"), 1, "");    // �����ڿմ���ɾȥ
				cout << start << "->" << temp;    // �����ǰ�Ƶ����
				cout << "   (" << Derivation[i].first << "->" << Derivation[i].second << ")" << endl;    // ���ʹ�õĲ���ʽ
			}
		}
		else
			cout << "�ַ��������ڸ�LL(1)�ķ������ľ���" << endl;
	} while (true);
	cout << "�������н���" << endl;
}

void CreateM() {    // ����Ԥ�������
	for (int i = 0; i < G.size(); i++)
	{
		//        set<char> selectSet = Select(G[i].left, G[i].right);    // ��ȡ�����Ӧ��Select��
		//        set<char>::iterator it;
		//        for (it = selectSet.begin(); it != selectSet.end(); it++)
		set<char>::iterator it;
		for (it = G[i].select.begin(); it != G[i].select.end(); it++)
			M[make_pair(G[i].left, *it)] = G[i].right;
	}
}

bool PredictiveAnalysis(string& input, vector<pair<char, string>>& Derivation) {    // Ԥ�����
	stack<char>buffer, done;    // buffer:���뻺��ջ��done:չ��ʽջ
	buffer.push('#'); done.push('#'); done.push(start);

	for (int i = input.length() - 1; i >= 0; i--)
		buffer.push(input[i]);    // �����ַ���ջ

	char in = buffer.top();    // ��ǰ��ƥ��������ַ�
	char X = done.top();    // ��ǰչ�����ַ�

	while (X != '#') {    // δ�������βʱ
		// �Ƿ���Ҫ����м�������ڿ��ӻ�
		map<pair<char, char>, string>::iterator p;
		p = M.find(make_pair(X, in));    // ���Ҷ�Ӧ����ʽ

		if (X == in) {    // ջ����ȣ�ƥ��
			buffer.pop(); done.pop();
			in = buffer.top();
		}
		else if (isTerminal(X)) return false;
		else if (p == M.end()) return false;
		else {
			done.pop();
			if (p->second == "@") {
				Derivation.push_back(make_pair(X, p->second));    // �ռ�����ʽ
				X = done.top();
				continue;
			}
			for (int i = p->second.length() - 1; i >= 0; i--)
				done.push(p->second[i]);    // ����ʽ���ַ�������ջ
			Derivation.push_back(make_pair(X, p->second));    // �ռ�����ʽ
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
							cout << endl << "�ⲻ��һ��LL1�ķ� " << endl;
							return false;
						}
					}
				}
			}
		}
	}
	cout << endl << "����һ��LL1�ķ�" << endl;
	return true;
}


//if (isLeftRecursion(s[0]))//������ݹ�

//����
void modified() {
	vector<grammar>tmpG;
	G = extract(tmpG);
	getLeftRecursion();
	setSelect();
	vector<grammar>::iterator it;
	cout << "��ȡ�����Ӻ��ж��Ƿ�ΪLL1�ķ�" << endl;
	for (it = G.begin(); it != G.end(); it++)
	{
		cout << (*it).left << "->" << (*it).right << endl;
	}

	cout << "����First����Ϣ��" << endl;
	getAllFirst();
	cout << "����Follow����Ϣ��" << endl;
	getAllFollow();
	cout << "����Select����Ϣ��" << endl;
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
			//��������G������ֵ
			getLeftRecursion();
			setSelect();
			vector<grammar>::iterator it;
			cout << "������ݹ���ж��Ƿ�ΪLL1�ķ�" << endl;
			for (it = G.begin(); it != G.end(); it++)
			{
				cout << (*it).left << "->" << (*it).right << endl;
			}
	
			cout << "����First����Ϣ��" << endl;
			getAllFirst();
			cout << "����Follow����Ϣ��" << endl;
			getAllFollow();
			cout << "����Select����Ϣ��" << endl;
			getAllSelect();


			if (judgeLL1())
			{
				GetInput();
			}
			
		}
	}
	//�ж�
}

//��ȡ������
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
	for (iter = NonTerminal.begin(); iter != NonTerminal.end(); iter++) //�������з��ս��
	{
		haveprefix = 0;
		vector<grammar>::iterator it;
		vector<string> strs;
		vector<string>restrs;
		string longestprefix;

		char s;
		for (it = G.begin(); it != G.end(); it++)   //������������        S->
		{
			if ((*it).left == (*iter)) {
				strs.push_back((*it).right);
				s = (*iter);

			}
		}


		restrs = longestCommonPrefix(strs);//����ǰ׺
		for (int i = 0; i < restrs.size(); i++) {//����ÿ��ǰ׺
			string tmps = restrs[i];
			int len = tmps.length();
			int flag = 1;
			int count_it = 0;
			int count_suf = 0;
			int match = 0;
			char tmp_not;
			string suffix;
			char not_end;//�ݴ���ս��
			tmp_not = judge_inNonTerminal_ornot(tmp_Notterminal);    //�õ�һ��������֪��Χ�ڵķ��ս��
			for (it = G.begin(); it != G.end(); it++) {
				flag = 1;
				if ((*it).left == (*iter)) {//����ս�����ʱ��ִ��
					for (int k = 0; k < len; k++) {
						if ((*it).right[k] != tmps[k]) {
							flag = 0;
							break;
						}
					}
					if (!flag) continue;
					else {  //ƥ����
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
char judge_inNonTerminal_ornot(set<char>tmp_Notterminal) {  //�ҳ�һ�����ڸ�����Χ�ķ��ս��
	set<char>::iterator iter;
	char tmps;

	int i = 0;
	while (1) {

		int flag = 1;
		for (iter = tmp_Notterminal.begin(); iter != tmp_Notterminal.end(); iter++) //�������з��ս��
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
vector<string> longestCommonPrefix(vector<string> strs) {  //��������ǰ׺
	int i = 0, j = 0;
	string tmps;
	vector<string> tmpstr;
	vector<string> restr;//���ص�vector
	char s;
	sort(strs.begin(), strs.end());
	while (j < strs.size()) {
		s = strs[i][0]; //�ݴ�ÿ���ַ����ĵ�һ���ַ�
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
		//cout << "�ݴ�ǰ׺Ϊ��" << tmps << endl;
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

string LongestCommonPrefix(vector<string>& strs) {  //��������ǰ׺
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


//������ݹ�
vector<grammar> EliminateleftRecursion() {
	set<char>::iterator iter;
	vector<grammar>tempG;
	tempG = G;
	string tempNonTerminal = "";
	for (iter = NonTerminal.begin(); iter != NonTerminal.end(); iter++) //�������з��ս��
	{
		tempNonTerminal += *iter;
	}

	for (int i = 0; i < tempNonTerminal.length(); i++) {
		set<string>NewPRight;
		vector<grammar>::iterator it;
		char pi = tempNonTerminal[i];
		for (it = tempG.begin(); it != tempG.end(); it++)   //������������        
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
	 //cout << "��ִ����:"  << endl;
		remove_left_gene(pi);
	}
	return G;
}

//����ֱ����ݹ�
void remove_left_gene(char c) {
	char Newtemp;
	vector<grammar>tempG;
	vector<grammar>::iterator it;
	set<char>tempNonTerminal = NonTerminal;
	Newtemp = judge_inNonTerminal_ornot(tempNonTerminal);    //�õ�һ��������֪��Χ�ڵķ��ս��
	bool isaddNewVn = 0;
	for (it = G.begin(); it != G.end(); it++)   //������������        
	{
		if ((*it).left == c) {
			string right = (*it).right;
			if (right[0] == c) {
				isaddNewVn = 1;
				//cout << "����1:" <<c << endl;
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

//���ո������ս�������ع�
vector<grammar> ModifyRight(char c, set<string>Right) {
	vector<grammar>tmpG;
	vector<grammar>::iterator it;
	grammar g_tmp;
	for (it = G.begin(); it != G.end(); it++)   //������������        S->
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