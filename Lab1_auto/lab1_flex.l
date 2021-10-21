%option noyywrap
%option c++
%x PARENT FUNC
%{
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
%}
norm_symb [a-zA-Z_]
digit [0-9]
space [ \t]
name_of_func ({norm_symb}|{digit})+\(\);
name_of_cl {norm_symb}({norm_symb}|{digit})*
%%
<INITIAL>{
class{space}+{name_of_cl}{space}*\:{space}*((public{space})|(protected{space})|(private{space}))?{space}* BEGIN(PARENT);
.|\n ;
<<EOF>> {return 0;}
}
<PARENT>{
({name_of_cl})/({space}*\{({space}*{name_of_func}{space}*)*\};) {BEGIN(FUNC);}
. BEGIN(INITIAL);
}
<FUNC>{
.* ;
\n BEGIN(INITIAL);
<<EOF>> {return 0;}
}
%%
using namespace std;

int main() {
	ifstream MyFile;
	ofstream FileForInf;
	for(int i = 1; i <= 10; i++) {
		stringstream buf;
		string str = "dop";
		buf << i;
		str += buf.str();
		str += ".txt";
		cout << str << endl;
		MyFile.open(str);
		if(MyFile.is_open()) {
			stringstream rd, wr;
			rd << MyFile.rdbuf();
			str = "Info";
			str += buf.str();
			str += ".txt";
			FileForInf.open(str);
			yyFlexLexer ftp(rd, wr);
			auto begin = chrono::high_resolution_clock::now();
			ftp.yylex();
			auto end = chrono::high_resolution_clock::now();
			cout << "Strings from file were checked for " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
			wr << chrono::duration_cast<chrono::milliseconds>(end - begin).count();
			FileForInf << wr.rdbuf();
			FileForInf.close();
			MyFile.close();
	}
	else cout << "File didn't opened((" << endl;
	}
	return 0;
}