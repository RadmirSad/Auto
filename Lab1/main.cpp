#define _CRT_SECURE_NO_WARNINGS

#include "CheckClass.h"
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

int main() {
	ifstream MyFile;
	string text;
	CheckClass classic;
	int dop;
	cout << "Type 1) <enter> or others <file>" << endl;
	cin >> dop;
	getline(cin, text);
	bool end = false;
	if (dop == 1)
		do {
			cout << endl << "Enter your class definitions using the following pattern: " << endl
				<< "class <name_of_class>: <modificator> <name_of_parent_class> { <functions> };" << endl
				<< "If you want to close the program enter \"end\"" << endl << endl;
			getline(cin, text);
			if (text == "end") end = true;
			classic.check_str(text);
			if (!end) classic.rest_stage();
		} while (!end);
	else {
		MyFile.open("D:/Program/Auto/dop3.txt");
		if (MyFile.is_open())
		{
			ofstream ForInfo;
			long long dur = 0;
			int count = 0;
			while (getline(MyFile, text)) {
				if (text == "end") end = true;
				auto begin = chrono::high_resolution_clock::now();
				classic.check_str(text);
				auto end_t = chrono::high_resolution_clock::now();
				dur += chrono::duration_cast<chrono::milliseconds>(end_t - begin).count();
				count++;
				if (!end) classic.rest_stage();
			}
			MyFile.close();
			cout << count << " strings were checked in " << dur << " milliseconds" << endl;
			ForInfo.open("D:/Program/Auto/Info.txt");
			if (ForInfo.is_open()) {
				ForInfo << dur;
				ForInfo.close();
			}
		}
		else cout << "File didn't opened(((" << endl;
	}
	return 0;
}