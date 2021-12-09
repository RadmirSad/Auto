#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>

enum type {
	STR,
	OR,
	CL,
	OPT,
	L_RBR,		// Left Round Bracket
	R_RBR,		// Rigth Round Bracket
	L_CBR,		// Left Curly Bracket
	R_CBR,		// Right Curly Bracket
	SUBTR,
	CONCAT,
	REP
};

class Token;
class TNode;
class TTree;

void create_n_save(TNode*& dop_node, Token* ptr);
void create_links(TNode* par, TNode* leaf);


class Token {
private:
	type my_type;
	std::string info;
	TNode* subtr = nullptr;
public:
	Token(TNode* Nd) : subtr(Nd), my_type(SUBTR) {};
	Token(type t) : my_type(t) {
		if ((t == SUBTR) || (t == STR))
			throw std::invalid_argument("For these token types, a parameter of a different type must be passed");
	};
	Token(const std::string& dop): info(dop), my_type(STR) {};
	~Token() { info.clear(); }

	const type get_type() const { return my_type; }
	const std::string& get_str() const { return info; }
	TNode* get_subtr() const { return subtr; }

	void change_str(const std::string& str) { info = str; }
	void change_type(type new_type) { my_type = new_type; }

	friend bool operator == (const Token& fir, const Token& sec) {
		return (fir.my_type == sec.my_type) && (fir.info == sec.info) && (fir.subtr == sec.subtr);
	}
};


class TNode {
protected:
	std::vector<TNode*> leafs;
	Token* tok = nullptr;

public:
	TNode() {};
	~TNode() { leafs.clear(); };
	void save_token(Token* new_tok) { tok = new_tok; }
	Token* get_saved_token() const { return tok; }
	void add_leaf(TNode* new_leaf) { leafs.push_back(new_leaf); }

	void change_leaf(int ind, TNode* new_leaf);

	TNode* get_fir_leaf() const;

	TNode* get_sec_leaf() const;

	void DestroyNodes();

	TNode* copy_tree(TNode* node, TNode* new_par = nullptr);
};

class TTree {
protected:
	TNode* root = nullptr;

	void DestroyTree() {
		root->DestroyNodes();
	}

public:
	TTree() { ; }
	TTree(TNode* new_root) : root(new_root) {};
	~TTree() {
		DestroyTree();
	}

	void change_node(TNode* new_ptr) { root = new_ptr; }

	TNode* get_node() const { return root; }
};