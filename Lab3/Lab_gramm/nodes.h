#include <typeinfo>
#include <map>
#include <fstream>
#include <sstream>
#include "str_switch.h"
#include "Game.h"

#define DEBUG

enum err {
	IND_VECT,
	IND_STR_MATR,
	IND_COL_MATR,
	EMPTY_STR_MATR,
	NON_RECT,
	EMPTY_MATR,
	EMPTY_VECT,
	OK
};

extern Robot* my_robot;

std::string what_type(const std::string& str);
void yyerror(const char* str);
void init();
void delAll();

void read_file();
void error_type(err code);

class DefaultNode {
protected:
	std::string my_type;
	bool is_oper = false;
	DefaultNode* par = nullptr;

public:
	DefaultNode() {};
	DefaultNode(const std::string& str, bool oper = false) : my_type(str), is_oper(oper) {};

	void get_type(std::string& result) const { result = my_type; }
	void set_type(const std::string& new_tp) { my_type = new_tp; }
	void set_parent(DefaultNode* new_par) { par = new_par; }
	void get_parent(DefaultNode*& ptr) { ptr = par; }

	bool check_oper() const { return is_oper; }

	/* General methods */

	virtual int get_size_vect() const = 0;
	virtual bool delete_vect_info(int, err& err_code, bool is_matr = false) = 0;
	virtual void delete_vect(bool is_matr = false) = 0;
	virtual bool push_back_node(DefaultNode* node) = 0;

	/* Data methods */
	
	virtual std::pair<int, int> get_size_matr() const { return std::pair<int, int>(-1,-1); }
	virtual bool is_var() const { return false; }
	virtual std::string get_var_name() const { return ""; }
	virtual bool get_const() const { return false; }
	virtual bool get_empty() const { return false; }

	virtual int get_elem(err& err_code, int ind_str = -1, int ind_col = -1) const { return 0; }

	virtual bool get_str(int ind_str, std::vector<int>& result, err& err_code) const { return false; }
	virtual bool get_col(int ind_col, std::vector<int>& result, err& err_code) const { return false; }

	virtual void get_vect(std::vector<int>& vect) const { ; }
	virtual bool get_matr(std::vector<std::vector<int>>& result) const { return false; }
	virtual bool get_log_matr(const std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& result, err& err_code) const { return false; }
	virtual bool get_expr_list(std::vector<DefaultNode*>& list) const { return false; }

	virtual bool set_var_name(const std::string& str) { return false; }

	virtual bool push_back_info(int new_val) { return false; }
	virtual bool push_back_info(const std::vector<int>& new_val, err& err_code) { return false; }
	virtual bool change_info(int new_val, err& err_code, int ind_str = -1, int ind_col = -1) { return false; }
	virtual bool change_const(bool new_val) { return false; }
	virtual bool change_empty(bool new_val) { return false; }
	virtual bool change_vector(const std::vector<int>& new_vect) { return false; }
	virtual bool change_matrix(const std::vector<std::vector<int>>& new_matrix) { return false; }

	virtual bool clear_exprs() { return false; }
	
	/* Operator methods */

	virtual DefaultNode* get_node(int index) { return nullptr; }
	virtual bool change_info(DefaultNode* new_node, int index) { return false; }

};

union YYSTYPE {
	bool bVal;
	int iVal;
	std::string* sVal;
	DefaultNode* dNode;
};

extern std::vector<std::map<std::string, DefaultNode*>> func_variables;
extern std::map<std::string, DefaultNode*> func_sentenses;
extern std::map<std::string, std::vector<DefaultNode*>> func_params;

void delTree(DefaultNode*& par);
DefaultNode* bypassTree(DefaultNode* par);

DefaultNode* zero_nodes(DefaultNode* par);
DefaultNode* one_node(DefaultNode* par);
DefaultNode* two_nodes(DefaultNode* par);
DefaultNode* three_nodes(DefaultNode* par);
DefaultNode* four_nodes(DefaultNode* par);
DefaultNode* five_nodes(DefaultNode* par);

/*			One ptr			*/

void sum(DefaultNode* ptr);
void transpon(DefaultNode* matrix);
void not_bool(DefaultNode* ptr);
void sl(DefaultNode* ptr);
void sr(DefaultNode* ptr);
void move(DefaultNode* ptr);
void get_variable(DefaultNode* ptr);
void matr_vect(DefaultNode* ptr);
void e_call(DefaultNode* ptr);
void new_decl(DefaultNode* var);
void e_call(DefaultNode* func_name);
void my_print(DefaultNode* elem);

/*			Two ptrs			*/

void init_two(DefaultNode* fir, DefaultNode*& sec);

void make_m2(DefaultNode* vect, DefaultNode*& new_sz);
void make_v(DefaultNode* scalar, DefaultNode*& new_sz);
void change_type(DefaultNode* expr, DefaultNode*& new_type);
void equality(DefaultNode* expr, DefaultNode*& new_val);
void log_matr(DefaultNode* var, DefaultNode*& logical_matr);
void mul(DefaultNode* fir_matr, DefaultNode*& sec_matr);
void el_mul(DefaultNode* fir_matr, DefaultNode*& sec_matr);
void get_str(DefaultNode* matr, DefaultNode*& index);
void get_col(DefaultNode* matr, DefaultNode*& index);
void do_and(DefaultNode* fir_log_expr, DefaultNode*& sec_log_expr);
void do_gt(DefaultNode* fir_ariph_expr, DefaultNode*& sec_ariph_expr);
void do_lt(DefaultNode* fir_ariph_expr, DefaultNode*& sec_ariph_expr);
void minus(DefaultNode* fir_ariphm_expr, DefaultNode*& sec_ariphm_expr);
void plus(DefaultNode* fir_ariphm_expr, DefaultNode*& sec_ariphm_expr);
void e_decl(DefaultNode* var, DefaultNode*& type); 
void v_e_decl(DefaultNode* var, DefaultNode*& type);
void m_e_decl(DefaultNode* var, DefaultNode*& type);
void call(DefaultNode* func, DefaultNode*& params);
void do_if(DefaultNode*& condit, DefaultNode* sent);

/*			Three ptrs			*/

void init_three(DefaultNode* fir, DefaultNode*& sec, DefaultNode*& third);

void make_m1(DefaultNode* scalar, DefaultNode*& strs, DefaultNode*& cols);
void coord(DefaultNode* var_matr, DefaultNode*& str, DefaultNode*& col);
void decl(DefaultNode* var_name, DefaultNode*& type, DefaultNode*& expr);
void v_decl(DefaultNode* var_name, DefaultNode*& type, DefaultNode*& expr);
void m_decl(DefaultNode* var_name, DefaultNode*& type, DefaultNode*& expr);

/*			Four and five ptrs			*/

void cr_func(DefaultNode* func_name, DefaultNode* related_vars, DefaultNode* e_params, DefaultNode* params, DefaultNode* sent_list);
void do_for(DefaultNode* name_scal, DefaultNode*& start_val, DefaultNode*& end_val, DefaultNode* sent_list);

class DataNode: public DefaultNode {
private:
	int val = 0;
	std::vector<int> v_val;
	std::vector<std::vector<int>> m_val;
	bool is_const = true;
	bool is_empty = false;
	std::string name;

	std::vector<DefaultNode*> expr_list;
public:
	DataNode() : DefaultNode() {};
	DataNode(int new_val) : DefaultNode("int"), val(new_val) {};
	DataNode(bool new_val) : DefaultNode("bool"), val(int(new_val)) {};
	DataNode(DefaultNode* new_node);

	virtual bool is_var() const override { return name.empty(); }
	virtual bool get_const() const override { return is_const; }
	virtual bool get_empty() const override { return is_empty; }

	virtual std::string get_var_name() const override { return name; }
	virtual int get_size_vect() const override { return v_val.size(); }
	virtual std::pair<int, int> get_size_matr() const override;
	virtual int get_elem(err& err_code, int ind_str = -1, int ind_col = -1) const override;
	virtual void get_vect(std::vector<int>& vect) const override { vect = v_val; }

	virtual bool get_str(int ind_str, std::vector<int>& result, err& err_code) const override;
	virtual bool get_col(int ind_col, std::vector<int>& result, err& err_code) const override;

	virtual bool get_matr(std::vector<std::vector<int>>& result) const override { result = m_val; return true; }
	virtual bool get_log_matr(const std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& res, err& err_code) const override;
	virtual bool get_expr_list(std::vector<DefaultNode*>& list) const override { list = expr_list; return true; }


	virtual bool set_var_name(const std::string& str) override { name = str; return true; }
	virtual bool push_back_info(int new_val) override;
	virtual bool push_back_info(const std::vector<int>& new_val, err& err_code) override;
	virtual bool change_info(int new_val, err& err_code, int ind_str = -1, int ind_col = -1) override;
	virtual bool change_const(bool new_val) override { is_const = new_val; return true; }
	virtual bool change_empty(bool new_val) override { is_empty = new_val; return true; }
	virtual bool change_vector(const std::vector<int>& new_vect) override { v_val = new_vect; return true; }
	virtual bool change_matrix(const std::vector<std::vector<int>>& new_matrix) override { m_val = new_matrix; return true; }
	virtual bool delete_vect_info(int index, err& err_code, bool is_matr = false) override;
	virtual void delete_vect(bool is_matr = false) override;
	virtual bool push_back_node(DefaultNode* node) override { expr_list.push_back(node); return true; }

	virtual bool clear_exprs() override { expr_list.clear(); return true; }
};

class OperNode : public DefaultNode {
private:
	std::vector<DefaultNode*> nodes;
public:
	OperNode() : DefaultNode() {};
	OperNode(const std::string& tp_of_oper) : DefaultNode(tp_of_oper, true) {};

	virtual int get_size_vect() const override { return nodes.size(); }
	virtual DefaultNode* get_node(int index) override;

	virtual bool push_back_node(DefaultNode* node) override;
	virtual void delete_vect(bool is_matr = false) override;
	virtual bool delete_vect_info(int index, err& err_code, bool is_matr = false) override;
	virtual bool change_info(DefaultNode* new_node, int index) override;
};