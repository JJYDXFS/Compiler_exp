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
char start;//开始符
set<char> NonTerminal;//存所有的非终结符

//判断是否为非终结符
bool isNonTerminal(char var);
//判断是否为终结符
bool isTerminal(char var);
//文法输入
void InputGrammar();
//输出文法
void VerifyGrammar(vector<grammar> g);
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

map<pair<char, char>, string> M;	// 预测分析表

void GetInput();	// 调试程序
void CreateM();	// 创建预测分析表
bool PredictiveAnalysis(string & input, vector<pair<char, string>> & Derivation);	// 预测分析

#endif // !firstfollowselect.h