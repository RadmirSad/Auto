#include "Token.h"

TNode* TNode::get_fir_leaf()
{
	if (leafs.size())
		return leafs[0];
	else return nullptr;
}

TNode* TNode::get_sec_leaf()
{
	if (leafs.size() > 1)
		return leafs[1];
	else return nullptr;
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
	leaf->change_par(par);
}

TNode* TNode::copy_tree(TNode* node, TNode* new_par) {
	if (node) {
		Token* dop = node->get_saved_token(), * new_token = nullptr;
		type tp = dop->get_type();
		if (tp == STR)
			new_token = new Token(dop->get_str());
		else new_token = new Token(tp);
		TNode* new_tree = nullptr;
		create_n_save(new_tree, new_token);
		create_links(new_par, new_tree);
		for (int i = 0; i < node->leafs.size(); i++)
			copy_tree(node->leafs[i], new_tree);
		return new_tree;
	}
	else return nullptr;
}