#define _CRT_SECURE_NO_WARNINGS
#include "CheckClass.h"

CheckClass::CheckClass() : _fsm(*this) {}

void errMsg() { std::cout << "Incorrect class definition" << std::endl; }

bool CheckClass::Comp(int a, int b) {
	if (a == b)
	{
		errMsg();
		_fsm.Other();
		return false;
	}
	return true;
}

void CheckClass::SayBye() {
	int num = par.size();
	if (num) {
		std::cout << "Parent classes: ";
		for (int i = 0; i < num; ++i) {
			std::cout << par[i];
			i != num - 1? std::cout << ", ": std::cout << std::endl;
		}
	}
	else std::cout << "Parents' classes have not been introduced" << std::endl;
	std::cout << "End of program.";
}

void CheckClass::AddPar(const std::string& str)
{
	int sz = par.size();
	for (int i = 0; i < sz; i++)
		if (par[i] == str)
			return;
	par.push_back(str);
}

bool CheckClass::CheckName(const std::string& dop)
{
	if (((dop[0] < 65) || (dop[0] > 90)) && (dop[0] != 95) && ((dop[0] < 97) || (dop[0] > 122))) {
		errMsg();
		return false;
	}
	int leng = dop.length();
	for (int ind = 1; ind < leng; ++ind)
		if (((dop[ind] < 48) || (dop[ind] > 57)) && ((dop[ind] < 65) || (dop[ind] > 90))
			&& (dop[ind] != 95) && ((dop[ind] < 97) || (dop[ind] > 122))) {
			errMsg();
			return false;
		}
	return true;
}

bool CheckClass::CheckMod(const std::string& modif)
{
	if (modif == "private")
		return true;
	if (modif == "public")
		return true;
	return modif == "protected";
}

bool CheckClass::CheckFunc(const std::string& funcs)
{
	int i = 0, len = funcs.length();
	if (len <= 2)
	{
		errMsg();
		_fsm.Other();
		return false;
	}
	for (; i < len - 1; i++)
	{
		if (i != len - 2) {
			if (((funcs[i] < 48) || (funcs[i] > 57)) && ((funcs[i] < 65) || (funcs[i] > 90))
				&& (funcs[i] != 95) && ((funcs[i] < 97) || (funcs[i] > 122)))
			{
				errMsg();
				_fsm.Other();
				return false;
			}
		}
		else if ((funcs[i] != '(') || (funcs[i + 1] != ')'))
		{
			errMsg();
			_fsm.Other();
			return false;
		}
	}
	return true;
}

bool CheckClass::CheckString(const std::string& ent) {
	_fsm.enterStartState();
	int length = ent.length();
	if (length > 5) {
		std::string dop;
		bool flag = true;
		dop.append(ent, 0, 6);
		if (dop == "class ") {										// Check first word "class"
			_fsm.Class();
			int fir_ind = 6;
			while ((fir_ind < length) && ((ent[fir_ind] == ' ') || (ent[fir_ind] == '\t')))
				++fir_ind;											// Trying to find second word
			if (!Comp(length, fir_ind)) return false;
			int sec_ind = fir_ind;
			while ((sec_ind < length) && (ent[sec_ind] != ':'))		// Find some word with possible " " in the end of the word 
				++sec_ind;
			if (!Comp(sec_ind, length)) return false;
			int i = 1;												// Deleting unnecessary " " and tabs
			while ((ent[sec_ind - i] == ' ') || (ent[sec_ind - i] == '\t'))
				++i;
			sec_ind = sec_ind - i + 1;
			dop.clear();
			dop.append(ent, fir_ind, sec_ind - fir_ind);
			flag = CheckName(dop);									// Check the name of the class
			if (!flag) {
				_fsm.Other();
				return false;
			}
			_fsm.NameOfClass();
			fir_ind = sec_ind;
			while ((fir_ind < length) && (ent[fir_ind] != ':'))		// Find symb ":" 
				++fir_ind;
			++fir_ind;
			if ((fir_ind < length) && (ent[fir_ind] != ' ') && (ent[fir_ind] != '\t')) {
				errMsg();											// Check on ' ' after ':'
				_fsm.Other();
				return false;
			}														// Find the first symbol of the next word
			while ((fir_ind < length) && ((ent[fir_ind] == ' ') || (ent[fir_ind] == '\t')))
				++fir_ind;
			if (!Comp(length, fir_ind)) return false;
			sec_ind = fir_ind;
			while ((sec_ind < length) && (ent[sec_ind] != ' ') && (ent[sec_ind] != '\t'))
				++sec_ind;											// Find the third word
			if (!Comp(length, sec_ind)) return false;
			dop.clear();
			dop.append(ent, fir_ind, sec_ind - fir_ind);
			flag = CheckMod(dop);
			if (flag)												// If third word was the modificator
			{														// Find the fourth word
				_fsm.State();
				fir_ind = sec_ind + 1;
				while ((fir_ind < length) && ((ent[fir_ind] == ' ') || (ent[fir_ind] == '\t')))
					++fir_ind;
				if (!Comp(length, fir_ind)) return false;
				sec_ind = fir_ind;
				while ((sec_ind < length) && (ent[sec_ind] != ' ') && (ent[sec_ind] != '\t'))
					++sec_ind;
				if (!Comp(length, sec_ind)) return false;
				dop.clear();
				dop.append(ent, fir_ind, sec_ind - fir_ind);
			}

			flag = CheckName(dop);									// Check parent's name
			if(!flag)
			{
				_fsm.Other();
				return false;
			}
			_fsm.NameOfClass();
			fir_ind = sec_ind + 1;									// Find the symbol '{'
			while ((fir_ind < length) && (ent[fir_ind] != '{')) {
				if ((ent[fir_ind] != ' ') && (ent[fir_ind] != '\t')) {
					errMsg();										// Check on any symbol after name of class
					_fsm.Other();
					return false;
				}
				fir_ind++;
			}
				
			if (!Comp(length, fir_ind)) return false;
			sec_ind = fir_ind + 1;
			bool find_another_symb = false, find_end = true;
			while ((sec_ind < length) && (ent[sec_ind] != '}'))		// while we didn't find the symbol '}'
			{
				if (!find_another_symb) {							// Is it a word or another symbols
					if ((ent[sec_ind] != ' ') && (ent[sec_ind] != '\t'))
					{
						find_another_symb = true;
						find_end = false;
						fir_ind = sec_ind;
					}
				}
				else
					if (ent[sec_ind] == ';')
					{
						find_another_symb = false;
						find_end = true;
						std::string buf;
						buf.append(ent, fir_ind, sec_ind - fir_ind);
						flag = CheckFunc(buf);
						if (!flag)
							return false;
						fir_ind = sec_ind + 1;
					}
				
				sec_ind++;
			}
			if (!Comp(sec_ind, length))
				return false;
			if (!find_end) {
				errMsg();
				_fsm.Other();
				return false;
			}
			++sec_ind;
			if((sec_ind >= length) || (ent[sec_ind] != ';'))
			{
				errMsg();
				_fsm.Other();
				return false;
			}
			++sec_ind;
			for(; sec_ind < length; sec_ind++)
				if ((ent[sec_ind] != ' ') && (ent[sec_ind] != '\t'))
				{
					errMsg();
					_fsm.Other();
					return false;
				}
			_fsm.Funcs();
			AddPar(dop);
			std::cout << "New class was defined" << std::endl;
			return false;
		}
		return false;
	}
	else
		if (ent == "end") {
			_fsm.End();
			return true;
		}
		else
		{
			std::cout << "Incorrect class definition" << std::endl;
			_fsm.Other();
			return false;
		}
}