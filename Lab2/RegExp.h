#pragma once
#include <sstream>
#include <map>
#include <set>
#include <gvc.h>
#include "Token.h"
#include "Auto.h"

class TestRegul;

class Regul {
private:
	std::string expr;
	std::vector<Token*> tokens;
	Automata* _st = nullptr;
	std::vector<std::string> _alphabet;
	std::vector<Automata*> _end_states;

	/*		Important functions		*/
	void setup();
	void tokenize();

	TNode* get_subtree(const std::vector<Token*>& str);
	void build_tree(TTree&);

	void move(std::vector<Automata*>& set, const std::string& letter);
	void eps_closure(std::vector<Automata*>& set);
	void build_NFA(TNode*, Automata*&, Automata*&, int&);

	void build_DFA(Automata*& start);

	void min_DFA(Automata*& start);

	bool find_way(const std::string& str);

	void cart_product(const Regul& second_lang, Automata*& new_start, std::vector<Automata*>& new_end) const;

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

	std::string get_type_in_str(type);
	std::string get_name_of_node(TNode*);
	void width_byp(TNode*, std::string&, int);
	void get_tree_on_pict(TTree&, std::string&);

	void make_reps(TNode* root);
	void del_tree_with_tokens(TNode*& root);
	void get_graph(Automata* start, std::string& for_tests, int ind);
	void bypass_graph(Automata* start, std::vector<int>& IDs, std::string& descript);

	template <typename T>
	bool equal_sets(const std::vector<T>& first, const std::vector<T>& second);
	void del_Auto(Automata*& start);

	void make_start_groups(Automata* start, std::vector<std::vector<Automata*>>& groups);
	void build_new_groups(std::vector<std::vector<Automata*>>& split);
	bool is_new(int ind, const std::vector<int>& IDs);
	void get_ids(const std::vector<Automata*>& ns, const std::vector<std::vector<Automata*>>& split, std::vector<int>& IDs, std::map<int, int>& unique_ids);
	void get_next_states(const std::vector<Automata*>& group, std::vector<Automata*>& ns, const std::string& letter);
	int get_ind_start(Automata* start, const std::vector<std::vector<Automata*>>& split);

	void exclude_other_states(Automata* start, std::vector<Automata*> end);
	Automata* find_redundant_state(Automata* start, std::vector<Automata*> end);
	std::string get_loops(Automata* state);
	bool is_meta(const std::string& letter);
	std::string get_one_trans(Automata* start, Automata* end);
	void combine_condits(Automata* start, Automata* end);
	void get_little_copy(Automata* start, std::vector<Automata*> end, Automata*& new_start, std::vector<Automata*>& new_end);

	void get_all_states(Automata* start, std::vector<Automata*>& states) const;

	void inverse_regexpr(std::string& regexpr) const;
	void inverse_str(std::string& str) const;

public:
	Regul() {};

	std::string get_str() const { return expr; };

	void compile(const std::string&);

	void lang_cross(const Regul& second_lang, Regul& result);

	void inversion();

	std::string restore_regexpr();

	bool match(const std::string& expr, const std::string& regexpr);
	bool match(const std::string& expr);

	friend TestRegul;
};