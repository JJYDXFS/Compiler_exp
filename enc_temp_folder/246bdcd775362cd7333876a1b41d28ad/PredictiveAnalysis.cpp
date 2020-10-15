#include <iostream>
#include <map>
#include <utility>
#include <string>
using namespace std;

int main()
{

	map<pair<char,char>, string> M;
	char VT[5] = {'a','b','c','d','#'};
	char VN[4] = {'A','B','C','D'};
	for (auto x : VT) 
		for (auto y : VN) 
			M[make_pair(y, x)] = "error";
	for (auto x : M) {
		cout << x.first.first << '\t'
			<< x.first.second << '\t'
			<< x.second << endl;
	}
	return 0;
}

