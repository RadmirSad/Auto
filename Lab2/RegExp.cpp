#include "RegExp.h"

void Regul::adding_nonempty_string(int first, int second) {
	int buf = second - first;
	if (buf) {
		Token* dop = new Token(expr.substr(first, buf));
		tokens.push_back(dop);
	}
}

void Regul::add_token(int& fir_ind, int& sec_ind, type tp) {
	adding_nonempty_string(fir_ind, sec_ind);
	fir_ind = sec_ind + 1;
	Token* dop = new Token(tp);
	tokens.push_back(dop);
}

void Regul::add_token(type tp, int ind) {
	Token* dop = new Token(tp);
	if(ind == -1)
		tokens.push_back(dop);
	else {
		auto it = tokens.cbegin() + ind + 1;
		tokens.insert(it, dop);
	}
}

bool Regul::add_token(const std::string& str, int ind) {
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

void Regul::connect(int ind, int count) {
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

bool Regul::subdiv_on_symb(int ind_of_token) {
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


bool Regul::match(const std::string& expr, const std::string& regexpr) {
	compile(regexpr);
	return find_way(expr);
}

bool Regul::match(const std::string& expr) {
	if (!_st)
	{
		std::cout << "The corresponding automaton was not built" << std::endl;
		return false;
	}
	return find_way(expr);
}

void Regul::compile(const std::string& dop)
{ 
	if (_st) {
		del_Auto(_st);
		_st = nullptr;
		_end_states.clear();
		_alphabet.clear();
	}
	expr = dop;
	setup();
}

void Regul::setup() {
	tokenize();
	join_strings();
	add_strs_for_oper();
	TTree root;
	build_tree(root);
	tokens.clear();	
	std::string str;
	TNode* this_tree = root.get_node();
	make_reps(this_tree);
	get_tree_on_pict(root, str);
	Automata* start = nullptr, * end = nullptr;
	int id = 1;
	build_NFA(this_tree, start, end, id);
	end->change_receive(true);
	del_tree_with_tokens(this_tree);
	root.change_node(nullptr);
	get_graph(start, str, 1);
	build_DFA(start);
	get_graph(start, str, 2);
	min_DFA(start);
	_st = start;
	get_graph(start, str, 3);
}

void Regul::tokenize() {
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

void Regul::join_strings() {
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

void Regul::add_strs_for_oper() {
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

std::vector<Token*> Regul::slice_vector(const std::vector<Token*>& str, int from_included, int to_NI) {
	std::vector<Token*> buf;
	if (from_included == to_NI)
		throw std::invalid_argument("There should be at least one symbol in the brackets");
	auto fir_iter = str.cbegin() + from_included;		// (a...e): fir -> 'a', sec -> ')'
	auto sec_iter = str.cbegin() + to_NI;
	for (; fir_iter != sec_iter; fir_iter++)
		buf.push_back(*fir_iter);
	return buf;
}

void Regul::chg_range_on_one_token(std::vector<Token*>& str, int from_included, int to_included, TNode* new_root) {
	delete str[from_included];
	str[from_included] = new Token(new_root);
	auto fir_iter = str.cbegin() + from_included + 1, sec_iter = str.cbegin() + to_included + 1;
	tokens.erase(fir_iter, sec_iter);
}

void Regul::build_tree(TTree& root) {
	int fir = 0, sec = 0;
	while (sec < tokens.size())
	{
		type dop = tokens[sec]->get_type();
		if (dop == L_RBR)
			fir = sec;
		if (dop == R_RBR) {
			if (tokens[fir]->get_type() != L_RBR)
				throw std::invalid_argument("Not enough brackets");
			std::vector<Token*> buf = slice_vector(tokens, fir + 1, sec);
			auto new_root = get_subtree(buf);
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

void Regul::get_subtree_from_token(TNode*& fir) {
	Token* buf = fir->get_saved_token();
	TNode* dop = fir;
	fir = buf->get_subtr();
	delete dop;
}

void Regul::get_cl_or_opt(TNode*& oper, TNode*& fir) {
	if (!fir)
		throw std::invalid_argument("First node was empty!!!");
	create_links(oper, fir);
	fir = oper;
}

void Regul::get_repeats(TNode*& oper, TNode*& fir, const std::vector<Token*>& str, int& ind, int sz_of_str) {
	if (!fir)
		throw std::invalid_argument("First node was empty!!!");
	ind++;
	if ((ind + 1 < sz_of_str) && (str[ind + 1]->get_type() == R_CBR) && (str[ind]->get_type() == STR)) {
		std::string dop = str[ind]->get_str();
		for (int i = 0; i < dop.length(); i++)
			if ((dop[i] < 48) || (dop[i] > 57))
				throw std::invalid_argument("This is not a correct number");
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

TNode* Regul::get_subtree(const std::vector<Token*>& str) {
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
							oper_dop_node->save_token(nullptr);
							delete oper_dop_node;
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


std::string Regul::get_type_in_str(type tp) {
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

std::string Regul::get_name_of_node(TNode* this_root) {
	Token* dop = this_root->get_saved_token();
	type tp = dop->get_type();
	std::string adder(get_type_in_str(tp));
	if (adder == "REP")
		adder = "<" + dop->get_str() + ">";
	if (adder == "STR")
		adder = dop->get_str();
	return adder;
}

void Regul::width_byp(TNode* root, std::string& str_out, int ind) {
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

void Regul::get_tree_on_pict(TTree& tree, std::string& for_tests) {
	TNode* root = tree.get_node();
	std::string str = "digraph ast {";
	int ind = 1;
	width_byp(root, str, ind);
	str += "}";
	Agraph_t* Graph = agmemread(str.c_str());
	GVC_t* gvc = gvContext();
	gvLayout(gvc, Graph, "dot");
	int res = gvRenderFilename(gvc, Graph, "jpeg", "out_tree.jpg");
	agclose(Graph);
	for_tests = str;
}

void Regul::make_reps(TNode* root) {
	TNode* left = root->get_fir_leaf(), *right = nullptr, *oper_node = nullptr, * new_tree = nullptr;
	if (left) {
		make_reps(left);
		Token* buf = root->get_saved_token();
		if (buf->get_type() == REP) {
			int count = 0;
			std::stringstream str(buf->get_str());
			str >> count;
			count--;
			TNode* dop_left = left;
			for (int i = 0; i < count; i++) {
				new_tree = root->copy_tree(left);
				if (i != count - 1) {
					buf = new Token(CONCAT);
					create_n_save(oper_node, buf);
					create_links(oper_node, dop_left);
					create_links(oper_node, new_tree);
					dop_left = oper_node;
				}
				else {
					root->change_leaf(0, dop_left);
					create_links(root, new_tree);
					buf = root->get_saved_token();
					buf->change_str("");
					buf->change_type(CONCAT);
				}
				new_tree = nullptr;
			}
		}
		else {
			right = root->get_sec_leaf();
			if(right)
				make_reps(right);
		}
	}
}

void Regul::build_NFA(TNode* root, Automata*& get_start, Automata*& get_end, int& id) {
	TNode* left = root->get_fir_leaf();
	Automata* start_left = nullptr, * end_left = nullptr, * start_right = nullptr,
		* end_right = nullptr,* start = nullptr, * end = nullptr;
	if (left) {
		build_NFA(left, start_left, end_left, id);
		TNode* right = root->get_sec_leaf();
		if (right)
			build_NFA(right, start_right, end_right, id);
	}
	Token* dop = root->get_saved_token();
	std::string buf;
	switch (dop->get_type())
	{
	case STR:
	{
		buf = dop->get_str();
		int sz_str = buf.size();
		Automata* add_start = new Automata(id++);
		start = add_start;
		for (int ind_str = 0; ind_str < sz_str; ind_str++) {
			Automata* add_end = new Automata(id++);

			add_start->add_transit(add_end, std::string(1, buf[ind_str]));

			add_end->add_prev(add_start, std::string(1, buf[ind_str]));
			if (ind_str + 1 == sz_str)
				end = add_end;
			add_start = add_end;
		}
	}
		break;
	case OR:
		start = new Automata(id++), end = new Automata(id++);
		start->add_transit(start_left, "");
		start->add_transit(start_right, "");
		end_left->add_transit(end, "");
		end_right->add_transit(end, "");

		start_left->add_prev(start, "");
		start_right->add_prev(start, "");
		end->add_prev(end_left, "");
		end->add_prev(end_right, "");
		break;
	case OPT:
	case CL:
		start = new Automata(id++), end = new Automata(id++);
		start->add_transit(end, "");
		start->add_transit(start_left, "");
		end_left->add_transit(end, "");

		end->add_prev(start, "");
		start_left->add_prev(start, "");
		end->add_prev(end_left, "");
		if (dop->get_type() == CL) {
			end_left->add_transit(start_left, "");

			start_left->add_prev(end_left, "");
		}
		break;
	case CONCAT:
		start = start_left;
		end = end_right;
		int sz = start_right->get_size_trans();
		for (int i = 0; i < sz; i++)
		{
			auto next = start_right->get_transition(i);
			end_left->add_transit(next);

			next.first->delete_prev(start_right);
			next.first->add_prev(end_left, next.second);
		}
		delete start_right;
		break;
	}
	if (!(get_start) && !(get_end)) {
		get_start = start; get_end = end;
	}
	else throw std::invalid_argument("Incorrect usage. Variables 'get_start' and 'get_end' weren't empty.\n"
		" They should be empty, and then they will have new values of pointers");
}

void Regul::del_tree_with_tokens(TNode*& root) {
	Token* my_tok = root->get_saved_token();
	root->save_token(nullptr);
	delete my_tok;
	TNode* left = root->get_fir_leaf(), * right = nullptr;
	if (left) {
		del_tree_with_tokens(left);
		right = root->get_sec_leaf();
		if (right) {
			del_tree_with_tokens(right);
			root->change_leaf(1, nullptr);
		}
		root->change_leaf(0, nullptr);
	}
	delete root;
}

void Regul::bypass_graph(Automata* start, std::vector<int>& IDs, std::string& descript) {
	int my_id = start->get_id();
	bool is_marked = false;
	for (auto it : IDs)
		if (my_id == it) {
			is_marked = true;
			break;
		}
	if(!is_marked) {
		if (start->get_receive())
			descript += std::to_string(my_id) + " [shape = doublecircle];";
		int num_of_next = start->get_size_trans();
		IDs.push_back(my_id);
		for (int i = 0; i < num_of_next; i++) {
			auto pr = start->get_transition(i);
			Automata* next = pr.first;
			if (!(pr.second.empty())) {
				bool is_added = false;
				for (auto it : _alphabet)
					if (it == pr.second) {
						is_added = true;
						break;
					}
				if (!is_added)
					_alphabet.push_back(pr.second);
			}
			descript += std::to_string(my_id) + "->" + std::to_string(next->get_id());
			if (!(pr.second.empty())) {
				descript += " [label = \"" + pr.second + "\"]";
			}
			descript += ";";
			bypass_graph(next, IDs, descript);
		}
	}
}

void Regul::get_graph(Automata* start, std::string& for_tests, int ind) {
	std::string str = "digraph ast { rankdir=LR;";
	std::vector<int> IDs;
	str += std::to_string(start->get_id()) + " [shape = square];";
	bypass_graph(start, IDs, str);
	str += "}";
	if (str.size() < 5000) {
		Agraph_t* Graph = agmemread(str.c_str());
		GVC_t* gvc = gvContext();
		gvLayout(gvc, Graph, "dot");
		std::string name_of_file = "out_graph" + std::to_string(ind) + ".jpg";
		int res = gvRenderFilename(gvc, Graph, "jpeg", name_of_file.c_str());
		agclose(Graph);
	}
	for_tests = str;
}

void Regul::eps_closure(std::vector<Automata*>& set)
{
	for (int ind_state = 0; ind_state < set.size(); ind_state++) {
		auto state = set[ind_state];
		int sz_transit = state->get_size_trans();
		for (int ind_trans = 0; ind_trans < sz_transit; ind_trans++) {
			auto pair_next_state = state->get_transition(ind_trans);
			if (pair_next_state.second.empty())
			{
				bool is_added = false;
				int set_sz = set.size();
				auto ptr = pair_next_state.first;
				if (!ptr)
					throw std::invalid_argument("Inexpected value of pointer.");
				for (int i = 0; i < set_sz; i++)
					if (set[i] == ptr) {
						is_added = true;
						break;
					}
				if (!is_added)
					set.push_back(ptr);
			}
		}
	}
}

void Regul::move(std::vector<Automata*>& set, const std::string& letter) {
	std::vector<Automata*> new_set;
	int sz = set.size();
	for (int ind_state = 0; ind_state < sz; ind_state++)
	{
		auto state = set[ind_state];
		int sz_transit = state->get_size_trans();
		for (int ind_trans = 0; ind_trans < sz_transit; ind_trans++) {
			auto pair_next_state = state->get_transition(ind_trans);
			if (pair_next_state.second == letter)
			{
				bool is_added = false;
				int new_sz = new_set.size();
				auto ptr = pair_next_state.first;
				if (!ptr)
					throw std::invalid_argument("Inexpected value of pointer.");
				for (auto it : new_set)
					if (it == ptr) {
						is_added = true;
						break;
					}
				if (!is_added)
					new_set.push_back(ptr);
			}
		}
	}
	set = new_set;
}

template <typename T>
bool Regul::equal_sets(const std::vector<T>& first, const std::vector<T>& second)
{
	int sz_fir = first.size(), sz_sec = second.size();
	if (sz_fir == sz_sec) {
		for (int fir_ind = 0; fir_ind < sz_fir; fir_ind++) {
			bool is_found = false;
			for (int sec_ind = 0; sec_ind < sz_sec; sec_ind++)
				if (first[fir_ind] == second[sec_ind]) {
					is_found = true;
					break;
				}
			if (!is_found)
				return false;
		}
		return true;
	}
	else return false;
}

bool check_receive(const std::vector<Automata*>& set) {
	int sz = set.size();
	for (int ind = 0; ind < sz; ind++)
		if (set[ind]->get_receive())
			return true;
	return false;
}

void Regul::del_Auto(Automata*& start) {
	for (int ind_trans = 0; ind_trans < start->get_size_trans(); ind_trans++) {
		auto next = start->get_transition(ind_trans);
		int sz_prev_next = next.first->get_size_prev();
		for (int ind_prev = 0; ind_prev < sz_prev_next; ind_prev++) {
			auto prev_for_next = next.first->get_prev(ind_prev);
			// if(prev_for_next.first->get_id() != start->get_id())
			prev_for_next.first->delete_transit(next.first);
		}
		del_Auto(next.first);
	}
	delete start;
}

void Regul::build_DFA(Automata*& start) {
	int id = 1;
	Automata* new_start = nullptr, * new_end = nullptr, *prev_state = nullptr;
	std::vector<Automata*> addit_set{ start };
	eps_closure(addit_set);															// First states
	std::vector< std::pair<Automata*, std::vector <Automata*> > > marked_states;
	prev_state = new Automata(id++);
	new_start = prev_state;
	if (check_receive(addit_set))
		prev_state->change_receive(true);
	marked_states.push_back({prev_state, addit_set});
	for (int ind_new_states = 0; ind_new_states < marked_states.size(); ind_new_states++) {
		auto newst_N_related_st = marked_states[ind_new_states];
		prev_state = newst_N_related_st.first;
		for (auto letter : _alphabet) {
			if (letter.size() > 1)
				throw std::length_error("Unexpected length of letter");
			addit_set = newst_N_related_st.second;
			move(addit_set, letter);
			eps_closure(addit_set);
			bool is_added = false;
			int ind_of_added = 0, sz_marked = marked_states.size();
			for (; ind_of_added < sz_marked; ind_of_added++)
				if (equal_sets(marked_states[ind_of_added].second, addit_set)) {
					is_added = true;
					break;
				}
			Automata* next_state = nullptr;
			if (is_added)
				next_state = marked_states[ind_of_added].first;
			else
				next_state = new Automata(id++);
			prev_state->add_transit({ next_state, letter });
			next_state->add_prev({ prev_state, letter });
			if (check_receive(addit_set))
				next_state->change_receive(true);
			if(!is_added)
				marked_states.push_back({ next_state, addit_set });
		}
	}
	del_Auto(start);
	start = new_start;
}

void Regul::make_start_groups(Automata* start, std::vector<std::vector<Automata*>>& groups) {
	int ind_for_search = 0;
	if (start->get_receive())
		ind_for_search = 1;
	int sz = groups[ind_for_search].size();
	bool is_find = false;
	for (int i = 0; i < sz; i++)
		if (start == groups[ind_for_search][i])
			return;
	groups[ind_for_search].push_back(start);
	int links = start->get_size_trans();
	for (int i = 0; i < links; i++)
		make_start_groups(start->get_transition(i).first, groups);
}

void Regul::get_next_states(const std::vector<Automata*>& group, std::vector<Automata*>& ns, const std::string& letter) {
	for (auto state : group) {
		auto next = state->get_next_state(letter);
		ns.push_back(next);
	}
}

bool Regul::is_new(int ind, const std::vector<int>& IDs) {
	for (auto id : IDs)
		if (ind == id)
			return false;
	return true;
}

void Regul::get_ids(const std::vector<Automata*>& NS, const std::vector<std::vector<Automata*>>& split,
	std::vector<int>& IDs, std::map<int, int>& unique_ids) {
	int sz_split = split.size(), id = 0;
	for (auto ptr : NS)
		for (int ind_of_group = 0; ind_of_group < sz_split; ind_of_group++) {
			int sz_group = split[ind_of_group].size();
			for(int i = 0; i < sz_group; i++)
				if (ptr == split[ind_of_group][i]) {
					if (is_new(ind_of_group, IDs))
						unique_ids.insert({ ind_of_group, id++ });
					IDs.push_back(ind_of_group);
					break;
				}
		}
}

void Regul::build_new_groups(std::vector<std::vector<Automata*>>& split) {
	std::vector<std::vector<Automata*>> new_split;
	int size = split.size(), ind = 0;
	for (; ind < size; ind++) {
		auto group = split[ind];
		int group_sz = group.size();
		if (group_sz == 1) {
			new_split.push_back(group);
			continue;
		}
		bool is_diff = false;
		for (auto letter : _alphabet) {
			std::vector<Automata*> next_states;
			get_next_states(group, next_states, letter);
			std::vector<int> IDs;
			std::map<int, int> unique_ids;
			get_ids(next_states, split, IDs, unique_ids);
			if (group_sz != IDs.size())
				throw std::invalid_argument("Unexpected situation: some state didn't make any id");
			if (unique_ids.size() == 1)
				continue;
			is_diff = true;
			std::vector<Automata*> adder;
			int prev_size = new_split.size(), index_of_state = 0;
			new_split.insert(new_split.cend(), unique_ids.size(), adder);
			for (auto it: group) {
				int new_id = IDs[index_of_state];
				auto search = unique_ids.find(new_id);
				new_id = search->second;
				new_split[new_id + prev_size].push_back(it);
				index_of_state++;
			}
			ind++;
			while (ind != size) {
				new_split.push_back(split[ind]);
				ind++;
			}
			break;
		}
		if (!is_diff)
			new_split.push_back(group);
	}
	split = new_split;
}

int Regul::get_ind_start(Automata* start, const std::vector<std::vector<Automata*>>& split) {
	int sz = split.size(), sz_group = 0;
	for (int ind = 0; ind < sz; ind++) {
		auto group = split[ind];
		sz_group = group.size();
		for (int i = 0; i < sz_group; i++)
			if (group[i] == start)
				return ind;
	}
	throw std::out_of_range("Start state didn't found");
}

void Regul::min_DFA(Automata*& start) {
	std::vector<std::vector<Automata*>> split;
	std::vector<Automata*> adder;
	split.push_back(adder); split.push_back(adder);
	make_start_groups(start, split);
	int new_size = 0, prev_size = 0;
	do {
		prev_size = split.size();
		build_new_groups(split);
		new_size = split.size();
	} while (new_size != prev_size);
	std::vector<Automata*> new_states;
	Automata* ptr = nullptr, * new_start = nullptr;
	int ind_of_start = get_ind_start(start, split);
	for (int i = 1; i <= new_size; ) {
		ptr = new Automata(i);
		new_states.push_back(ptr);
		i++;
	}
	for (int ind = 0; ind < new_size; ind++) {
		auto group = split[ind];
		auto elem = group.begin();
		if ((*elem)->get_receive()) {
			new_states[ind]->change_receive(true);
			_end_states.push_back(new_states[ind]);
		}
		for (auto letter : _alphabet) {
			auto next = (*elem)->get_next_state(letter);
			bool is_find = false;
			for (int i = 0; i < new_size; i++) {
				int sz_group = split[i].size();
				for(int j = 0; j < sz_group; j++)
					if (split[i][j] == next) {
						is_find = true;
						new_states[ind]->add_transit(new_states[i], letter);
						new_states[i]->add_prev(new_states[ind], letter);
						break;
					}
			}
			if (!is_find)
				throw std::invalid_argument("Unexpected situation. Pointer wasn't in any of groups.");
		}
	}
	del_Auto(start);
	start = new_states[ind_of_start];
}

bool Regul::find_way(const std::string& str) {
	Automata* start_ptr = _st;
	int sz_str = str.size();
	for (int i = 0; i < sz_str; i++) {
		auto next = start_ptr->get_next_state(std::string(1, str[i]));
		if (!next)
			return false;
		start_ptr = next;
	}
	return start_ptr->get_receive();
}

std::string Regul::get_one_trans(Automata* start, Automata* end) {
	int sz_trans = start->get_size_trans();
	for (int i = 0; i < sz_trans; i++) {
		auto next = start->get_transition(i);
		if (next.first == end) {
			if(next.second.size() == 1) {
				if(is_meta(next.second))
					next.second = "%" + next.second + "%";
			}
			return next.second;
		}
	}
	return "";
}

std::string Regul::get_loops(Automata* state) {
	std::string letters = get_one_trans(state, state);
	if (letters.empty())
		return "";
	if (letters.size() != 1)
		letters = "(" + letters + ")";
	letters += "...";
	return letters;
}

void Regul::combine_condits(Automata* start, Automata* end) {
	int sz_trans = start->get_size_trans(), find = 0;
	while (find < sz_trans) {
		if (start->get_transition(find).first == end)
			break;
		find++;
	}
	if (find == sz_trans)
		return;
	std::string find_str = start->get_transition(find).second;
	if (find_str.size() == 1)
		if (is_meta(find_str))
			start->get_transition(find).second = "%" + find_str + "%";
	for (int i = sz_trans - 1; i != find; i--) {
		auto next = start->get_transition(i);
		if (next.first == end) {
			std::string dop = next.second;
			if (next.second.size() == 1)
				if (is_meta(next.second))
					next.second = "%" + next.second + "%";
			std::string add = start->get_transition(find).second + "|" + next.second;
			start->change_condit_trans(add, end);
			end->change_condit_prev(add, start);
			start->delete_transit(end, dop);
			end->delete_prev(start, dop);
		}
	}
}

std::string Regul::restore_regexpr() {
	if (!_st) {
		std::cout << "To restore a regular expression there must be an automaton" << std::endl;
		return std::string("");
	}
	if (_end_states.empty()) {
		std::cout << "There are no receptive states" << std::endl;
		return std::string("");
	}
	exclude_other_states(_st, _end_states);
	combine_condits(_st, _st);
	int sz = _end_states.size();
	std::string restored_reg, adder;
	for (int ind_of_end = 0; ind_of_end < sz; ind_of_end++)
		if(_st != _end_states[ind_of_end]) {
			combine_condits(_st, _end_states[ind_of_end]);
			combine_condits(_end_states[ind_of_end], _st);
			combine_condits(_end_states[ind_of_end], _end_states[ind_of_end]);
		}
	for (int ind_of_end = 0; ind_of_end < sz; ind_of_end++) {
		Automata* new_start = nullptr;
		std::vector<Automata*> new_end;
		get_little_copy(_st, _end_states, new_start, new_end);
		for (int del = 0; del < sz; del++)
			if (del != ind_of_end)
				new_end[del]->change_receive(false);
		new_end.erase(new_end.cbegin() + ind_of_end + 1, new_end.cend());
		if(ind_of_end)
			new_end.erase(new_end.cbegin(), new_end.cbegin() + ind_of_end);
		exclude_other_states(new_start, new_end);
		if (new_start == new_end[0])
		{
			adder = get_loops(new_start);
			delete new_start;
		}
		else {
			adder = "(";
			std::string dop;
			adder += get_loops(new_start);
			dop = get_one_trans(new_end[0], new_start);
			if (dop.empty()) {
				adder = adder.substr(1, adder.size() - 1);
				adder += "(" + get_one_trans(new_start, new_end[0]) + ")";
				adder += get_loops(new_end[0]);
			}
			else {
				int search = adder.find("...", 0);
				if (search != -1)
					adder.erase(search, 3);
				if (adder.size() != 1)
					adder += "|";
				adder += "(" + get_one_trans(new_start, new_end[0]) + ")";
				adder += get_loops(new_end[0]);
				adder += "(" + dop + ")" + ")...";
				adder += "(" + get_one_trans(new_start, new_end[0]) + ")";
				adder += get_loops(new_end[0]);
			}
			if (new_start->get_receive())
				delete new_start;
			for (auto it : new_end)
				delete it;
		}
		if (restored_reg.empty())
			restored_reg = adder;
		else restored_reg += "|" + adder;
	}
	del_Auto(_st);
	_st = nullptr;
	_end_states.clear();
	return restored_reg;
}

void Regul::get_little_copy(Automata* start, std::vector<Automata*> end, Automata*& new_start, std::vector<Automata*>& new_end)
{
	int sz_end = end.size(), id = 1;
	for (int i = 0; i < sz_end; i++) {
		Automata* add = new Automata(id++);
		add->change_receive(true);
		new_end.push_back(add);
		if (end[i] == start)
			new_start = add;
	}
	if (!new_start)
		new_start = new Automata(id++);
	for (int ind_of_end = 0; ind_of_end < sz_end; ind_of_end++) {
		int sz_trans = end[ind_of_end]->get_size_trans();
		for (int ind_trans = 0; ind_trans < sz_trans; ind_trans++) {
			auto trans = end[ind_of_end]->get_transition(ind_trans);
			bool is_transited = false;
			for(int sec_ind = 0; sec_ind < sz_end; sec_ind++)
				if (trans.first == end[sec_ind]) {
					is_transited = true;
					new_end[ind_of_end]->add_transit(new_end[sec_ind], trans.second);
					new_end[sec_ind]->add_prev(new_end[ind_of_end], trans.second);
					break;
				}
			if (!is_transited) {
				if (trans.first == start) {
					new_end[ind_of_end]->add_transit(new_start, trans.second);
					new_start->add_prev(new_end[ind_of_end], trans.second);
				}
				else throw std::invalid_argument("Unexpected situation.");
			}
		}
	}
	if (!(start->get_receive())) {
		int sz_trans = start->get_size_trans();
		for (int ind_trans = 0; ind_trans < sz_trans; ind_trans++) {
			auto trans = start->get_transition(ind_trans);
			bool is_transited = false;
			for (int sec_ind = 0; sec_ind < sz_end; sec_ind++)
				if (trans.first == end[sec_ind]) {
					is_transited = true;
					new_start->add_transit(new_end[sec_ind], trans.second);
					new_end[sec_ind]->add_prev(new_start, trans.second);
					break;
				}
			if (!is_transited) {
				if (trans.first == start) {
					new_start->add_transit(new_start, trans.second);
					new_start->add_prev(new_start, trans.second);
				}
				else throw std::invalid_argument("Unexpected situation.");
			}
		}
	}
}

bool Regul::is_meta(const std::string& letter) {
	switch (letter[0]) {
	case '|': case '?': case '%': case '{': case '}': case '(': case ')':
		return true;
		break;
	default:
		return false;
		break;
	}
}

void Regul::exclude_other_states(Automata* start, std::vector<Automata*> end) {
	while (true) {
		Automata* del_state = find_redundant_state(start, end);
		if (!del_state)
			break;
		combine_condits(del_state, del_state);
		int sz_prev = del_state->get_size_prev(), sz_next = del_state->get_size_trans();
		for (int ind_prev = sz_prev - 1; ind_prev >= 0; ind_prev--) {
			auto prev_trans = del_state->get_prev(ind_prev);
			if (prev_trans.first == del_state)
				continue;
			std::string new_condit = prev_trans.second;
			if (new_condit.size() == 1)
				if (is_meta(new_condit))
					new_condit = "%" + new_condit + "%";
			new_condit += get_loops(del_state);
			for (int ind_next = sz_next - 1; ind_next >= 0; ind_next--) {
				std::string adder = new_condit, second_cond;
				auto next_trans = del_state->get_transition(ind_next);
				if (next_trans.first == del_state)
					continue;
				second_cond = next_trans.second;
				if (second_cond.size() == 1)
					if (is_meta(second_cond))
						second_cond = "%" + second_cond + "%";
				adder += second_cond;
				bool is_find = false;
				int sz_prev_trans = prev_trans.first->get_size_trans(), ind_of_next = 0;
				for (ind_of_next; ind_of_next < sz_prev_trans; ind_of_next++) {
					auto prev_to_next = prev_trans.first->get_transition(ind_of_next);
					if (prev_to_next.first == next_trans.first) {
						is_find = true;
						if (prev_to_next.second.size() == 1)
							if (is_meta(prev_to_next.second))
								prev_to_next.second = "%" + prev_to_next.second + "%";
						prev_trans.first->change_condit_trans(prev_to_next.second + "|" + adder, next_trans.first);
						next_trans.first->change_condit_prev(prev_to_next.second + "|" + adder, prev_trans.first);
						break;
					}
				}
				if (!is_find) {
					prev_trans.first->add_transit({ next_trans.first, adder });
					next_trans.first->add_prev({ prev_trans.first, adder });
				}
			}
			if(prev_trans.first != del_state)
				prev_trans.first->delete_transit(del_state, prev_trans.second);
		}
		sz_next = del_state->get_size_trans();
		for (int i = sz_next - 1; i >= 0; i--) {
			auto next_trans = del_state->get_transition(i);
			next_trans.first->delete_prev(del_state, next_trans.second);
		}
		delete del_state;
	}
}

Automata* Regul::find_redundant_state(Automata* start, std::vector<Automata*> end) {
	Automata* state = nullptr;
	int sz = start->get_size_trans();
	for (int ind_st = 0; ind_st < sz; ind_st++) {
		auto buf = start->get_transition(ind_st);
		state = buf.first;
		if (state != start) {
			bool is_end = false;
			for (auto it : end)
				if (it == state) {
					is_end = true;
					break;
				}
			if (!is_end)
				return state;
		}
	}
	for (auto it : end) {
		sz = it->get_size_trans();
		for (int ind_st = 0; ind_st < sz; ind_st++) {
			auto buf = it->get_transition(ind_st);
			state = buf.first;
			if (state != start) {
				bool is_end = false;
				for (auto it : end)
					if (it == state) {
						is_end = true;
						break;
					}
				if (!is_end)
					return state;
			}
		}
	}
	return nullptr;
}

void Regul::lang_cross(const Regul& second_lang, Regul& result) {
	if (result._st) {
		del_Auto(result._st);
		result._st = nullptr;
		result._end_states.clear();
	}
	Automata* start = nullptr;
	std::vector<Automata*> end;
	if (equal_sets(_alphabet, second_lang._alphabet))
		cart_product(second_lang, start, end);
	else {
		std::cout << "Alphabets weren't equal." << std::endl;
		return;
	}
	result._st = start;
	result._end_states = end;
	result._alphabet = _alphabet;
	result.expr = result.restore_regexpr();
}

void Regul::cart_product(const Regul& second_lang, Automata*& new_start, std::vector<Automata*>& new_end) const {
	Automata* sec_st = second_lang._st;
	std::vector<Automata*> sec_ends = second_lang._end_states, all_st_fir, all_st_sec;
	get_all_states(_st, all_st_fir);
	get_all_states(sec_st, all_st_sec);
	int sz_fir_st = all_st_fir.size(), sz_sec_st = all_st_sec.size(), id = 1;
	std::vector<std::vector<Automata*>> product_states;
	for (int fir_ind = 0; fir_ind < sz_fir_st; fir_ind++)
	{
		std::vector<Automata*> adder;
		for (int sec_ind = 0; sec_ind < sz_sec_st; sec_ind++) {
			Automata* buf = new Automata(id++);
			adder.push_back(buf);
		}
		product_states.push_back(adder);
	}
	for (int fir_ind = 0; fir_ind < sz_fir_st; fir_ind++)
		for (int sec_ind = 0; sec_ind < sz_sec_st; sec_ind++) {
			for (auto letter : _alphabet) {
				auto fir_next = all_st_fir[fir_ind]->get_next_state(letter), sec_next = all_st_sec[sec_ind]->get_next_state(letter);
				if ((all_st_fir[fir_ind]->get_receive()) && (all_st_sec[sec_ind]->get_receive()))
					if(!product_states[fir_ind][sec_ind]->get_receive()) {
						product_states[fir_ind][sec_ind]->change_receive(true);
						new_end.push_back(product_states[fir_ind][sec_ind]);
					}
				int fir_find = 0, sec_find = 0;
				for (; fir_find < sz_fir_st; fir_find++)
					if (fir_next == all_st_fir[fir_find])
						break;
				if (fir_find == sz_fir_st)
					throw std::out_of_range("First state wasn't found in vector of states");
				for (; sec_find < sz_sec_st; sec_find++)
					if (sec_next == all_st_sec[sec_find])
						break;
				if (sec_find == sz_sec_st)
					throw std::out_of_range("Second state wasn't found in vector of states");
				product_states[fir_ind][sec_ind]->add_transit(product_states[fir_find][sec_find], letter);
				product_states[fir_find][sec_find]->add_prev(product_states[fir_ind][sec_ind], letter);
			}
		}
	new_start = product_states[0][0];
	get_all_states(new_start, all_st_fir);
	int sz_new = all_st_fir.size();
	for (int fir_ind = 0; fir_ind < sz_fir_st; fir_ind++)
		for (int sec_ind = 0; sec_ind < sz_sec_st; sec_ind++) {
			bool is_find = false;
			for (int ind_st = 0; ind_st < sz_new; ind_st++)
				if (all_st_fir[ind_st] == product_states[fir_ind][sec_ind]) {
					is_find = true;
					break;
				}
			if (!is_find) {
				int sz = new_end.size();
				for (int i = sz - 1; i >= 0; i--)
					if (product_states[fir_ind][sec_ind] == new_end[i])
						new_end.erase(new_end.begin() + i);
				sz = product_states[fir_ind][sec_ind]->get_size_trans();
				for (int i = 0; i < sz; i++) {
					auto next = product_states[fir_ind][sec_ind]->get_transition(i);
					next.first->delete_prev(product_states[fir_ind][sec_ind]);
				}
				sz = product_states[fir_ind][sec_ind]->get_size_prev();
				for (int i = 0; i < sz; i++) {
					auto prev = product_states[fir_ind][sec_ind]->get_prev(i);
					prev.first->delete_transit(product_states[fir_ind][sec_ind]);
				}
				delete product_states[fir_ind][sec_ind];
			}
		}
}

void Regul::get_all_states(Automata* start, std::vector<Automata*>& states) const {
	int sz_states = states.size();
	for (int i = 0; i < sz_states; i++)
		if (states[i] == start)
			return;
	states.push_back(start);
	if (!start)
		throw std::invalid_argument("Pointer was 'nullptr'");
	int sz_trans = start->get_size_trans();
	for (int i = 0; i < sz_trans; i++)
		get_all_states(start->get_transition(i).first, states);
}

void Regul::inversion() {
	std::string regexpr = restore_regexpr();
	inverse_regexpr(regexpr);
	compile(regexpr);
}

void Regul::inverse_regexpr(std::string& str) const {
	int fir_ind_brack = 0, fir_ind_or = 0, sec_ind = 0, len = str.length(), dop = 0;
	for (; sec_ind < len; sec_ind++) {
		switch (str[sec_ind]) {
		case '%':
			if (((sec_ind + 2 < len) && (expr[sec_ind + 2] == '%')) ||
				((sec_ind + 4 < len) && (expr.substr(sec_ind + 1, 4) == "...%")))
			{
				sec_ind += 2;
				while (expr[sec_ind] != '%')
					sec_ind++;
			}
			else throw std::invalid_argument("This symbol should be in pair like %c%");
			break;
		case '|':
		{
			std::string adder = str.substr(fir_ind_or, sec_ind - fir_ind_or);
			inverse_str(adder);
			int len_add = adder.length();
			if (len_add != sec_ind - fir_ind_or)
				throw std::length_error("Unexpected situation. Length of reversed string wasn't equal to length of string");
			for (int k = 0; k < len_add; k++)
				str[fir_ind_or + k] = adder[k];
			fir_ind_or = sec_ind + 1;
		}
			break;
		case '(':
			fir_ind_brack = sec_ind + 1; sec_ind++;
			while ((sec_ind < len) && ((str[sec_ind] != ')') || (dop))) {
				if (str[sec_ind] == '(')
					dop++;
				if (str[sec_ind] == ')')
					dop--;
				sec_ind++;
			}
			if (sec_ind < len) {
				std::string add = str.substr(fir_ind_brack, sec_ind - fir_ind_brack);
				inverse_regexpr(add);
				int len_add = add.length();
				if (len_add != sec_ind - fir_ind_brack)
					throw std::length_error("Unexpected situation. Length of reversed string wasn't equal to length of string");
				for (int k = 0; k < len_add; k++)
					str[fir_ind_brack + k] = add[k];
			}
			else
				throw std::invalid_argument("Insufficient number of brackets");
			break;
		case ')':
			throw std::invalid_argument("There shouldn't be unchecked symbol ')'");
			break;
		}
	}
	std::string adder = str.substr(fir_ind_or, sec_ind - fir_ind_or);
	inverse_str(adder);
	int len_add = adder.length();
	if (len_add != sec_ind - fir_ind_or)
		throw std::length_error("Unexpected situation. Length of reversed string wasn't equal to length of string");
	for (int k = 0; k < len_add; k++)
		str[fir_ind_or + k] = adder[k];
}

void Regul::inverse_str(std::string& str) const {
	int len = str.length();
	std::string res(str);
	for (int i = 0, from_end = len - 1; i < len; i++, from_end--) {					// aab...cdc...(bg|ga)|fd	->	(gb|ag)c...dcb...aa|df
		switch (str[i]) {															// (gb|ag)c...dcb...aa
		case '%':
			if ((i + 2 < len) && (str[i + 2] == '%'))
			{
				int count = 0;
				if ((i + 5 < len) && (str[i + 3] == '.') && (str[i + 4] == '.') && (str[i + 5] == '.'))
					count = 5;
				else
					count = 2;
				i += count;
				for (int k = 0; k < count + 1; k++, from_end--)
					res[from_end] = str[i - k];
				from_end++;
			}
			else throw std::invalid_argument("This symbol should be in pair like %c%");
			break;
		case '(':
		{
			int ind_copy = 1, dop = 0, sec_ind = i + 1;
			while ((sec_ind < len) && ((str[sec_ind] != ')') || (dop))) {
				if (str[sec_ind] == '(')
					dop++;
				if (str[sec_ind] == ')')
					dop--;
				sec_ind++;
				ind_copy++;
			}
			if ((sec_ind + 3 < len) && (str[sec_ind + 1] == '.') && (str[sec_ind + 2] == '.') && (str[sec_ind + 3] == '.'))
				ind_copy += 3;
			from_end -= ind_copy;
			for (int k = 0; k <= ind_copy; k++, i++)
				res[from_end + k] = str[i];
			i--;
		}
			break;
		default:
			if ((i + 3 < len) && (str[i + 1] == '.') && (str[i + 2] == '.') && (str[i + 3] == '.')) {
				i += 3;
				for (int k = 0; k < 4; k++, from_end--)
					res[from_end] = str[i - k];
				from_end++;
			}
			else
				res[from_end] = str[i];
			break;
		}
	}
	str = res;
}