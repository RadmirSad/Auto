#define _CRT_SECURE_NO_WARNINGS

#include "CheckClass.h"

void CheckClass::err_msg(int type) {
	switch (type) {
	case word_err:
		std::cout << "Defenition should start with the word <class>" << std::endl;
		break;
	case name_cl:
		std::cout << "You made a mistake in the name of class" << std::endl;
		break;
	case colon_err:
		std::cout << "The class name must be followed by a colon" << std::endl;
		break;
	case spec_err:
		std::cout << "There is no specificator or the name of parent class" << std::endl;
		break;
	case par_err:
		std::cout << "You made a mistake in the name of parent class" << std::endl;
		break;
	case lbr_err:
		std::cout << "The parent class name must be followed by an opening curly bracket" << std::endl;
		break;
	case rbr_err:
		std::cout << "You made a mistake in the definitions of functions or you didn't write a closing curly bracket" << std::endl;
		break;
	case fin_col_err:
		std::cout << "There must be a semicolon after the closing parenthesis" << std::endl;
		break;
	case ok:
		std::cout << "New class was defined!" << std::endl;
		break;
	}
}

void CheckClass::say_bye() {
	if (!classes.empty()) {
		std::vector<std::string> dop;
		for (auto fir_iter : classes) {
			bool flag = true;
			for (auto sec_iter : classes)
				if (fir_iter.second == sec_iter.first)
					flag = false;
			if (flag) dop.push_back(fir_iter.second);
		}
		std::cout << "Parent classes: ";
		int num = dop.size();
		for (int i = 0; i < num; ++i) {
			std::cout << dop[i];
			i != num - 1 ? std::cout << ", " : std::cout << std::endl;
		}
	}
	else std::cout << "Parents' classes have not been introduced" << std::endl;
}

bool CheckClass::is_specif(const std::string& str) {
	if (str == "private")
		return true;
	if (str == "public")
		return true;
	return str == "protected";
}

bool CheckClass::is_func(const std::string& str) {
	int i = 0, len = str.length();
	if (len <= 2)
		return false;
	for (; i < len - 1; i++)
	{
		if (i != len - 2) {
			if (((str[i] < 48) || (str[i] > 57)) && ((str[i] < 65) || (str[i] > 90))
				&& (str[i] != 95) && ((str[i] < 97) || (str[i] > 122)))
				return false;
		}
		else if ((str[i] != '(') || (str[i + 1] != ')'))
			return false;
	}
	return true;
}

int CheckClass::is_name_of_class(const std::string& str) {
	if (str.empty()) return 2;
	if (((str[0] < 65) || (str[0] > 90)) && (str[0] != 95) && ((str[0] < 97) || (str[0] > 122)))
		return false;
	int leng = str.length();
	for (int ind = 1; ind < leng; ++ind)
		if (((str[ind] < 48) || (str[ind] > 57)) && ((str[ind] < 65) || (str[ind] > 90))
			&& (str[ind] != 95) && ((str[ind] < 97) || (str[ind] > 122)))
			return 0;
	return 1;
}

void CheckClass::check_str(const std::string& str) {
	_fsm.enterStartState();
	_fsm.Class(str);
	if (!word_st) return;
	while ((!err) && !(class_st))
		_fsm.NameOfClass(str);
	if (err) return;
	while ((!err) && (!col_st))
		_fsm.DesiredSymb(str);
	if (err) return;
	while ((!err) && (!par_st) && (!spec_st))
		_fsm.NextWd(str);
	if (err) return;
	if (spec_st) {
		while ((!err) && (!par_st))
			_fsm.NextState(str);
		if (err) return;
	}
	while ((!err) && (!lbr_st))
		_fsm.FindBr(str);
	if (err) return;
	while ((!err) && (!rbr_st))
		_fsm.FindBr(str);
	if (err) return;
	_fsm.FindCol(str);
}
