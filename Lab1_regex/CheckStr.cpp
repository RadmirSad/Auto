#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

using namespace std;
						/*class имя_класса: спецификатор_доступа имя_родительского_класса { список_функций };*/

void say_bye(const vector<string>& par)
{
	int num = par.size();
	if (num) {
		std::cout << "Parent classes: ";
		for (int i = 0; i < num; ++i) {
			std::cout << par[i];
			i != num - 1 ? std::cout << ", " : std::cout << std::endl;
		}
	}
	else std::cout << "Parents' classes have not been introduced" << std::endl;
	std::cout << "End of program.";
}

int main() {
	string str;
#ifndef DEBUG
	vector<string> par;
#endif
	cmatch result;
	regex regular("^class\\s+"
					"[a-zA-Z_]\\w*\\s*:"
					"\\s+((private\\s)|(public\\s)|(protected\\s))?\\s*"
					"([a-zA-Z_]\\w*)\\s*"
					"\\{(\\s*\\w+\\(\\);)*\\s*\\};$");
	int dop;
	cout << "Type 1) <enter> or others <file>" << endl;
	cin >> dop;
	if (dop == 1)
		while (str != "end")
		{
			str.clear();
			cout << endl << "Enter your class definitions using the following pattern: " << endl
				<< "class <name_of_class>: <specifier> <name_of_parent_class> { <functions> };" << endl
				<< "If you want to close the program enter \"end\"" << endl << endl;
			getline(cin, str);
			if (regex_match(str.c_str(), result, regular))
			{
				cout << "New class was defined" << endl;
#ifndef DEBUG
				auto i = result.cbegin();
				i += 5;
				str = *i;
				par.push_back(str);
#endif	
			}
			else if (str != "end")
				cout << "Incorrect class definition" << endl;
		}
	else {
		ifstream MyFile;
		MyFile.open("D:/Program/Auto/test.txt");
		if (MyFile.is_open()) {
			long long dur = 0;
			int count = 0;
			bool flag = false;
			while (getline(MyFile, str)) {
				auto begin = chrono::high_resolution_clock::now();
				flag = regex_match(str.c_str(), result, regular);
				auto end_t = chrono::high_resolution_clock::now();
				dur += chrono::duration_cast<chrono::microseconds>(begin - end_t).count();
				if (flag)
				{
#ifndef DEBUG
					auto i = result.cbegin();
					i += 5;
					str = *i;
					par.push_back(str);
#endif	
				}
				count++;
			}
			MyFile.close();
		}
		else cout << "File didn't opened((" << endl;
	}
#ifndef DEBUG
	say_bye(par);
	par.clear();
#endif
	return 1;
}