#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "CheckClass_sm.h"
#include <iostream>
#include <vector>

class CheckClass {
private:
	CheckClassContext _fsm;
	std::vector<std::string> par;

	bool CheckName(const std::string& name);
	bool CheckMod(const std::string& modif);
	bool CheckFunc(const std::string& funcs);
	bool Comp(int a, int b);
	void AddPar(const std::string& str);
public:
	CheckClass();

	~CheckClass() {};

	bool CheckString(const std::string&);

	void RetStart() { _fsm.Start(); }

	void SayBye();
};
