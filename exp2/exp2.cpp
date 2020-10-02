#include<iostream>
using namespace std;

char Precede(char a, char b);
int main(){
	char a,b;
	while(1){
		cin>>a>>b;
		cout<<Precede(a,b)<<endl;
	}
    return 0;
}

char Precede(char a, char b)
{
	//判断符号优先级的函数
	switch (a)
	{
		case'|':
		{
			if (b == '(' || b == '*' ||  b == '.')
				return '<';
			else return '>';
			break;
		}
		case'.':
		{
			if (b == ')' || b == '|' || b == '.' )
				return '>';
			else return '<';
			break;
		}
		case'*':
		{
			if (b == '）' || b == '|' || b == '.'||b=='*')
				return '>';
			else return '<';
			break;
		}
		case'(':
		{
			if (b == ')')
				return '=';
			else return '<';
			break;
		}
		case')':
		{
			if (b = '(')
				return '!';
			else return '>';
			break;
		}
	}
}									
