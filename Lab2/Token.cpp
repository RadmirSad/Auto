#include "Token.h"

TNode* TNode::get_fir_leaf() const
{
	if (leafs.size())
		return leafs[0];
	else return nullptr;
}

TNode* TNode::get_sec_leaf() const
{
	if (leafs.size() > 1)
		return leafs[1];
	else return nullptr;
}

void TNode::change_leaf(int ind, TNode* new_leaf) {
	if (ind == 1) {
		if (new_leaf)
			leafs[ind] = new_leaf;
		else
			leafs.erase(leafs.cbegin() + 1);
	}
	else if (ind == 0) {
		if (new_leaf)
			leafs[ind] = new_leaf;
		else if (leafs.size() == 1)
			leafs.clear();
		else throw std::invalid_argument("Undefined situation.");
	}
	else throw std::invalid_argument("Incorrect index.");
}

void TNode::DestroyNodes() {
	if (this) {
		int sz = leafs.size();
		for (int i = 0; i < sz; i++)
			leafs[i]->DestroyNodes();
		delete this;
	}
}

void create_n_save(TNode*& dop_node, Token* ptr) {
	dop_node = new TNode();
	dop_node->save_token(ptr);
}

void create_links(TNode* par, TNode* leaf) {
	if (par)
		par->add_leaf(leaf);
}

TNode* TNode::copy_tree(TNode* node, TNode* new_par) {
	if (node) {
		Token* dop = node->get_saved_token(), * new_token = nullptr;
		type tp = dop->get_type();
		if (tp == STR)
			new_token = new Token(dop->get_str());
		else {
			new_token = new Token(tp);
			if (tp == REP)
				new_token->change_str(dop->get_str());
		}
		TNode* new_tree = nullptr;
		create_n_save(new_tree, new_token);
		create_links(new_par, new_tree);
		for (int i = 0; i < node->leafs.size(); i++)
			copy_tree(node->leafs[i], new_tree);
		return new_tree;
	}
	else return nullptr;
}