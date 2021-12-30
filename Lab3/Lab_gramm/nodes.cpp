#include "nodes.h"


Robot* my_robot = nullptr;
std::vector<std::map<std::string, DefaultNode*>> func_variables;
std::map<std::string, DefaultNode*> func_sentenses;
std::map<std::string, std::vector<DefaultNode*>> func_params;

/*		Methods for tree		*/


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
		CASE("get_map"):
			my_robot->print_map();
			ptr = new DataNode;
			ptr->change_empty(true);
			break;
		CASE("left") :
			my_robot->left();
			ptr = new DataNode;
			ptr->change_empty(true);
			break;
		CASE("right") :
			my_robot->right();
			ptr = new DataNode;
			ptr->change_empty(true);
			break;
		CASE("exit") :
			ptr = new DataNode(my_robot->exit());
			break;
		CASE("wall") :
			ptr = new DataNode(my_robot->wall());
			break;
		default:
			throw std::logic_error("Incorrect number of parameters");
			break;
	}
	return ptr;
}

DefaultNode* one_node(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* fir_leaf = nullptr;
	fir_leaf = bypassTree(par->get_node(0));
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("sum") :
			sum(fir_leaf);
			break;
		CASE("transpon") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else transpon(fir_leaf);
			break;
		CASE("not") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else not_bool(fir_leaf);
			break;
		CASE("sl") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else sl(fir_leaf);
			break;
		CASE("sr") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else sr(fir_leaf);
			break;
		CASE("move") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else move(fir_leaf);
			break;
		CASE("get_var") :
			get_variable(fir_leaf);
			break;
		CASE("matr_vect") :
			matr_vect(fir_leaf);
			break;
		CASE("e_call") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else e_call(fir_leaf);
			break;
		CASE("const") :
			new_decl(fir_leaf);
			fir_leaf->change_const(true);
			break;
		CASE("new_decl") :
			new_decl(fir_leaf);
			break;
		CASE("print") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else my_print(fir_leaf);
		CASE("get_size") :
			if (fir_leaf->get_empty())
				yyerror("This is an undefined variable");
			else get_size(fir_leaf);
		CASE("func_act") : CASE("new_expr") :
			break;
		default:
			throw std::logic_error("Incorrect number of parameters");
			break;
	}
	return fir_leaf;
}

void sum(DefaultNode* ptr) {
	int new_sum = 0;
	err code = OK;
	std::string data_tp;
	ptr->get_type(data_tp);
	if (ptr->check_oper())
		yyerror("Incorrect type: it is operation type");
	SWITCH(data_tp) {
		CASE("int") : CASE("bool") : CASE("v_bool") : CASE("m_bool"):
			yyerror("Incorrect type of data");
			break;
		CASE("v_int") : {
			int sz = ptr->get_size_vect();
			for (int i = 0; i < sz; i++) {
				new_sum += ptr->get_elem(code, i);
				if (code != OK)
					error_type(code);
			}
		}
			break;
		CASE("m_int") : {
			auto [sz_str, sz_col] = ptr->get_size_matr();
			for (int i = 0; i < sz_str; i++)
				for (int j = 0; j < sz_col; j++) {
					new_sum += ptr->get_elem(code, i, j);
					if (code != OK)
						error_type(code);
				}
		}
			break;
	}
	if (!ptr->change_info(new_sum, code))
		yyerror("Unexcpected behavior: info wasn't changed");
	ptr->set_type("int");
	ptr->set_var_name("");
	ptr->change_const(true);
}

void transpon(DefaultNode* matrix) {
	std::string data_tp;
	matrix->get_type(data_tp);
	if (matrix->check_oper())
		yyerror("Incorrect type: it is operation type");
	if((data_tp == "m_int") || (data_tp == "m_bool")) {
		std::vector<std::vector<int>> new_matrix;
		std::vector<int> add;
		err code = OK;
		auto [str_sz, col_sz] = matrix->get_size_matr();
		for (int i = 0; i < col_sz; i++) {
			for (int j = 0; j < str_sz; j++) {
				add.push_back(matrix->get_elem(code, j, i));
				if (code != OK)
					error_type(code);
			}
			new_matrix.push_back(add);
			add.clear();
		}
		matrix->change_matrix(new_matrix);
		matrix->set_var_name("");
		matrix->change_const(true);
	}
	else yyerror("Incorrect type of data");
}

void not_bool(DefaultNode* ptr) {
	std::string tp;
	ptr->get_type(tp);
	err code = OK;
	SWITCH(tp) {
		CASE("int") :CASE("v_int") : CASE("m_int") :
			yyerror("Incorrect type of data");
			break;
		CASE("bool") :
			if (ptr->get_elem(code)) {
				if (code == OK) {
					ptr->change_info(0, code);
					if (code != OK)
						error_type(code);
				}
				else error_type(code);
			}
			else {
				if (code == OK) {
					ptr->change_info(1, code);
					if (code != OK)
						error_type(code);
				}
				else error_type(code);
			}
			break;
		CASE("v_bool") : {
			int sz = ptr->get_size_vect();
			for (int i = 0; i < sz; i++)
				if (ptr->get_elem(code, i)) {
					if (code == OK) {
						ptr->change_info(0, code, i);
						if (code != OK)
							error_type(code);
					}
					else error_type(code);
				}
				else {
					if (code == OK) {
						ptr->change_info(1, code, i);
						if (code != OK)
							error_type(code);
					}
					else error_type(code);
				}
		}
			break;
		CASE("m_bool") : {
			auto [str_sz, col_sz] = ptr->get_size_matr();
			for(int i = 0; i < str_sz; i++)
				for(int j = 0; j < col_sz; j++)
					if (ptr->get_elem(code, i, j)) {
						if (code == OK) {
							ptr->change_info(0, code, i, j);
							if (code != OK)
								error_type(code);
						}
						else error_type(code);
					}
					else {
						if (code == OK) {
							ptr->change_info(1, code, i, j);
							if (code != OK)
								error_type(code);
						}
						else error_type(code);
					}
		}
			break;
	}
	ptr->change_const(true);
	ptr->set_var_name("");
}

void sl(DefaultNode* ptr) {
	std::string tp;
	ptr->get_type(tp);
	err code = OK;
	if (tp == "int") {
		int num = ptr->get_elem(code);
		char up_byte = num? 0: 1;
		num <<= 1;
		num |= up_byte;
		ptr->change_info(num, code);
		if (code != OK)
			error_type(code);
		ptr->set_var_name("");
		ptr->change_const(true);
	}
	else yyerror("Incorrect type of data");
}

void sr(DefaultNode* ptr) {
	std::string tp;
	ptr->get_type(tp);
	err code = OK;
	if (tp == "int") {
		int num = ptr->get_elem(code);
		int up_byte = num & 1;
		num >>= 1;
		if (up_byte != 0) {
			up_byte <<= 31;
			num |= up_byte;
		}
		ptr->change_info(num, code);
		if (code != OK)
			error_type(code);
		ptr->set_var_name("");
		ptr->change_const(true);
	}
	else yyerror("Incorrect type of data");
}

void move(DefaultNode* ptr) {
	std::string tp;
	ptr->get_type(tp);
	if(tp != "int")
		yyerror("Incorrect type of data");
	err code = OK;
	int steps = ptr->get_elem(code);
	if (steps <= 0)
		yyerror("There must be number more than 0");
	if (code != OK)
		error_type(code);
	bool went = my_robot->move(steps);
	ptr->change_info(went, code);
	ptr->set_type("bool");
	ptr->change_const(false);
	ptr->set_var_name("");
}

void get_variable(DefaultNode*& ptr) {
	std::string str = ptr->get_var_name();
	DefaultNode* node = nullptr;
	for (int last_func_ind = func_variables.size() - 1; last_func_ind >= 0; last_func_ind--) {
		auto search = func_variables[last_func_ind].find(str);
		if (search != func_variables[last_func_ind].end()) {
			node = new DataNode((*search).second);
			break;
		}
	}
	if(!node) {
		std::string dop = "Variable " + str + " didn't found";
		yyerror(dop.c_str());
		return;
	}
	delete ptr;
	ptr = node;
}

void matr_vect(DefaultNode* ptr) {
	std::string tp, buf;
	ptr->get_type(tp);
	err code = OK;
	if (tp[0] == 'm')
		yyerror("Unknown dimension of data");
	else if (tp[0] == 'v'){
		std::vector<DefaultNode*> list;
		std::vector<int> adder;
		std::vector<std::vector<int>> new_matrix;
		ptr->get_vect(adder);
		int sz = adder.size();
		new_matrix.push_back(adder);
		ptr->get_expr_list(list);
		for (auto vectors : list) {
			vectors->get_type(buf);
			if (buf != tp)
				yyerror("Different types of data");
			vectors->get_vect(adder);
			if (sz != adder.size())
				yyerror("Different sizes of vectors");
			new_matrix.push_back(adder);
			delete vectors;
		}
		ptr->clear_exprs();
		ptr->change_matrix(new_matrix);
		buf = "m_";
		buf.append(tp.cbegin() + 2, tp.cend());
	}
	else {
		std::vector<DefaultNode*> list;
		std::vector<int> new_vector;
		new_vector.push_back(ptr->get_elem(code));
		if (code != OK)
			error_type(code);
		ptr->get_expr_list(list);
		for (auto scalar : list) {
			scalar->get_type(buf);
			if (buf != tp)
				yyerror("Different types of data");
			new_vector.push_back(scalar->get_elem(code));
			if (code != OK)
				error_type(code);
			delete scalar;
		}
		ptr->clear_exprs();
		ptr->change_vector(new_vector);
		buf = "v_";
		buf.append(tp);
	}
	ptr->set_type(buf);
}

void e_call(DefaultNode* func_name) {
	std::string name = func_name->get_var_name();
	auto search_func = func_sentenses.find(name);
	if (search_func == func_sentenses.end()) {
		yyerror("Function with this name doesn't exist");
		return;
	}
	std::map<std::string, DefaultNode*> add;
	auto search_params = func_params.find(name);
	if (search_params != func_params.end()) {
		auto vect_params = (*search_params).second;
		if (!vect_params.empty()) {
			int sz_par = vect_params.size();
			err code = OK;
			for (int i = 0; i < sz_par; i++) {
				if(vect_params[i]->get_empty()) {
					yyerror("Incorrect number of parameters");
					return;
				}
				DefaultNode* new_var = new DataNode(vect_params[i]);
				add.insert({new_var->get_var_name(), new_var});
			}
		}
	}
	func_variables.push_back(add);
	bypassTree((*search_func).second);
	int last = func_variables.size() - 1;
	for (auto ptr : func_variables[last])
		delete ptr.second;
	func_variables.pop_back();
}

void new_decl(DefaultNode* var) {
	var->change_const(false);
	std::string var_name = var->get_var_name();
	auto search = func_variables[func_variables.size() - 1].find(var_name);
	if(search == func_variables[func_variables.size() - 1].end())
		func_variables[func_variables.size() - 1].insert({ var_name, var });
	else yyerror("Variable with this name has already been created");
}

void my_print(DefaultNode* elem) {
	std::string my_type, my_name = elem->get_var_name();
	elem->get_type(my_type);
	err code = OK;
	std::cout << std::endl << "Type: " << my_type << std::endl;
	if (elem->get_const())
		std::cout << "Constant ";
	if (!my_name.empty()) {
		std::cout << "Variable with name: " << my_name;
		if (elem->get_empty())
			std::cout << " (empty)";
	}
	bool flag = false;
	std::cout << std::endl << "Content:";
	SWITCH(my_type) {
		CASE("bool") :
			flag = true;
		CASE("int") :
			if (flag) {
				int num = elem->get_elem(code);
				if(num)
					std::cout << "true" << " (" << num << ')' << std::endl;
				else std::cout << "false" << " (" << num << ')' << std::endl;
			}
			else std::cout << elem->get_elem(code) << std::endl;
			break;
		CASE("v_bool") :
			flag = true;
		CASE("v_int") : {
			std::vector<int> buf;
			elem->get_vect(buf);
			int sz = buf.size();
			std::cout << std::endl;
			for (int i = 0; i < sz; i++)
				if (flag) {
					if(buf[i])
						std::cout << ' ' << "true" << " (" << buf[i] << ") ";
					else std::cout << ' ' << "false" << " (" << buf[i] << ") ";
				}
				else std::cout << ' ' << buf[i] << ' ';
			std::cout << std::endl;
		}
			break;
		CASE("m_bool") :
			flag = true;
		CASE("m_int") : {
			std::vector<std::vector<int>> buf;
			elem->get_matr(buf);
			std::cout << std::endl;
			for(int i = 0; i < buf.size(); i++) {
				for(int j = 0; j < buf[0].size(); j++)
					if (flag) {
						if(buf[i][j])
							std::cout << ' ' << "true" << " (" << buf[i][j] << ") ";
						else std::cout << ' ' << "false" << " (" << buf[i][j] << ") ";
					}
					else std::cout << ' ' << buf[i][j] << ' ';
				std::cout << std::endl;
			}
		}
			break;
	}
}

void get_size(DefaultNode* vect) {
	std::string my_type;
	err code = OK;
	vect->get_type(my_type);
	bool is_vector = false;
	switch(my_type[0]) {
		case 'v':
			is_vector = true;
		case 'i': case 'b':
			vect->set_type("int");
			vect->change_info(is_vector? vect->get_size_vect() : 1, code);
			if(code != OK) {
				error_type(code);
				return;
			}
		break;
		case 'm': {
			vect->set_type("v_int");
			auto [sz_str, sz_col] = vect->get_size_matr();
			vect->change_vector({sz_str, sz_col});	// look here plz
		}
		break;
		default:
			yyerror("Unknown data type");
			return;
	}
	vect->set_var_name("");
	vect->change_const("true");
}

DefaultNode* two_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	err code = OK;
	DefaultNode * fir_leaf = nullptr, * sec_leaf = nullptr;
	fir_leaf = bypassTree(par->get_node(0));
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("make_m2") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else make_m2(fir_leaf, sec_leaf);
			break;
		CASE("make_v") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else make_v(fir_leaf, sec_leaf);
			break;
		CASE("ch_tp") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else change_type(sec_leaf, fir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("eq") :
			sec_leaf = bypassTree(par->get_node(1));
			if (sec_leaf->get_empty())
				yyerror("There are an undefined variable");
			else equality(fir_leaf, sec_leaf);
			break;
		CASE("log_matr") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else log_matr(fir_leaf, sec_leaf);
			break;
		CASE("mul") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else mul(fir_leaf, sec_leaf);
			break;
		CASE("el_mul") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else el_mul(fir_leaf, sec_leaf);
			break;
		CASE("get_str") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else get_str(fir_leaf, sec_leaf);
			break;
		CASE("get_col") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else get_col(fir_leaf, sec_leaf);
			break;
		CASE("and") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else do_and(fir_leaf, sec_leaf);
			break;
		CASE("gt") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else do_gt(fir_leaf, sec_leaf);
			break;
		CASE("lt") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else do_lt(fir_leaf, sec_leaf);
			break;
		CASE("-") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else minus(fir_leaf, sec_leaf);
			break;
		CASE("+") :
			sec_leaf = bypassTree(par->get_node(1));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else plus(fir_leaf, sec_leaf);
			break;
		CASE("e_decl") :
			sec_leaf = bypassTree(par->get_node(1));
			e_decl(sec_leaf, fir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("v_e_decl") :
			sec_leaf = bypassTree(par->get_node(1));
			v_e_decl(sec_leaf, fir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("m_e_decl") :
			sec_leaf = bypassTree(par->get_node(1));
			m_e_decl(sec_leaf, fir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("call") :
			sec_leaf = bypassTree(par->get_node(1));
			call(fir_leaf, sec_leaf);
			break;
		CASE("endl") : CASE(",") :
			sec_leaf = bypassTree(par->get_node(1));
			fir_leaf->push_back_node(sec_leaf);
			break;
		CASE("if") :
			do_if(fir_leaf, par->get_node(1));
			break;
		CASE("push"):
			sec_leaf = bypassTree(par->get_node(1));
			do_push(fir_leaf, sec_leaf);
			break;
		default:
			throw std::logic_error("Incorrect number of parameters");
			break;
	}
	return fir_leaf;
}

void init_two(DefaultNode* fir, DefaultNode*& sec) {
	fir->change_const(true);
	fir->set_var_name("");
	delete sec;
	sec = nullptr;
}

void make_m2(DefaultNode* vect, DefaultNode*& new_sz) {
	std::string fir_tp, sec_tp;
	err code = OK;
	vect->get_type(fir_tp);
	if (fir_tp[0] != 'v')
		yyerror("Incorrect type of data: first expression must be a vector");
	else {
		std::vector<std::vector<int>> new_matrix;
		new_sz->get_type(sec_tp);
		if (sec_tp != "int") {
			yyerror("Incorrect type of data: second expression must be a scalar");
			return;
		}
		int sz = new_sz->get_elem(code);
		if (code != OK)
			error_type(code);
		else {
			std::vector<int> add;
			vect->get_vect(add);
			for (int i = 0; i < sz; i++)
				new_matrix.push_back(add);
			fir_tp[0] = 'm';
			vect->change_matrix(new_matrix);
			vect->set_type(fir_tp);
			init_two(vect, new_sz);
		}
	}
}

void make_v(DefaultNode* scalar, DefaultNode*& new_sz) {
	std::string fir_tp, sec_tp;
	err code = OK;
	scalar->get_type(fir_tp);
	if((fir_tp[0] != 'i') && (fir_tp[0] != 'b'))
		yyerror("Incorrect type of data: first expression must be a scalar");
	else {
		std::vector<int> new_vector;
		int sz = new_sz->get_elem(code);
		if (code != OK)
			error_type(code);
		else {
			int elem = scalar->get_elem(code);
			new_sz->get_type(sec_tp);
			if (sec_tp != "int") {
				yyerror("Incorrect type of data: second expression must be a scalar");
				return;
			}
			if (code != OK)
				error_type(code);
			{
				std::vector<int> add;
				if (sz <= 0)
					yyerror("Incorrect index of new size of columns");
				for (int i = 0; i < sz; i++)
					add.push_back(elem);
				fir_tp = "v_" + fir_tp;
				scalar->change_vector(add);
				scalar->set_type(fir_tp);
				init_two(scalar, new_sz);
			}
		}
	}
}

void change_type(DefaultNode* expr, DefaultNode*& new_type) {
	std::string new_tp, prev_tp;
	new_type->get_type(new_tp);
	if ((new_tp != "bool") && (new_tp != "int"))
		yyerror("Incorrect type of data for typecasting.");
	expr->get_type(prev_tp);
	switch (prev_tp[0]) {
	case 'i': case 'b':
		expr->set_type(new_tp);
		break;
	case 'v': case 'm':
		if(prev_tp[0] == 'm')
			new_tp = "m_" + new_tp;
		else new_tp = "v_" + new_tp;
		expr->set_type(new_tp);
		break;
	default:
		throw std::logic_error("Unexpected type of data");
	}
	init_two(expr, new_type);
}

void equality(DefaultNode*& expr, DefaultNode*& new_val) {
	std::string name = expr->get_var_name();
	DefaultNode* ptr = nullptr;
	for (int last_func_ind = func_variables.size() - 1; last_func_ind >= 0; last_func_ind--) {
		auto search = func_variables[last_func_ind].find(name);
		if (search != func_variables[last_func_ind].end()) {
			ptr = (*search).second;
			break;
		}
	}
	if(!ptr) {
		yyerror("Variable with this name doesn't exist");
		return;
	}
	delete expr;
	expr = ptr;
	if (expr->get_const())
		yyerror("Incorrect variable: the variable is constant");
	else {
		std::string fir_tp, sec_tp;
		expr->get_type(fir_tp); new_val->get_type(sec_tp);
		if (fir_tp != sec_tp)
			yyerror("Incorrect types of data: new value must have the same type as the variable");
		else {
			err code = OK;
			switch (fir_tp[0]) {
				case 'i':case 'b': {
					int val = new_val->get_elem(code);
					if (code != OK)
						error_type(code);
					expr->change_info(val, code);
					if (code != OK)
						error_type(code);
				}
					break;
				case 'v': {
					std::vector<int> new_vect;
					new_val->get_vect(new_vect);
					expr->change_vector(new_vect);
				}
					break;
				case 'm': {
					std::vector<std::vector<int>> new_matr;
					new_val->get_matr(new_matr);
					expr->change_matrix(new_matr);
				}
					break;
			}
			expr->change_empty(false);
			expr = new_val;
		}
	}
}

void log_matr(DefaultNode* var, DefaultNode*& logical_matr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	if (var->get_empty()) {
		yyerror("This is an undefined variable");
		return;
	}
	var->get_type(fir_tp);
	if (fir_tp[0] != 'm')
		yyerror("Incorrect type of variable: there must be only a matrix");
	else {
		logical_matr->get_type(sec_tp);
		if (sec_tp != "m_bool")
			yyerror("Incorrect type of argument: there must be only a logical matrix");
		std::vector<std::vector<int>> res, matr;
		logical_matr->get_matr(matr);
		var->get_log_matr(matr, res, code);
		if (code != OK)
			error_type(code);
		var->change_matrix(res);
		transpon(var);
		init_two(var, logical_matr);
	}
}

void mul(DefaultNode* fir_matr, DefaultNode*& sec_matr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_matr->get_type(fir_tp);
	sec_matr->get_type(sec_tp);
	if ((fir_tp == sec_tp) && (fir_tp == "m_int")) {
		std::vector<std::vector<int>> fir, sec;
		fir_matr->get_matr(fir);
		sec_matr->get_matr(sec);
		auto [str1, col1] = fir_matr->get_size_matr();
		auto [str2, col2] = sec_matr->get_size_matr();
		if (col1 != str2)
			yyerror("Incompatible matrix sizes");
		else {
			std::vector<int> add;
			std::vector<std::vector<int>> res;
			for(int str_fir_matr = 0; str_fir_matr < str1; str_fir_matr++) {
				for (int col_sec_matr = 0; col_sec_matr < col2; col_sec_matr++) {
					int sum = 0;
					for (int buf_ind = 0; buf_ind < col1; buf_ind++)
						sum += fir[str_fir_matr][buf_ind] * sec[buf_ind][col_sec_matr];
					add.push_back(sum);
				}
				res.push_back(add);
				add.clear();
			}
			fir_matr->change_matrix(res);
			init_two(fir_matr, sec_matr);
		}
	}
	else if((fir_tp == sec_tp) && (fir_tp == "int")) {
		int fir_elem = fir_matr->get_elem(code), sec_elem;
		if(code != OK) {
			error_type(code);
			return;
		}
		sec_elem = sec_matr->get_elem(code);
		if(code != OK) {
			error_type(code);
			return;
		}
		fir_matr->change_info(fir_elem * sec_elem, code);
		if(code != OK) {
			error_type(code);
			return;
		}
		init_two(fir_matr, sec_matr);
	}
	else yyerror("There is an expression with unsupported type of data");
}

void el_mul(DefaultNode* fir_matr, DefaultNode*& sec_matr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_matr->get_type(fir_tp);
	sec_matr->get_type(sec_tp);
	if ((fir_tp == sec_tp) && (fir_tp == "m_int")) {
		std::vector<std::vector<int>> fir, sec;
		fir_matr->get_matr(fir);
		sec_matr->get_matr(sec);
		auto [str1, col1] = fir_matr->get_size_matr();
		auto [str2, col2] = sec_matr->get_size_matr();
		if ((str1 == str2) && (col1 == col2)) {
			for (int i = 0; i < str1; i++)
				for (int j = 0; j < col1; j++)
					fir[i][j] = fir[i][j] * sec[i][j];
			fir_matr->change_matrix(fir);
			init_two(fir_matr, sec_matr);
		}
		else yyerror("Incompatible matrix sizes");
	}
	else yyerror("There is an expression with unsupported type of data");
}

void get_str(DefaultNode* matr, DefaultNode*& index) {
	std::string fir_tp, sec_tp;
	err code = OK;
	if (matr->get_empty()) {
		yyerror("This is an undefined variable");
		return;
	}
	matr->get_type(fir_tp);
	if (fir_tp[0] != 'm') {
		yyerror("Incorrect type of data: variable should be a matrix");
		return;
	}
	index->get_type(sec_tp);
	SWITCH(sec_tp) {
		CASE("v_int") : {
			std::vector<int> IDs, adder;
			std::vector<std::vector<int>> res;
			index->get_vect(IDs);
			for (auto ind : IDs) {
				matr->get_str(ind, adder, code);
				if (code != OK) {
					error_type(code);
					return;
				}
				res.push_back(adder);
			}
			matr->change_matrix(res);
			init_two(matr, index);
		}
			break;
		CASE("v_bool") : {
			std::vector<int> IDs, adder;
			std::vector<std::vector<int>> res;
			index->get_vect(IDs);
			auto [str_sz, col_sz] = matr->get_size_matr();
			if (IDs.size() == str_sz) {
				for (int i = 0; i < str_sz; i++)
					if (IDs[i]) {
						matr->get_str(i, adder, code);
						if (code != OK) {
							error_type(code);
							return;
						}
						res.push_back(adder);
					}
				matr->change_matrix(res);
				init_two(matr, index);
			}
			else yyerror("The size of logical vector should be equal to amount of strings in matrix");
		}
			break;
		CASE("int") : {
			std::vector<int> res;
			int ind = index->get_elem(code);
			if (code != OK) {
				error_type(code);
				return;
			}
			matr->get_str(ind, res, code);
			matr->change_vector(res);
			fir_tp[0] = 'v';
			matr->set_type(fir_tp);
			init_two(matr, index);
		}
			break;
		default:
			yyerror("Incorrect index: there should be 'int' or 'v_int' or 'v_bool'");
			break;
	}	
}

void get_col(DefaultNode* matr, DefaultNode*& index) {
	std::string fir_tp, sec_tp;
	err code = OK;
	if (matr->get_empty()) {
		yyerror("This is an undefined variable");
		return;
	}
	matr->get_type(fir_tp);
	if (fir_tp[0] != 'm') {
		yyerror("Incorrect type of data: variable should be a matrix");
		return;
	}
	index->get_type(sec_tp);
	SWITCH(sec_tp) {
		CASE("int") : {
			std::vector<int> res;
			int ind = index->get_elem(code);
			if (code != OK) {
				error_type(code);
				return;
			}
			matr->get_col(ind, res, code);
			matr->change_vector(res);
			fir_tp[0] = 'v';
			matr->set_type(fir_tp);
			init_two(matr, index);
		}
			break;
		CASE("v_int") : {
			std::vector<int> IDs, adder;
			std::vector<std::vector<int>> res;
			index->get_vect(IDs);
			for (auto ind : IDs) {
				matr->get_col(ind, adder, code);
				if (code != OK) {
					error_type(code);
					return;
				}
				res.push_back(adder);
			}
			matr->change_matrix(res);
			init_two(matr, index);
		}
			break;
		CASE("v_bool") :{
			std::vector<int> IDs, adder;
			std::vector<std::vector<int>> res;
			index->get_vect(IDs);
			auto [str_sz, col_sz] = matr->get_size_matr();
			if (IDs.size() == col_sz) {
				for (int i = 0; i < col_sz; i++)
					if (IDs[i]) {
						matr->get_col(i, adder, code);
						if (code != OK) {
							error_type(code);
							return;
						}
						res.push_back(adder);
					}
				matr->change_matrix(res);
				init_two(matr, index);
			}
			else yyerror("The size of logical vector should be equal to amount of strings in matrix");
		}
			break;
		default:
			yyerror("Incorrect index: there should be 'int' or 'v_int' or 'v_bool'");
			break;
	}
}

void do_and(DefaultNode* fir_log_expr, DefaultNode*& sec_log_expr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_log_expr->get_type(fir_tp);
	sec_log_expr->get_type(sec_tp);
	if (fir_tp != sec_tp) {
		yyerror("There should be the same types of expressions");
		return;
	}
	SWITCH(fir_tp) {
		CASE("bool") : {
			int fir = fir_log_expr->get_elem(code), sec;
			if (code != OK) {
				error_type(code);
				return;
			}
			sec = sec_log_expr->get_elem(code);
			if (code != OK) {
				error_type(code);
				return;
			}
			fir_log_expr->change_info(fir && sec, code);
			if (code != OK) {
				error_type(code);
				return;
			}
			init_two(fir_log_expr, sec_log_expr);
		}
			break;
		CASE("v_bool") : {
			std::vector<int> fir, sec, res;
			fir_log_expr->get_vect(fir);
			sec_log_expr->get_vect(sec);
			int sz = fir.size();
			if (sz != sec.size()) {
				yyerror("Vectors must have the same size");
				return;
			}
			for (int i = 0; i < sz; i++)
				res.push_back(fir[i] && sec[i]);
			fir_log_expr->change_vector(res);
			init_two(fir_log_expr, sec_log_expr);
		}
			break;
		CASE("m_bool") : {
			std::vector<std::vector<int>> fir, sec, res;
			int sz_str = fir.size(), sz_col = fir[0].size();
			if ((sz_str != sec.size()) || (sz_col != sec[0].size())) {
				yyerror("Matrix must have the same size");
				return;
			}
			std::vector<int> add;
			for (int i = 0; i < sz_str; i++) {
				for (int j = 0; j < sz_col; j++)
					add.push_back(fir[i][j] && sec[i][j]);
				res.push_back(add);
				add.clear();
			}
			fir_log_expr->change_matrix(res);
			init_two(fir_log_expr, sec_log_expr);
		}
			break;
		default:
			yyerror("There must be logical types of data");
			break;
	}
}

void do_gt(DefaultNode* fir_ariph_expr, DefaultNode*& sec_ariph_expr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_ariph_expr->get_type(fir_tp);
	sec_ariph_expr->get_type(sec_tp);
	if ((fir_tp != sec_tp) || (fir_tp != "int")) {
		yyerror("There should be two ariphmetical scalars");
		return;
	}
	int fir = fir_ariph_expr->get_elem(code), sec;
	if (code != OK) {
		error_type(code);
		return;
	}
	sec = sec_ariph_expr->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	fir_ariph_expr->change_info(fir > sec, code);
	if (code != OK) {
		error_type(code);
		return;
	}
	fir_ariph_expr->set_type("bool");
	init_two(fir_ariph_expr, sec_ariph_expr);
}

void do_lt(DefaultNode* fir_ariph_expr, DefaultNode*& sec_ariph_expr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_ariph_expr->get_type(fir_tp);
	sec_ariph_expr->get_type(sec_tp);
	if ((fir_tp != sec_tp) || (fir_tp != "int")) {
		yyerror("There should be two ariphmetical scalars");
		return;
	}
	int fir = fir_ariph_expr->get_elem(code), sec;
	if (code != OK) {
		error_type(code);
		return;
	}
	sec = sec_ariph_expr->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	fir_ariph_expr->change_info(fir < sec, code);
	if (code != OK) {
		error_type(code);
		return;
	}
	fir_ariph_expr->set_type("bool");
	init_two(fir_ariph_expr, sec_ariph_expr);
}

void minus(DefaultNode* fir_ariph_expr, DefaultNode*& sec_ariph_expr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_ariph_expr->get_type(fir_tp);
	sec_ariph_expr->get_type(sec_tp);
	if ((fir_tp != sec_tp) || (fir_tp != "int")) {
		yyerror("There should be two ariphmetical scalars");
		return;
	}
	int fir = fir_ariph_expr->get_elem(code), sec;
	if (code != OK) {
		error_type(code);
		return;
	}
	sec = sec_ariph_expr->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	fir_ariph_expr->change_info(fir - sec, code);
	if (code != OK) {
		error_type(code);
		return;
	}
	init_two(fir_ariph_expr, sec_ariph_expr);
}

void plus(DefaultNode* fir_ariph_expr, DefaultNode*& sec_ariph_expr) {
	std::string fir_tp, sec_tp;
	err code = OK;
	fir_ariph_expr->get_type(fir_tp);
	sec_ariph_expr->get_type(sec_tp);
	if ((fir_tp != sec_tp) || (fir_tp != "int")) {
		yyerror("There should be two ariphmetical scalars");
		return;
	}
	int fir = fir_ariph_expr->get_elem(code), sec;
	if (code != OK) {
		error_type(code);
		return;
	}
	sec = sec_ariph_expr->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	fir_ariph_expr->change_info(fir + sec, code);
	if (code != OK) {
		error_type(code);
		return;
	}
	init_two(fir_ariph_expr, sec_ariph_expr);
}

void e_decl(DefaultNode* var, DefaultNode*& type) {
	std::string new_tp;
	type->get_type(new_tp);
	var->set_type(new_tp);
	var->change_const(false);
	delete type;
}

void v_e_decl(DefaultNode* var, DefaultNode*& type) {
	std::string new_tp;
	type->get_type(new_tp);
	new_tp = "v_" + new_tp;
	var->set_type(new_tp);
	var->change_const(false);
	delete type;
}

void m_e_decl(DefaultNode* var, DefaultNode*& type) {
	std::string new_tp;
	type->get_type(new_tp);
	new_tp = "m_" + new_tp;
	var->set_type(new_tp);
	var->change_const(false);
	delete type;
}

void call(DefaultNode* func_name, DefaultNode*& params) {
	std::string name = func_name->get_var_name();
	auto search_func = func_sentenses.find(name);
	if (search_func == func_sentenses.end()) {
		yyerror("Function with this name doesn't exist");
		return;
	}
	auto search_params = func_params.find(name);
	if (search_params == func_params.end()) {
		yyerror("Unexpected error: parameters were not declared for the function");
		return;
	}
	auto all_parameters = (*search_params).second;
	std::vector<DefaultNode*> found_params, adder;
	found_params.push_back(params);
	params->get_expr_list(adder);
	found_params.insert(found_params.cend(), adder.begin(), adder.end());
	int sz_found = found_params.size(), sz_all = all_parameters.size();
	if (sz_found > sz_all) {
		yyerror("Unexpected behaviour: the number of parameters is more than the function supports");
		return;
	}
	std::map<std::string, DefaultNode*> add;
	for (int ind_def_param = 0; ind_def_param < sz_found; ind_def_param++) {
		DefaultNode * old_par = new DataNode(all_parameters[ind_def_param]), * new_par = found_params[ind_def_param];
		std::string old_tp, new_tp;
		err code = OK;
		old_par->get_type(old_tp); new_par->get_type(new_tp);
		if(old_tp == new_tp) {
			int elem = new_par->get_elem(code);
			if(code != OK) {
				error_type(code);
				return;
			}
			old_par->change_info(elem, code);
			if(code != OK) {
				error_type(code);
				return;
			}
			std::vector<int> int_adder;
			new_par->get_vect(int_adder);
			old_par->change_vector(int_adder);
			std::vector<std::vector<int>> new_matr;
			new_par->get_matr(new_matr);
			old_par->change_matrix(new_matr);
			delete new_par;
		}
		else {
			std::string error("Incorrect type of " + std::to_string(ind_def_param + 1) + " parameter");
			yyerror(error);
			return;
		}
		add.insert({ old_par->get_var_name(), old_par });
	}
	if (sz_found < sz_all)
		if (all_parameters[sz_found]->get_empty()) {
			for (auto it : add)
				delete it.second;
			yyerror("Incorrect number of parameters");
			return;
		}
	for (int ind_def_param = sz_found; ind_def_param < sz_all; ind_def_param++) {
		DefaultNode* new_par = new DataNode(all_parameters[ind_def_param]);
		add.insert({ new_par->get_var_name(), new_par });
	}
	func_variables.push_back(add);
	bypassTree((*search_func).second);
	int last = func_variables.size() - 1;
	for (auto ptr : func_variables[last])
		delete ptr.second;
	func_variables.pop_back();
	params = nullptr;
}

void do_if(DefaultNode*& condit, DefaultNode* sent) {
	std::string tp;
	err code = OK;
	condit->get_type(tp);
	if (tp != "bool") {
		yyerror("Incorrect type of condition");
		return;
	}
	int val = condit->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	if (val)
		condit = bypassTree(sent);
}

void do_push(DefaultNode*& var_name, DefaultNode*& new_val) {
	std::string vname = var_name->get_var_name(), my_type, new_type;
	new_val->get_type(new_type);
	DefaultNode* node = nullptr;
	for (int last_func_ind = func_variables.size() - 1; last_func_ind >= 0; last_func_ind--) {
		auto search = func_variables[last_func_ind].find(vname);
		if (search != func_variables[last_func_ind].end()) {
			node = (*search).second;
			break;
		}
	}
	node->get_type(my_type);
	if(((my_type == "v_int") && (new_type == "int")) || ((new_type == "bool") && (my_type == "v_bool"))) {
		err code = OK;
		int elem = new_val->get_elem(code);
		if(code != OK) {
			error_type(code);
			return;
		}
		std::vector<int> add;
		node->get_vect(add);
		add.push_back(elem);
		node->change_vector(add);
		node->change_empty(false);
	}
	else yyerror("Incorrect type of data");
	delete new_val;
	get_variable(var_name);
}

DefaultNode* three_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* fir_leaf = nullptr, * sec_leaf = nullptr, * thir_leaf = nullptr;
	fir_leaf = bypassTree(par->get_node(0)), sec_leaf = bypassTree(par->get_node(1));
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("make_m1") :
			thir_leaf = bypassTree(par->get_node(2));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()) || (thir_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else make_m1(fir_leaf, sec_leaf, thir_leaf);
			break;
		CASE("coord") :
			thir_leaf = bypassTree(par->get_node(2));
			if ((fir_leaf->get_empty()) || (sec_leaf->get_empty()) || (thir_leaf->get_empty()))
				yyerror("There are an undefined variable");
			else coord(fir_leaf, sec_leaf, thir_leaf);
			break;
		CASE("decl") :
			thir_leaf = bypassTree(par->get_node(2));
			decl(sec_leaf, fir_leaf, thir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("v_decl") :
			thir_leaf = bypassTree(par->get_node(2));
			v_decl(sec_leaf, fir_leaf, thir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("m_decl") :
			thir_leaf = bypassTree(par->get_node(2));
			m_decl(sec_leaf, fir_leaf, thir_leaf);
			fir_leaf = sec_leaf;
			break;
		CASE("cr_func4") :
			thir_leaf = par->get_node(2);
			cr_func(sec_leaf, fir_leaf, nullptr, nullptr, thir_leaf);
			break;
		default:
			throw std::logic_error("Incorrect number of parameters");
			break;
	}
	return fir_leaf;
}

void init_three(DefaultNode* fir, DefaultNode*& sec, DefaultNode*& third) {
	fir->change_const(true);
	fir->set_var_name("");
	delete sec, third;
	sec = nullptr; third = nullptr;
}

void make_m1(DefaultNode* scalar, DefaultNode*& strs, DefaultNode*& cols) {
	std::string fir_tp, sec_tp, thir_tp;
	err code = OK;
	scalar->get_type(fir_tp);
	if ((fir_tp[0] == 'v') || (fir_tp[0] == 'm')) {
		yyerror("First parameter must be a scalar");
		return;
	}
	strs->get_type(sec_tp); cols->get_type(thir_tp);
	if ((sec_tp != "int") || (thir_tp != "int")) {
		yyerror("Second and third parameters must be a scalars");
		return;
	}
	int sz_str = strs->get_elem(code), elem = scalar->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	int sz_col = cols->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	std::vector<int> add;
	std::vector<std::vector<int>> res;
	for (int i = 0; i < sz_col; i++)
		add.push_back(elem);
	for (int i = 0; i < sz_str; i++)
		res.push_back(add);
	scalar->change_matrix(res);
	scalar->set_type("m_" + fir_tp);
	init_three(scalar, strs, cols);
}

void coord(DefaultNode* var_matr, DefaultNode*& str, DefaultNode*& col) {
	std::string fir_tp, sec_tp, thir_tp;
	err code = OK;
	if (var_matr->get_empty()) {
		yyerror("This is an undefined variable");
		return;
	}
	var_matr->get_type(fir_tp);
	if (fir_tp[0] != 'm') {
		yyerror("Incorrect variable: the variable must be a matrix");
		return;
	}
	str->get_type(sec_tp); col->get_type(thir_tp);
	if ((thir_tp != "int") || (sec_tp != "int")) {
		yyerror("Second and third parameters must be a scalars");
		return;
	}
	int ind_str = str->get_elem(code), ind_col = col->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	int res = var_matr->get_elem(code, ind_str, ind_col);
	if (code != OK) {
		error_type(code);
		return;
	}
	var_matr->change_info(res, code);
	std::string dop_str;
	dop_str.append(fir_tp.cbegin() + 2, fir_tp.cend());
	var_matr->set_type(dop_str);
	init_three(var_matr, str, col);
}

void decl(DefaultNode* var_name, DefaultNode*& type, DefaultNode*& expr) {
	std::string new_tp, sec_tp;
	err code = OK;
	type->get_type(new_tp);
	var_name->set_type(new_tp);
	expr->get_type(sec_tp);
	if (new_tp != sec_tp) {
		yyerror("Incorrect type of variable");
		return;
	}
	int elem = expr->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	var_name->change_info(elem, code);
	if (code != OK) {
		error_type(code);
		return;
	}
	delete type, expr;
	type = expr = nullptr;
}

void v_decl(DefaultNode* var_name, DefaultNode*& type, DefaultNode*& expr) {
	std::string new_tp, sec_tp;
	err code = OK;
	type->get_type(new_tp);
	new_tp = "v_" + new_tp;
	var_name->set_type(new_tp);
	expr->get_type(sec_tp);
	if (new_tp != sec_tp) {
		yyerror("Incorrect type of variable");
		return;
	}
	std::vector<int> elems;
	expr->get_vect(elems);
	var_name->change_vector(elems);
	delete type, expr;
	type = expr = nullptr;
}

void m_decl(DefaultNode* var_name, DefaultNode*& type, DefaultNode*& expr) {
	std::string new_tp, sec_tp;
	err code = OK;
	type->get_type(new_tp);
	new_tp = "m_" + new_tp;
	var_name->set_type(new_tp);
	expr->get_type(sec_tp);
	if (new_tp != sec_tp) {
		yyerror("Incorrect type of variable");
		return;
	}
	std::vector<std::vector<int>> elems;
	expr->get_matr(elems);
	var_name->change_matrix(elems);
	delete type, expr;
	type = expr = nullptr;
}

void cr_func(DefaultNode* func_name, DefaultNode* related_vars, DefaultNode* e_params, DefaultNode* params, DefaultNode* sent_list) {
	std::string name = func_name->get_var_name();
	auto search = func_sentenses.find(name);
	if ((search != func_sentenses.end()) || (name == "left") || (name == "right") || (name == "move") || (name == "wall") || (name == "exit")) {
		yyerror("Incorrect name of function: function with this name has already been created");
		return;
	}
	func_sentenses.insert({ name, sent_list });
	std::vector<DefaultNode*> new_vars;
	related_vars->get_expr_list(new_vars);
	std::string var_name = related_vars->get_var_name();
	related_vars->change_const(false);
	related_vars->change_empty(true);
	for (int last_func_ind = func_variables.size() - 1; last_func_ind >= 0; last_func_ind--) {
		auto search_var = func_variables[last_func_ind].find(var_name);
		if (search_var != func_variables[last_func_ind].end()) {
			yyerror("Incorrect name of first returnable variable: variable with this name has already been created");
			func_sentenses.erase(name);
			return;
		}
	}
	func_variables[0].insert({ var_name, related_vars });
	int count = 2;
	for (auto var : new_vars) {
		var_name = var->get_var_name();
		var->change_const(false);
		var->change_empty(true);
		for (int last_func_ind = func_variables.size() - 1; last_func_ind >= 0; last_func_ind--) {
			auto search_var = func_variables[last_func_ind].find(var_name);
			if (search_var != func_variables[last_func_ind].end()) {
				std::string dop = "Incorrect name of " + std::to_string(count) + " returnable variable: variable with this name has already been created";
				yyerror(dop.c_str());
				func_sentenses.erase(name);
				return;
			}
		}
		func_variables[0].insert({ var_name, var });
		count++;
	}
	std::vector<DefaultNode*> all_params;
	if (e_params) {
		std::vector<DefaultNode*> add;
		all_params.push_back(e_params);
		e_params->get_expr_list(add);
		all_params.insert(all_params.cend(), add.cbegin(), add.cend());
		for(auto par: all_params)
			par->change_empty(true);
	}
	if (params) {
		std::vector<DefaultNode*> add;
		all_params.push_back(params);
		params->get_expr_list(add);
		all_params.insert(all_params.cend(), add.cbegin(), add.cend());
	}
	func_params.insert({ name, all_params});
	related_vars->clear_exprs();
}

DefaultNode* four_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* fir_leaf = nullptr, * sec_leaf = nullptr, * thir_leaf = nullptr, * fourth_leaf = nullptr;
	fir_leaf = bypassTree(par->get_node(0)), sec_leaf = bypassTree(par->get_node(1)), 
		thir_leaf = bypassTree(par->get_node(2)), fourth_leaf = par->get_node(3);
	par->get_type(tp_action);
	SWITCH(tp_action) {
		CASE("cr_func2") :
			fourth_leaf = par->get_node(3);
			cr_func(sec_leaf, fir_leaf, nullptr, thir_leaf, fourth_leaf);
			break;
		CASE("cr_func3") :
			cr_func(sec_leaf, fir_leaf, thir_leaf, nullptr, fourth_leaf);
			break;
		CASE("for") :
			do_for(fir_leaf, sec_leaf, thir_leaf, fourth_leaf);
			break;
		default:
			throw std::logic_error("Incorrect number of parameters");
			break;
	}
	return fir_leaf;
}

void do_for(DefaultNode*& name_scal, DefaultNode*& start_val, DefaultNode*& end_val, DefaultNode* sent_list) {
	std::string start_tp, end_tp;
	err code = OK;
	start_val->get_type(start_tp);
	end_val->get_type(end_tp);
	if ((start_tp != "int") || (end_tp != "int")) {
		yyerror("Incorrect type of start or end value");
		return;
	}
	int start = start_val->get_elem(code), end;
	if (code != OK) {
		error_type(code);
		return;
	}
	end = end_val->get_elem(code);
	if (code != OK) {
		error_type(code);
		return;
	}
	name_scal->change_const(false);
	name_scal->change_empty(false);
	name_scal->set_type("int");
	std::map<std::string, DefaultNode*> add;
	for (int i = start; i < end; i++) {
		name_scal->change_info(i, code);
		if (code != OK) {
			error_type(code);
			return;
		}
		add.insert({ name_scal->get_var_name(), name_scal });
		func_variables.push_back(add);
		auto ptr = bypassTree(sent_list);
		int last = func_variables.size() - 1;
		for (auto it : func_variables[last])
			if(it.second != name_scal)
				delete it.second;
		func_variables.pop_back();
		add.clear();
	}
	delete start_val, end_val, name_scal;
	start_val = end_val = nullptr;
	name_scal = sent_list;
}

DefaultNode* five_nodes(DefaultNode* par) {
	std::string tp_action;
	int num = 0;
	DefaultNode* fir_leaf = nullptr, * sec_leaf = nullptr, * thir_leaf = nullptr, * fourth_leaf = nullptr, * fifth_leaf = nullptr;
	par->get_type(tp_action);
	if (tp_action == "cr_func1") {
		fir_leaf = bypassTree(par->get_node(0)), sec_leaf = bypassTree(par->get_node(1)), thir_leaf = bypassTree(par->get_node(2)),
			fourth_leaf = bypassTree(par->get_node(3)), fifth_leaf = par->get_node(4);
		cr_func(sec_leaf, fir_leaf, thir_leaf, fourth_leaf, fifth_leaf);
	}
	else throw std::logic_error("Incorrect number of parameters");
	return fir_leaf;
}

void read_file() {
	std::string line;
	std::ifstream for_robot("robot3.txt");
	if (for_robot.is_open())
	{
		std::getline(for_robot, line);
		std::stringstream buf;
		buf << line;
		int x_sz = 0, y_sz = 0, x_coord = 0, y_coord = 0;
		char dir;
		buf >> x_sz >> y_sz >> dir >> y_coord >> x_coord;
		std::vector<std::vector<objects>> my_map;
		std::vector<objects> add;
		for(int y = 0; y < y_sz; y++)
		{
			std::getline(for_robot, line);
			for(int x = 0; x < x_sz; x++)
				switch (line[x]) {
				case '#':
					add.push_back(WALL);
					break;
				case '*':
					add.push_back(EMPTY);
					break;
				case '|':
					add.push_back(EXIT);
					break;
				}
			my_map.push_back(add);
			add.clear();
		}
		switch (dir) {
		case 'N':
			dir = NORTH;
			break;
		case 'E':
			dir = EAST;
			break;
		case 'W':
			dir = WEST;
			break;
		case 'S':
			dir = SOUTH;
			break;
		}
		my_robot = new Robot(x_coord, y_coord, directions(dir), my_map);
	}
	for_robot.close();
}

void yyerror(const std::string& str) {
	std::cerr << str << std::endl;
}

void error_type(err code) {
	switch (code) {
	case IND_VECT:
		yyerror("Incorrect index of vector");
		break;
	case IND_STR_MATR:
		yyerror("Incorrect index of str in matrix");
		break;
	case IND_COL_MATR:
		yyerror("Incorrect index of column in matrix");
		break;
	case EMPTY_STR_MATR:
		yyerror("Incorrect matrix: new matrix should have non-empty strings");
		break;
	case NON_RECT:
		yyerror("Incorrect matrix: new matrix should be rectangular");
		break;
	case EMPTY_MATR:
		yyerror("Incorrect matrix: it was empty");
		break;
	case EMPTY_VECT:
		yyerror("Incorrect vector: it was empty");
		break;
	default:
		break;
	}
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
	is_empty = old_node->get_empty();
	name = old_node->get_var_name();
	old_node->get_matr(m_val);
	old_node->get_vect(v_val);
	old_node->get_type(my_type);
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
			err_code = IND_VECT;
			return 0;
		}
		return v_val[ind_str];
	}
	auto [sz_str, sz_col] = get_size_matr();
	if ((ind_str >= sz_str) || (ind_str < 0)) {
		err_code = IND_STR_MATR;
		return 0;
	}
	if ((ind_col >= sz_col) || (ind_col < 0)) {
		err_code = IND_COL_MATR;
		return 0;
	}
	return m_val[ind_str][ind_col];
}

bool DataNode::get_str(int ind_str, std::vector<int>& result, err& err_code) const {
	int sz_str = m_val.size();
	if ((ind_str < sz_str) && (ind_str >= 0)) {
		result = m_val[ind_str];
		return true;
	}
	else err_code = IND_STR_MATR;
	return false;
}

bool DataNode::get_col(int ind_col, std::vector<int>& res, err& err_code) const {
	auto [sz_str, sz_col] = get_size_matr();
	if ((ind_col >= sz_col) || (ind_col < 0)) {
		err_code = IND_COL_MATR;
		return false;
	}
	res.clear();
	for (int i = 0; i < sz_str; i++)
		res.push_back(m_val[i][ind_col]);
	return true;
}

bool DataNode::get_log_matr(const std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& res, err& err_code) const {
	int sz_str = matr.size();
	std::vector<std::vector<int>> new_res;
	if (sz_str) {
		int sz_col = matr[0].size();
		if (sz_col) {
			int zero_sz = 0;
			for (int ind_col = 0; ind_col < sz_col; ind_col++) {
				std::vector<int> add;
				for (int ind_str = 0; ind_str < sz_str; ind_str++)
					if (matr[ind_str][ind_col])
						add.push_back(m_val[ind_str][ind_col]);
				if (!add.empty())
					new_res.push_back(add);
			}
			sz_str = new_res.size();
			if(!sz_str) {
				err_code = EMPTY_STR_MATR;
				return false;
			}
			zero_sz = new_res[0].size();
			for(int i = 1; i < sz_str; i++)
				if(zero_sz != new_res[i].size()) {
					err_code = NON_RECT;
					return false;
				}
			res = new_res;
			return true;
		}
		else err_code = EMPTY_STR_MATR;
	}
	else err_code = EMPTY_MATR;
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
		err_code = IND_VECT;
		return false;
	}
	else sz = m_val.size();
	if ((ind_str < 0) || (ind_str >= sz)) {
		err_code = IND_STR_MATR;
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