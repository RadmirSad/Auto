#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <map>
#include "CheckClass_sm.h"

enum err_type {
	word_err,
	name_cl,
	colon_err,
	spec_err,
	par_err,
	lbr_err,
	rbr_err,
	fin_col_err,
	ok
};

class CheckClass {
private:
	CheckClassContext _fsm;
	std::map<std::string, std::string> classes;
	std::string my_cl;
	std::string par_cl;
	bool word_st = false, class_st = false, col_st = false, spec_st = false, par_st = false,
		lbr_st = false, rbr_st = false, symbols = false, class_here = false, err = false;
	int fir_ind = 0, sec_ind = 0;
public:
	CheckClass() : _fsm(*this) {};

	void check_str(const std::string&);

	void say_bye();
	void err_msg(int);
	bool symb_was() { return symbols; }
	int get_fir() { return fir_ind; }
	int get_sec() { return sec_ind; }
	void put_fir(int fir) { fir_ind = fir; }
	void put_sec(int sec) { sec_ind = sec; }
	void word_stage() { word_st = true; }
	void up_fir() { fir_ind++; }
	void up_sec() { sec_ind++; }
	void cg_symb(bool fl) { symbols = fl; }
	bool is_end_of_class() { return class_here; }
	void cg_end_of_class(bool fl) { class_here = fl; }
	int is_name_of_class(const std::string&);
	bool is_func(const std::string&);
	void add_my_cl(const std::string& str) { my_cl = str; }
	void add_par_cl(const std::string& par) { if (par.size()) par_cl = par; }
	void cr_new_cls() { classes[my_cl] = par_cl; }
	void class_stage() { class_st = true; }
	void col_stage() { col_st = true; }
	void spec_stage() { spec_st = true; }
	void par_stage() { par_st = true; }
	void lbr_stage() { lbr_st = true; }
	void rbr_stage() { rbr_st = true; }
	void rest_stage() { word_st = false, class_st = false, col_st = false, spec_st = false, par_st = false,
		lbr_st = false, rbr_st = false, symbols = false, class_here = false, err = false; _fsm.enterStartState();  _fsm.Start(); }
	bool is_specif(const std::string& str);
	void ch_err() { err = true; }
};