#pragma once
#include <sstream>
#include <gvc.h>
#include "Token.h"

class regul {
private:
	std::string expr;
	std::vector<Token*> tokens;

	/*		Make life easier		*/	
	void adding_nonempty_string(int first, int second);

	void add_token(type tp, int ind = -1);
	void add_token(int& fir_ind, int& sec_ind, type tp);
	bool add_token(const std::string&, int);

	void join_strings();
	void connect(int, int);
	bool subdiv_on_symb(int);
	void add_strs_for_oper();
	std::vector<Token*> slice_vector(const std::vector<Token*>&, int, int);
	void chg_range_on_one_token(std::vector<Token*>&, int, int, TNode*);

	void get_subtree_from_token(TNode*&);
	void get_cl_or_opt(TNode*&, TNode*&);
	void get_repeats(TNode*&, TNode*&, const std::vector<Token*>&, int&, int);

	const char* get_type_in_str(type);
	std::string get_name_of_node(TNode*);
	void width_byp(TNode*, std::string&, int);

	/*		Important functions		*/
	void setup();
	void tokenize();
	void build_tree(TTree&);
	TNode* get_subtree(const std::vector<Token*>& str);

public:
	regul() {};

	void print_str() { std::cout << expr << std::endl; };
	void get_tree_on_pict(TTree&, std::string&);

	bool match(const std::string&, const std::string&) { return true; }
	bool match(const std::string&, const std::string&, const std::vector<std::string>&) { return true; }

	void compile(const std::string&);
};