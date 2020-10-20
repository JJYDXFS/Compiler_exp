#pragma once
#ifndef firstfollowselect.h

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <iterator>

#include <utility>
#include <map>
#include <stack>

using namespace std;

typedef struct grammar
{
	char left;
	string right;
}grammar;

vector<grammar> G;
char start;//��ʼ��
set<char> NonTerminal;//�����еķ��ս��

//�ж��Ƿ�Ϊ���ս��
bool isNonTerminal(char var);
//�ж��Ƿ�Ϊ�ս��
bool isTerminal(char var);
//�ķ�����
void InputGrammar();
//����ķ�
void VerifyGrammar(vector<grammar> g);
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

map<pair<char, char>, string> M;	// Ԥ�������

void GetInput();	// ���Գ���
void CreateM();	// ����Ԥ�������
bool PredictiveAnalysis(string & input, vector<pair<char, string>> & Derivation);	// Ԥ�����

#endif // !firstfollowselect.h