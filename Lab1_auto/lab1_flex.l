%option noyywrap
%option c++
%x NAME COLON SPECIF DOPPAR PARENT FUNC SKIP
%{
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>

std::string name_cl, par;
std::map<std::string, std::string> classes;
%}
norm_symb [a-zA-Z_]
digit [0-9]
space [ \t]
name_of_func ({norm_symb}|{digit})+\(\);
name_of_cl {norm_symb}({norm_symb}|{digit})*
%%
<INITIAL>{
class{space}+ {yyout << "init" << std::endl; BEGIN(NAME);}
.|\n ;
<<EOF>> {return 0;}
}
<NAME>{
({name_of_cl}) {name_cl = YYText(); BEGIN(COLON);}
. BEGIN(INITIAL);
}
<COLON>{
{space}*\:{space}* {BEGIN(SPECIF);}
. BEGIN(INITIAL);
}
<SPECIF>{
((public)|(protected)|(private)) {par = YYText(); BEGIN(DOPPAR);}
({name_of_cl})/({space}*\{({space}*{name_of_func}{space}*)*\}\;) {par = YYText(); classes[name_cl] = par; BEGIN(SKIP);}
. BEGIN(INITIAL);
}
<DOPPAR>{
{space} ;
{name_of_cl} {par = YYText(); BEGIN(FUNC);}
({space}*\{({space}*{name_of_func}{space}*)*\}\;) { classes[name_cl] = par; BEGIN(INITIAL);}
. BEGIN(INITIAL);
}
<PARENT>{
({name_of_cl})/({space}*\{({space}*{name_of_func}{space}*)*\}\;) { par = YYText(); classes[name_cl] = par; BEGIN(FUNC);}
. BEGIN(INITIAL);
}
<FUNC>{
({space}*\{({space}*{name_of_func}{space}*)*\}\;) {classes[name_cl] = par; BEGIN(INITIAL);}
. BEGIN(SKIP);
<<EOF>> {return 0;}
}
<SKIP>{
.* ;
\n BEGIN(INITIAL);
<<EOF>> {return 0;}
}
%%
using namespace std;

int main() {
	ifstream MyFile;
	ofstream FileForInf;
	vector<string> dop;
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
			cout << "Strings from file were checked for " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << endl;
			wr << chrono::duration_cast<chrono::milliseconds>(end - begin).count();
			FileForInf << wr.rdbuf();
			FileForInf.close();
			MyFile.close();
			if(classes.empty()) cout << "Parent classes weren't defined." << endl;
			else {
				for (auto fir_iter : classes) {
					bool flag = true;
					for (auto sec_iter : classes)
						if(fir_iter.second == sec_iter.first)
							flag = false;
					if (flag) dop.push_back(fir_iter.second);
				}
				cout << "Parent classes: ";
				int num = dop.size();
				for (int i = 0; i < num; ++i) {
					std::cout << dop[i];
					i != num - 1 ? std::cout << ", " : std::cout << std::endl;
				}
			}
		}
		else cout << "File didn't opened((" << endl;
	}
	return 0;
}
