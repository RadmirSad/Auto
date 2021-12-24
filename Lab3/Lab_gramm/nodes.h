#include <typeinfo>
#include <map>
#include "str_switch.h"
#include "Game.h"
// #include "y.tab.h"

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
void yyerror(const std::string& str);
int exec(DefaultNode* par);
void delTree(DefaultNode*& par);
DefaultNode* bypassTree(DefaultNode* par);
void init();

void read_file();
void init_map_and_robot();

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

	/* Data methods */
	
	virtual std::pair<int, int> get_size_matr() const { return std::pair<int, int>(-1,-1); }
	virtual bool is_var() const { return false; }
	virtual std::string get_var_name() const { return ""; }
	virtual bool get_const() const { return false; }

	virtual int get_elem(err& err_code, int ind_str = -1, int ind_col = -1) const { return 0; }

	virtual bool get_str(int ind_str, std::vector<int>& result, err& err_code) const { return false; }
	virtual bool get_col(int ind_col, std::vector<int>& result, err& err_code) const { return false; }

	virtual void get_vect(std::vector<int>& vect) const { ; }
	virtual bool get_matr(std::vector<std::vector<int>>& result) const { return false; }
	virtual bool get_log_matr(const std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& result, err& err_code) const { return false; }

	virtual bool push_back_info(int new_val) { return false; }
	virtual bool push_back_info(const std::vector<int>& new_val, err& err_code) { return false; }
	virtual bool change_info(int new_val, err& err_code, int ind_str = -1, int ind_col = -1) { return false; }
	
	/* Operator methods */

	virtual DefaultNode* get_node(int index) { return nullptr; }
	virtual bool push_back_node(DefaultNode* node) { return false; }
	virtual bool change_info(DefaultNode* new_node, int index) { return false; }

};

extern std::map<std::string, DefaultNode*> variables;

DefaultNode* zero_nodes(DefaultNode* par);
DefaultNode* one_node(DefaultNode* par);
DefaultNode* two_nodes(DefaultNode* par);
DefaultNode* three_nodes(DefaultNode* par);
DefaultNode* four_nodes(DefaultNode* par);
DefaultNode* five_nodes(DefaultNode* par);
void sum(DefaultNode* ptr);
void transpon(DefaultNode* matrix);

class DataNode: public DefaultNode {
private:
	int val = 0;
	std::vector<int> v_val;
	std::vector<std::vector<int>> m_val;
	bool is_const = false;
	std::string name;

	std::vector<DefaultNode*> expr_list;
public:
	DataNode() : DefaultNode() {};
	DataNode(int new_val) : DefaultNode("int"), val(new_val) {};
	DataNode(bool new_val) : DefaultNode("bool"), val(new_val) {};
	DataNode(DefaultNode* new_node);

	virtual bool is_var() const override { return name.empty(); }
	virtual bool get_const() const override { return is_const; }

	virtual std::string get_var_name() const override { return name; }
	virtual int get_size_vect() const override { return v_val.size(); }
	virtual std::pair<int, int> get_size_matr() const override;
	virtual int get_elem(err& err_code, int ind_str = -1, int ind_col = -1) const override;
	virtual void get_vect(std::vector<int>& vect) const override { vect = v_val; }

	virtual bool get_str(int ind_str, std::vector<int>& result, err& err_code) const override;
	virtual bool get_col(int ind_col, std::vector<int>& result, err& err_code) const override;

	virtual bool get_matr(std::vector<std::vector<int>>& result) const override { result = m_val; }
	virtual bool get_log_matr(const std::vector<std::vector<int>>& matr, std::vector<std::vector<int>>& res, err& err_code) const override;

	virtual bool push_back_info(int new_val) override;
	virtual bool push_back_info(const std::vector<int>& new_val, err& err_code) override;
	virtual bool change_info(int new_val, err& err_code, int ind_str = -1, int ind_col = -1) override;
	virtual bool delete_vect_info(int index, err& err_code, bool is_matr = false) override;
	virtual void delete_vect(bool is_matr = false) override;
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