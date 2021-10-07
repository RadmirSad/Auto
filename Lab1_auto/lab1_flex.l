%option noyywrap
%option c++
%x PARENT FUNC
%{
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
%}
norm_symb [a-zA-Z_]
digit [0-9]
space [ \t]
name_of_func ({norm_symb}|{digit})+\(\);
name_of_cl {norm_symb}({norm_symb}|{digit})*
%%
<INITIAL>{
class{space}+{name_of_cl}{space}*:{space}*((public{space})|(protected{space})|(private{space}))?{space}* BEGIN(PARENT);
\n {BEGIN(INITIAL);return 0;}
. BEGIN(INITIAL);
}
<PARENT>{
{name_of_cl}/{space}*\{({space}*{name_of_func}{space}*)*\}; {BEGIN(FUNC); return 1;}
. BEGIN(INITIAL);
\n {BEGIN(INITIAL); return 0;}
}
<FUNC>{
. BEGIN(FUNC); 
\n BEGIN(INITIAL);
}
%%
using namespace std;

int main() {
	ifstream MyFile;
	MyFile.open("test.txt");
	if(MyFile.is_open()) {
		bool flag = false;
		stringstream ss;
		vector<string> par;
		string dop;
		while(getline(MyFile, dop)) {
			ss.str(dop);
			yyFlexLexer ftp(ss, cout);
			flag = ftp.yylex();
			if(flag) {
				dop = ftp.YYText();
				cout<<" good "<< dop;
				par.push_back(dop);
			}
		}
		cout << "Parent classes: " << endl;
			for(auto i : par)
				cout << i << endl;
	}
	return 0;
}