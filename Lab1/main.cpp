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
	bool end = false;
	if (dop == 1)
		do {
			cout << endl << "Enter your class definitions using the following pattern: " << endl
				<< "class <name_of_class>: <modificator> <name_of_parent_class> { <functions> };" << endl
				<< "If you want to close the program enter \"end\"" << endl << endl;
			getline(cin, text);
			end = classic.CheckString(text);
			if (!end) classic.RetStart();
		} while (!end);
	else {
		MyFile.open("D:/Program/Auto/test.txt");
		if (MyFile.is_open())
		{
			long long dur = 0;
			int count = 0;
			while (getline(MyFile, text)) {
				auto begin = chrono::high_resolution_clock::now();
				end = classic.CheckString(text);
				auto end_t = chrono::high_resolution_clock::now();
				dur += chrono::duration_cast<chrono::microseconds>(begin - end_t).count();
				count++;
				if (!end) classic.RetStart();
			}
			MyFile.close();
			cout << count << " strings were checked in " << dur << " microseconds" << endl;
		}
		else cout << "File didn't opened(((" << endl;
	}
	classic.SayBye();
	return 0;
}