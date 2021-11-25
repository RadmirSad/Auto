#include "RegExp.h"

void regul::adding_nonempty_string(int first, int second) {
	int buf = second - first;
	if (buf) {
		Token* dop = new Token(expr.substr(first, buf));
		tokens.push_back(dop);
	}
}

void regul::add_token(int& fir_ind, int& sec_ind, type tp) {
	adding_nonempty_string(fir_ind, sec_ind);
	fir_ind = sec_ind + 1;
	Token* dop = new Token(tp);
	tokens.push_back(dop);
}

void regul::add_token(type tp, int ind) {
	Token* dop = new Token(tp);
	if(ind == -1)
		tokens.push_back(dop);
	else {
		auto it = tokens.cbegin() + ind + 1;
		tokens.insert(it, dop);
	}
}

bool regul::add_token(const std::string& str, int ind) {
	if (str.empty())
		return false;
	Token* dop = new Token(str);
	if (ind == -1)
		tokens.push_back(dop);
	else {
		auto it = tokens.cbegin() + ind + 1;
		tokens.insert(it, dop);
	}
	return true;
}

void regul::connect(int ind, int count) {
	std::string first_str = tokens[ind]->get_str();
	delete tokens[ind];
	Token* dop = nullptr;
	for (int j = 1; j < count; j++) {
		first_str.append(tokens[ind + j]->get_str());
		delete tokens[ind + j];
	}
	Token* new_tok = new Token(first_str);
	tokens[ind] = new_tok;
}

bool regul::subdiv_on_symb(int ind_of_token) {
	std::string dop = tokens[ind_of_token]->get_str(), first, second = "";
	bool flag = 0;
	int len_1 = dop.length() - 1;
	first = dop.substr(0, len_1);
	if (first == "")
		first = dop[len_1];
	else
		second = dop[len_1];
	tokens[ind_of_token]->change_str(first);
	flag = add_token(second, ind_of_token);
	if (flag)
		add_token(CONCAT, ind_of_token);
	return flag;
}


void regul::compile(const std::string& dop)
{ 
	expr = dop;
	setup();
}

void regul::setup() {
	/*
	1. Build tree for building NSM
		a) Check string for tokens
		b) Build tree
	2. Build NSM from tree
	3. NFSM -> DFSM
	4. DFSM -> minimal DFSM

	*/
	tokenize();
	join_strings();
	add_strs_for_oper();
	TTree root;
	build_tree(root);
	std::string str;
	get_tree_on_pict(root, str);
	tokens.clear();
	// почистить память вектора
}

void regul::tokenize() {
	int fir_ind = 0, sec_ind = 0, len = expr.length();
	if (!len)
		throw std::length_error("You didn't enter any regular expression");
	add_token(L_RBR);
	while (sec_ind < len)
	{
		switch (expr[sec_ind]) {
		case '%':
			if (((sec_ind + 2 < len) && (expr[sec_ind + 2] == '%')) || 
				((sec_ind + 4 < len) && (expr.substr(sec_ind + 1, 4) == "...%")))
			{
				adding_nonempty_string(fir_ind, sec_ind);
				fir_ind = sec_ind + 1;
				sec_ind += 2;
				while (expr[sec_ind] != '%')
					sec_ind++;
				adding_nonempty_string(fir_ind, sec_ind);
				fir_ind = sec_ind + 1;
			}
			else throw std::invalid_argument("You can't use only one %, this symbol should be in pair like %c%");
			break;
		case '|':
			add_token(fir_ind, sec_ind, OR);
			break;
		case '.':
			if ((sec_ind + 2 < len) && (expr[sec_ind] == expr[sec_ind + 1]) && (expr[sec_ind] == expr[sec_ind + 2])) {
				add_token(fir_ind, sec_ind, CL);
				sec_ind += 2;
				fir_ind += 2;
			}
			break;
		case '?':
			add_token(fir_ind, sec_ind, OPT);
			break;
		case '{':
			add_token(fir_ind, sec_ind, L_CBR);
			while ((sec_ind < len) && (expr[sec_ind] != '}'))
				sec_ind++;
			if (sec_ind == len)
				throw std::invalid_argument("You can't use only one '{', this symbol should be in pair like {number}");
			add_token(fir_ind, sec_ind, R_CBR);
			break;
		case '(':
			add_token(fir_ind, sec_ind, L_RBR);
			break;
		case ')':
			add_token(fir_ind, sec_ind, R_RBR);
			break;
		}
		sec_ind++;
	}
	adding_nonempty_string(fir_ind, sec_ind);
	add_token(R_RBR);
}

void regul::join_strings() {
	int count_str = 0;
	bool is_str = false;
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i]->get_type() == STR) {
			is_str = true;
			count_str++;
			continue;
		}
		if (is_str) {
			if (count_str > 1) {
				connect(i - count_str, count_str);
				auto fir_iter = tokens.cbegin() + (i - count_str + 1);
				auto sec_iter = tokens.cbegin() + i;
				tokens.erase(fir_iter, sec_iter);
				i -= count_str;
			}
			count_str = 0;
			is_str = false;
		}
	}
	if (count_str > 1) {
		connect(tokens.size() - count_str, count_str);
		auto sec_iter = tokens.cend();
		auto fir_iter = tokens.cend() - count_str + 1;
		tokens.erase(fir_iter, sec_iter);
	}
}

void regul::add_strs_for_oper() {
	bool is_c_br = false;
	for (int i = 0; i < tokens.size(); i++)
		switch (tokens[i]->get_type()) {
		case L_CBR: is_c_br = true;
		case OPT:
		case CL:
			if (tokens[i - 1]->get_type() == STR)
				if (subdiv_on_symb(i - 1))
					i += 2;
		case STR:
		case R_CBR:
		case R_RBR:
			if ((i + 1 < tokens.size()) && (!is_c_br) &&
				((tokens[i + 1]->get_type() == L_RBR) || (tokens[i + 1]->get_type() == STR))) {
				add_token(CONCAT, i);
				i++;
			}
			is_c_br = false;
			break;
		}
}

std::vector<Token*> regul::slice_vector(const std::vector<Token*>& str, int from_included, int to_NI) {
	std::vector<Token*> buf;
	if (from_included == to_NI)
		throw std::invalid_argument("There should be at least one symbol in the brackets");
	auto fir_iter = str.cbegin() + from_included;		// (a...e): fir -> 'a', sec -> ')'
	auto sec_iter = str.cbegin() + to_NI;
	for (; fir_iter != sec_iter; fir_iter++)
		buf.push_back(*fir_iter);
	return buf;
}

void regul::chg_range_on_one_token(std::vector<Token*>& str, int from_included, int to_included, TNode* new_root) {
	delete str[from_included];
	str[from_included] = new Token(new_root);
	auto fir_iter = str.cbegin() + from_included + 1, sec_iter = str.cbegin() + to_included + 1;
	tokens.erase(fir_iter, sec_iter);
}

void regul::build_tree(TTree& root) {
	int fir = 0, sec = 0;
	while (sec < tokens.size())
	{
		type dop = tokens[sec]->get_type();
		if (dop == L_RBR)
			fir = sec;
		if (dop == R_RBR) {
			if (tokens[fir]->get_type() != L_RBR)
				throw std::invalid_argument("Not enough brackets");
			std::vector<Token*> buf = slice_vector(tokens, fir + 1, sec);		// Получили выражение без скобок изнутри
			auto new_root = get_subtree(buf);									// Нужно заменить токены в строчке на один токен с узлом
			chg_range_on_one_token(tokens, fir, sec, new_root);
			sec = fir;
		}
		while ((fir >= 0) && (tokens[fir]->get_type() != L_RBR))
			fir--;
		sec++;
	}
	if (tokens.size() > 1)
		throw std::out_of_range("There are more than 1 token in the vector");
	TNode* buf = tokens[0]->get_subtr();
	root.change_node(buf);
}

void regul::get_subtree_from_token(TNode*& fir) {		// Нужно не забыть почистить память от токенов
	Token* buf = fir->get_saved_token();
	TNode* dop = fir;
	fir = buf->get_subtr();
	delete dop;
}

void regul::get_cl_or_opt(TNode*& oper, TNode*& fir) {
	if (!fir)
		throw std::invalid_argument("First node was empty!!!");
	create_links(oper, fir);
	fir = oper;
}

void regul::get_repeats(TNode*& oper, TNode*& fir, const std::vector<Token*>& str, int& ind, int sz_of_str) {
	if (!fir)
		throw std::invalid_argument("First node was empty!!!");
	ind++;
	if ((ind + 1 < sz_of_str) && (str[ind + 1]->get_type() == R_CBR) && (str[ind]->get_type() == STR)) {
		std::string dop = str[ind]->get_str();
		for (int i = 0; i < dop.length(); i++)
			if ((dop[i] < 48) || (dop[i] > 57))
				throw std::invalid_argument("This is not a number");
		ind++;
		create_links(oper, fir);
		Token* buf = oper->get_saved_token();
		buf->change_str(dop);
		buf->change_type(REP);
		fir = oper;
	}
	else
		throw std::invalid_argument("Incorrect pattern");
}

TNode* regul::get_subtree(const std::vector<Token*>& str) {
	int ind = 0;
	TNode* dop_node = nullptr, *fir_node = nullptr, *sec_node = nullptr, *oper_node = nullptr;
	while (ind < str.size()) {
		int sz = str.size();
		create_n_save(dop_node, str[ind]);
		switch (str[ind]->get_type()) {
		case SUBTR:
			get_subtree_from_token(dop_node);
		case STR:
			if (!fir_node)
				fir_node = dop_node;
			else
				throw std::invalid_argument("This case might be only once");
			break;
		case CL:
		case OPT:
			get_cl_or_opt(dop_node, fir_node);
			break;
		case L_CBR:
			get_repeats(dop_node, fir_node, str, ind, sz);
			break;
		case CONCAT:
			oper_node = dop_node;
			if (!fir_node)
				throw std::invalid_argument("First node was empty!!!");
			create_links(oper_node, fir_node);
			if(ind + 1 == sz)
				throw std::invalid_argument("There is no second token for operation *");
			ind++;
			create_n_save(sec_node, str[ind]);
			switch (str[ind]->get_type()) {
			case SUBTR:
				get_subtree_from_token(sec_node);
			case STR:
				{
					TNode* oper_dop_node = nullptr;
					if (ind + 1 < sz)
					{
						ind++;
						create_n_save(oper_dop_node, str[ind]);
						switch (str[ind]->get_type()) 
						{
						case CL:
						case OPT:
							get_cl_or_opt(oper_dop_node, sec_node);
							break;
						case L_CBR:
							get_repeats(oper_dop_node, sec_node, str, ind, sz);
							break;
						case OR:
						case CONCAT:
							ind--;
							break;
						default:
							throw std::invalid_argument("Incorrect pattern: operation * should be before string or subtree");
							break;
						}
					}
					break;
				}
			default:
				throw std::invalid_argument("Incorrect pattern: operation * should be before string or subtree");
				break;
			}
			create_links(oper_node, sec_node);
			fir_node = oper_node;
			break;
		case OR:
		{
			oper_node = dop_node;
			if (!fir_node)
				throw std::invalid_argument("First node was empty!!!");
			create_links(oper_node, fir_node);
			if (ind + 1 == sz)
				throw std::invalid_argument("There is no second token for operation |");
			ind++;
			int sec_ind = ind;
			for (; sec_ind < sz && str[sec_ind]->get_type() != OR; sec_ind++);
			std::vector<Token*> buf = slice_vector(str, ind, sec_ind);
			sec_node = get_subtree(buf);
			ind = sec_ind - 1;
			create_links(oper_node, sec_node);
			fir_node = oper_node;
		}
			break;
		default:
			throw std::invalid_argument("There is no token with this type");
		}
		ind++;
	}

	return fir_node;
}


const char* regul::get_type_in_str(type tp) {
	switch (tp) {
	case STR:
		return "STR";
	case OR:
		return "|";
	case CL:
		return "...";
	case OPT:
		return "?";
	case CONCAT:
		return "*";
	case REP:
		return "REP";
	default:
		throw std::invalid_argument("Unexpected token");
	}
}

std::string regul::get_name_of_node(TNode* this_root) {
	Token* dop = this_root->get_saved_token();
	type tp = dop->get_type();
	std::string adder(get_type_in_str(tp));
	if (adder == "REP")
		adder = "<" + dop->get_str() + ">";
	if (adder == "STR")
		adder = dop->get_str();
	return adder;
}

void regul::width_byp(TNode* root, std::string& str_out, int ind) {
	TNode* my_node = root->get_fir_leaf();
	if (my_node) {
		std::string adder_par = "\"" + get_name_of_node(root) + "'",
			adder = "\"" + get_name_of_node(my_node) + "'", str_int;
		std::stringstream buf;
		buf << ind;
		buf >> str_int;
		adder_par += str_int + "\"";
		buf.clear();
		buf << ind * 2;
		buf >> str_int;
		adder += str_int + "\"";
		str_out += adder_par + "->" + adder + ";";
		auto sec_node = root->get_sec_leaf();
		if (sec_node) {
			adder = "\"" + get_name_of_node(sec_node) + "'";
			buf.clear();
			buf << (ind * 2 + 1);
			buf >> str_int;
			adder += str_int + "\"";
			str_out += adder_par + "->" + adder + ";";
			width_byp(sec_node, str_out, ind * 2 + 1);
		}
		width_byp(my_node, str_out, ind * 2);
	}
}

void regul::get_tree_on_pict(TTree& tree, std::string& for_tests) {
	TNode* root = tree.get_node();
	std::string str = "digraph ast {";
	int ind = 1;
	width_byp(root, str, ind);
	str += "}";
	Agraph_t* Graph = agmemread(str.c_str());
	GVC_t* gvc = gvContext();
	gvLayout(gvc, Graph, "dot");
	int res = gvRenderFilename(gvc, Graph, "jpeg", "out.jpg");		/*		\\Users\\79192\\Desktop\\Automata_Theory\\LAB2\\ */
	agclose(Graph);
	for_tests = str;
}


