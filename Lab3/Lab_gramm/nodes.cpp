#include "nodes.h"

/*		Methods for tree		*/

int exec(DefaultNode* par) {
	bypassTree(par);
}

DefaultNode* bypassTree(DefaultNode* par) {
	DefaultNode* ptr = nullptr;
	if (!par->check_oper())
		ptr = new DataNode(par);
	else {
		int sz_nodes = par->get_size_vect();
		switch (sz_nodes) {
		case 0:
			ptr = zero_nodes(par);
			break;
		case 1:
			ptr = one_node(par);
			break;
		case 2:
			ptr = two_nodes(par);
			break;
		case 3:
			ptr = three_nodes(par);
			break;
		case 4:
			ptr = four_nodes(par);
			break;
		case 5:
			ptr = five_nodes(par);
			break;
		}
	}
	return ptr;
}

DefaultNode* zero_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* ptr = nullptr;
	par->get_type(tp_action);
	SWITCH (tp_action) {
		CASE("left") :
			my_robot->left();
			break;
		CASE("right") :
			my_robot->right();
			break;
		CASE("exit") :
			ptr = new DataNode(my_robot->exit());
			break;
		CASE("wall") :
			ptr = new DataNode(my_robot->wall());
			break;
	}
	return ptr;
}

DefaultNode* one_node(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* ptr = nullptr,* fir_leaf = nullptr;
	fir_leaf = bypassTree(par->get_node(0));
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("sum") :
			sum(fir_leaf);
			break;
		CASE("transpon") :
			transpon(fir_leaf);
			break;
		CASE("not") :
			break;
		CASE("sl") :
			break;
		CASE("sr") :
			break;
		CASE("move") :
			break;
		CASE("get_var") :
			break;
		CASE("matr_vect") :
			break;
		CASE("e_call") :
			break;
		CASE("const") :
			break;
		CASE("func") :
			break;
	}
	return ptr;
}

void sum(DefaultNode* ptr) {
	int new_sum = 0;
	err code = OK;
	std::string data_tp;
	ptr->get_type(data_tp);
	if (ptr->check_oper())
		yyerror("Incorrect type: it is operation type");
	SWITCH(data_tp) {
		CASE("int") : CASE("bool") :
			yyerror("Incorrect type of data");
			break;
		CASE("v_int") : CASE("v_bool") : {
			int sz = ptr->get_size_vect();
			for (int i = 0; i < sz; i++)
				new_sum += ptr->get_elem(code, i);
		}
			break;
		CASE("m_int") : CASE("m_bool") :
			break;
	}
	if (!ptr->change_info(new_sum, code))
		yyerror("Unexcpected behavior: info wasn't changed");
	ptr->set_type(data_tp.substr(2));
}

void transpon(DefaultNode* matrix) {

}

DefaultNode* two_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* ptr = nullptr;
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("make_m2") :
			break;
		CASE("make_v") :
			break;
		CASE("ch_tp") :
			break;
		CASE("eq") :
			break;
		CASE("log_matr") :
			break;
		CASE("mul") :
			break;
		CASE("el_mul") :
			break;
		CASE("get_str") :
			break;
		CASE("get_col") :
			break;
		CASE("and") :
			break;
		CASE("gt") :
			break;
		CASE("lt") :
			break;
		CASE("-") :
			break;
		CASE("+") :
			break;
		CASE(",") :
			break;
		CASE("e_decl") :
			break;
		CASE("v_e_decl") :
			break;
		CASE("m_e_decl") :
			break;
		CASE("call") :
			break;
		CASE("endl") :
			break;
	}
	return ptr;
}

DefaultNode* three_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* ptr = nullptr;
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("make_m1") :
			break;
		CASE("coord") :
			break;
		CASE("decl") :
			break;
		CASE("v_decl") :
			break;
		CASE("m_decl") :
			break;
		CASE("cr_func4") :
			break;
	}
	return ptr;
}

DefaultNode* four_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* ptr = nullptr;
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("cr_func2") :
			break;
		CASE("cr_func3") :
			break;
	}
	return ptr;
}

DefaultNode* five_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* ptr = nullptr;
	par->get_type(tp_action);
	if (tp_action == "cr_func1") {

	}
	else throw std::logic_error("");
	return ptr;
}

void delTree(DefaultNode*& par) {
	if (par) {
		int sz = par->get_size_vect();
		for (int i = 0; i < sz; i++) {
			auto ptr = par->get_node(i);
			delTree(ptr);
		}
		delete par;
	}
}

void init() {
	read_file();
	init_map_and_robot();
}

void read_file() {

}

void init_map_and_robot() {

}

void yyerror(const std::string& str) {
	std::cerr << str << std::endl;
}

std::string what_type(const std::string& str) {
	const std::string vect_bool = "class std::vector<bool,class std::allocator<bool> >",
		vect_int = "class std::vector<int,class std::allocator<int> >",
		matr_int = "class std::vector<class std::vector<int,class std::allocator<int> >,"
		"class std::allocator<class std::vector<int,class std::allocator<int> > > >",
		matr_bool = "class std::vector<class std::vector<bool,class std::allocator<bool> >,"
		"class std::allocator<class std::vector<bool,class std::allocator<bool> > > >";
	if ((str == "int") || (str == "bool") || 
		(str == "v_int") || (str == "v_bool") ||
		(str == "m_int") || (str == "m_bool"))
		return str;
	if (str == vect_bool)
		return "v_bool";
	if (str == vect_int)
		return "v_int";
	if (str == matr_int)
		return "m_int";
	if (str == matr_bool)
		return "m_bool";
	throw std::invalid_argument("Invalid variable type");
}

/*		Data's nodes	*/

DataNode::DataNode(DefaultNode* old_node) {
	err code = OK;
	val = old_node->get_elem(code);
	if (code != OK)
		yyerror("error");
	is_const = old_node->get_const();
	name = old_node->get_var_name();
	old_node->get_matr(m_val);
	old_node->get_vect(v_val);
}

std::pair<int, int> DataNode::get_size_matr() const {
	std::pair<int, int> add;
	add.first = m_val.size();
	if (add.first)
		add.second = m_val[0].size();
	else add.second = 0;
	return add;
}

int DataNode::get_elem(err& err_code, int ind_str, int ind_col) const {
	if (ind_col == -1) {
		if (ind_str == -1)
			return val;
		int sz = get_size_vect();
		if ((ind_str >= sz) || (ind_str < 0)) {
			err_code = IND_VECT;		//yyerror("Incorrect index of vector");
			return 0;
		}
		return v_val[ind_str];
	}
	auto [sz_str, sz_col] = get_size_matr();
	if ((ind_str >= sz_str) || (ind_str < 0)) {
		err_code = IND_STR_MATR;		//yyerror("Incorrect index of str in matrix");
		return 0;
	}
	if ((ind_col >= sz_col) || (ind_col < 0)) {
		err_code = IND_COL_MATR;			//yyerror("Incorrect index of column in matrix");
		return 0;
	}
	return m_val[ind_str][ind_col];
}

bool DataNode::get_str(int ind_str, std::vector<int>& result, err& err_code) const {		// везде исправить условия на проверку >= 0
	int sz_str = m_val.size();
	if ((ind_str < sz_str) && (ind_str >= 0)) {
		result = m_val[ind_str];
		return true;
	}
	else err_code = IND_STR_MATR;		//yyerror("Incorrect index of str in matrix");
	return false;
}

bool DataNode::get_col(int ind_col, std::vector<int>& res, err& err_code) const {
	auto [sz_str, sz_col] = get_size_matr();
	if ((ind_col >= sz_col) || (ind_col < 0)) {
		err_code = IND_COL_MATR;		//yyerror("Incorrect index of column in matrix");
		return false;
	}
	res.clear();
	for (int i = 0; i < sz_str; i++)
		res.push_back(m_val[i][ind_col]);
	return true;
}

bool DataNode::get_log_matr(const std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& res, err& err_code) const { // проверить тип элементов матрицы
	int sz_str = matr.size();
	std::vector<std::vector<int>> new_res;
#ifdef DEBUG
	if (sz_str) {
#endif // DEBUG
		int sz_col = matr[0].size();
#ifdef DEBUG
		if (sz_col) {
#endif // DEBUG
			for (int ind_str = 0; ind_str < sz_str; ind_str++) {
				int zero_sz = 0;
				std::vector<int> add;
				for (int ind_col = 0; ind_col < sz_col; ind_col++)
					if (matr[ind_str][ind_col])
						add.push_back(m_val[ind_str][ind_col]);
				if (add.empty()) {
#ifdef DEBUG
					err_code = EMPTY_STR_MATR;		//yyerror("Incorrect matrix: new matrix should have non-empty strings");
#endif // DEBUG
					return false;
				}
				if (ind_str) {
					if (zero_sz == add.size())
						new_res.push_back(add);
					else {
#ifdef DEBUG
						err_code = NON_RECT;		//yyerror("Incorrect matrix: new matrix should be rectangular");
#endif // DEBUG
						return false;
					}
				}
				else {
					zero_sz = new_res[0].size();
					new_res.push_back(add);
				}
			}
			res = new_res;
			return true;

#ifdef DEBUG
		}
		else err_code = EMPTY_STR_MATR;		//yyerror("Incorrect matrix: string in matrix was empty");
	}
	else err_code = EMPTY_MATR;		//yyerror("Incorrect matrix: it was empty");
#endif // DEBUG
	return false;
}

bool DataNode::change_info(int new_val, err& err_code, int ind_str, int ind_col) {
	int sz = 0;
	if (ind_col == -1) {
		if (ind_str == -1) {
			val = new_val;
			return true;
		}
		sz = v_val.size();
		if ((ind_str < sz) && (ind_str >= 0)) {
			v_val[ind_str] = new_val;
			return true;
		}
		err_code = IND_VECT;		//yyerror("Incorrect index of vector");
		return false;
	}
	else sz = m_val.size();
	if ((ind_str < 0) || (ind_str >= sz)) {
		err_code = IND_STR_MATR;		//yyerror("Incorrect index of string of matrix");
		return false;
	}
	int sz_str = m_val[0].size();
	if ((ind_col < 0) || (ind_col >= sz)) {
		err_code = IND_COL_MATR;
		return false;
	}
	m_val[ind_str][ind_col] = new_val;
	return true;
}

bool DataNode::push_back_info(int new_val) {
	v_val.push_back(new_val);
	return true;
}

bool DataNode::push_back_info(const std::vector<int>& new_val, err& err_code) {
	if (new_val.empty()) {
		err_code = EMPTY_VECT;
		return false;
	}
	m_val.push_back(new_val);
	return true;
}

bool DataNode::delete_vect_info(int index, err& err_code, bool is_matr) {
	if (is_matr) {
		int sz = m_val.size();
		if ((index < 0) || (index >= sz)) {
			err_code = IND_STR_MATR;
			return false;
		}
		auto it = m_val.cbegin() + index;
		m_val.erase(it);
	}
	else {
		int sz = v_val.size();
		if ((index < 0) || (index >= sz)) {
			err_code = IND_VECT;
			return false;
		}
		auto it = v_val.cbegin() + index;
		v_val.erase(it);
	}
	return true;
}

void DataNode::delete_vect(bool is_matr) {
	if (is_matr)
		m_val.clear();
	else
		v_val.clear();
}

/*		Operation's nodes	*/

DefaultNode* OperNode::get_node(int index) {
	int sz = nodes.size();
	if ((index < 0) || (index >= sz)) {
		throw std::out_of_range("Index wasn't in range from 0 to size of vector of pointers");
		return nullptr;
	}
	return nodes[index];
}

bool OperNode::push_back_node(DefaultNode* node) {
	if (!node) {
		throw std::invalid_argument("Pointer was 'nullptr'");
		return false;
	}
	nodes.push_back(node);
	return true;
}

void OperNode::delete_vect(bool is_matr) {
	if (is_matr)
		throw std::invalid_argument("Incorrect usage of function");
	nodes.clear();
}

bool OperNode::delete_vect_info(int index, err& err_code, bool is_matr) {
	if (is_matr) {
		throw std::invalid_argument("Incorrect usage of function");
		return false;
	}
	int sz = nodes.size();
	if ((index < 0) || (index >= sz)) {
		throw std::out_of_range("Index wasn't in range from 0 to size of vector of pointers");
		return false;
	}
	nodes.erase(nodes.cbegin() + index);
	return true;
}

bool OperNode::change_info(DefaultNode* new_node, int index) {
	int sz = nodes.size();
	if ((index < 0) || (index >= sz)) {
		throw std::out_of_range("Index wasn't in range from 0 to size of vector of pointers");
		return false;
	}
	if (!new_node) {
		throw std::invalid_argument("New node was 'nullptr'");
		return false;
	}
	nodes[index] = new_node;
	return true;
}