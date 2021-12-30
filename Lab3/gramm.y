%{
	#include <cstdarg>
%}

%require "3.0.4"
%defines "Parser.hpp"
%define api.value.type {union YYSTYPE}
%output "Parser.cpp"
%locations

%code requires {
	#include "nodes.h"
	#define YYDEBUG 1
}

%code provides {
	extern FILE * yyin;
	extern int yylex();
	extern void yyerror(const std::string& str);
	void init();
	void delAll();
	int exec(DefaultNode* par);
	void delTree(DefaultNode*& par);
	
	DefaultNode* create_data(int info);
	DefaultNode* create_data(bool info);

	DefaultNode* oper(const std::string& type_of_oper, int num,...);
	DefaultNode* get_var(std::string* str);
	DefaultNode* get_type(std::string* new_type);
	DefaultNode* find_var(DefaultNode* var);
}

%token <bVal> BOOL_VAL
%token <iVal> NUMBER
%token <sVal> NAME TYPE
%token <iVal> CONST VECT MATRIX
%token <iVal> FOR IF BEG BEGIN_FOR BEGIN_IF END END_IF END_FOR WORD_FUNC
%token <iVal> SUM SR SL AND ELEM_MULT ASSIGN TRANSPON ENDL

%token <iVal> MOVE WALL_FUNC EXIT_FUNC RIGHT LEFT PRINT SIZE PUSH MAP_FUNC

%precedence ASSIGN
%left '>' '<' AND
%left '+' 
%left '-'
%left '*' ELEM_MULT
%precedence SR SL
%precedence TRANSPON
%precedence '!'
%precedence PUSH

%type <dNode> data expr var_declar argument_list empty_var_declar expr_matr_or_vect
%type <dNode> empty_argument_list function_actions sent sent_list program error

%%

program:
		sent_list						{ init(); exec($1); delTree($1); delAll(); exit(0);}
		| %empty						{ $$ = nullptr; }
		;

sent_list:
		sent						{ $$ = $1; }
		| sent_list sent			{ $$ = oper("endl", 2, $1, $2); }
		;

sent:
		var_declar ENDL						{ $$ = oper("new_decl", 1, $1); }
		| CONST var_declar ENDL				{ $$ = oper("const", 1, $2); }
		| function_actions ENDL				{ $$ = oper("func_act", 1, $1); }
		| FOR NAME '=' expr ':' expr ENDL beg_for ENDL sent_list end_for ENDL			{ $$ = oper("for", 4, get_var($2), $4, $6, $10); }
		| IF expr ENDL beg_if ENDL sent_list end_if ENDL								{ $$ = oper("if", 2, $2, $6); }
		| expr ENDL							{ $$ = oper("new_expr", 1, $1); }
		| LEFT ENDL							{ $$ = oper("left", 0); }
		| RIGHT	ENDL						{ $$ = oper("right", 0); }
		| PRINT '(' expr ')' ENDL			{ $$ = oper("print", 1, $3); }
		;

beg_for:
		BEGIN_FOR
		| BEG
		;

beg_if:
		BEGIN_IF
		| BEG
		;

end_for:
		END_FOR
		| END
		;

end_if:
		END_IF
		| END
		;

function_actions:
	'[' empty_argument_list ']' '=' WORD_FUNC NAME '(' empty_argument_list ',' argument_list ')' ENDL BEG ENDL sent_list END	{ $$ = oper("cr_func1", 5, $2, get_var($6), $8, $10, $15); }
	| '[' empty_argument_list ']' '=' WORD_FUNC NAME '(' argument_list ')' ENDL BEG ENDL sent_list END						{ $$ = oper("cr_func2", 4, $2, get_var($6), $8, $13); }
	| '[' empty_argument_list ']' '=' WORD_FUNC NAME '(' empty_argument_list ')' ENDL BEG ENDL sent_list END					{ $$ = oper("cr_func3", 4, $2, get_var($6), $8, $13); }
	| '[' empty_argument_list ']' '=' WORD_FUNC NAME '(' ')' ENDL BEG ENDL sent_list END										{ $$ = oper("cr_func4", 3, $2, get_var($6), $12); }
	| NAME '[' expr_matr_or_vect ']'		{ $$ = oper("call", 2, get_var($1), $3); }
	| NAME '[' ']'							{ $$ = oper("e_call", 1, get_var($1)); }
	;

empty_argument_list:
		empty_var_declar 								{ $$ = $1; }
		| empty_argument_list ',' empty_var_declar		{ $$ = oper(",", 2, $1, $3); }
		;

empty_var_declar:
		MATRIX TYPE NAME				{ $$ = oper("m_e_decl", 2, get_type($2), get_var($3)); }
		| VECT TYPE NAME				{ $$ = oper("v_e_decl", 2, get_type($2), get_var($3)); }
		| TYPE NAME						{ $$ = oper("e_decl", 2, get_type($1), get_var($2)); }
		;

argument_list:
		var_declar						{ $$ = $1; }
		| argument_list ',' var_declar	{ $$ = oper(",", 2, $1, $3); }
		;

var_declar:
		MATRIX TYPE NAME '=' expr		{ $$ = oper("m_decl", 3, get_type($2), get_var($3), $5); }
		| VECT TYPE NAME '=' expr		{ $$ = oper("v_decl", 3, get_type($2), get_var($3), $5); }
		| TYPE NAME '=' expr			{ $$ = oper("decl", 3, get_type($1), get_var($2), $4); }
		;


expr_matr_or_vect:
		expr							{ $$ = $1; }
		| expr_matr_or_vect ',' expr	{ $$ = oper(",", 2, $1, $3); }
		;

expr:
		data							{ $$ = $1; }
		| '{' expr_matr_or_vect '}'		{ $$ = oper("matr_vect", 1, $2); }
		| NAME							{ $$ = find_var(get_var($1)); }
		| NAME '(' expr ',' expr ')'	{ $$ = oper("coord", 3, find_var(get_var($1)), $3, $5); }
		| MOVE '(' expr ')'				{ $$ = oper("move", 1, $3); }
		| MAP_FUNC						{ $$ = oper("get_map", 0); }
		| WALL_FUNC						{ $$ = oper("wall", 0); }
		| EXIT_FUNC						{ $$ = oper("exit", 0); }
		| expr '+' expr					{ $$ = oper("+", 2, $1, $3); }
		| expr '-' expr					{ $$ = oper("-", 2, $1, $3); }
		| expr SL						{ $$ = oper("sl", 1, $1); }
		| expr SR						{ $$ = oper("sr", 1, $1); }
		| expr '<' expr					{ $$ = oper("lt", 2, $1, $3); }
		| expr '>' expr					{ $$ = oper("gt", 2, $1, $3); }
		| expr AND expr					{ $$ = oper("and", 2, $1, $3); }
		| '!' expr						{ $$ = oper("not", 1, $2); }
		| expr ELEM_MULT expr			{ $$ = oper("el_mul", 2, $1, $3); }
		| expr '*' expr					{ $$ = oper("mul", 2, $1, $3); }
		| expr TRANSPON					{ $$ = oper("transpon", 1, $1); }
		| NAME '(' expr ',' '[' ':' ']' ')'	{ $$ = oper("get_col", 2, find_var(get_var($1)), $3); }
		| NAME '(' expr ',' '[' ']' ')'		{ $$ = oper("get_col", 2, find_var(get_var($1)), $3); }
		| NAME '(' '[' ':' ']' ',' expr ')'	{ $$ = oper("get_str", 2, find_var(get_var($1)), $7); }
		| NAME '(' '[' ']' ',' expr ')'		{ $$ = oper("get_str", 2, find_var(get_var($1)), $6); }
		| NAME '(' expr ')'				{ $$ = oper("log_matr", 2, find_var(get_var($1)), $3); }
		| SIZE '(' expr ')'				{ $$ = oper("get_size", 1, $3); }
		| SUM '(' expr ')'				{ $$ = oper("sum", 1, $3); }
		| NAME ASSIGN expr				{ $$ = oper("eq", 2, get_var($1), $3); }
		| NAME PUSH expr				{ $$ = oper("push", 2, get_var($1), $3); }
		| TYPE '(' expr ')'				{ $$ = oper("ch_tp", 2, get_type($1), $3); }
		| VECT '(' expr ',' expr ')'	{ $$ = oper("make_v", 2, $3, $5); }
		| MATRIX '(' expr ',' expr ',' expr')'	{ $$ = oper("make_m1", 3, $3, $5, $7); }
		| MATRIX '(' expr ',' expr ')'	{ $$ = oper("make_m2", 2, $3, $5); }
		;
		
data:
		BOOL_VAL						{ $$ = create_data($1); }
		| NUMBER						{ $$ = create_data($1); }
		;

%%
// {	std::cout << "Error at line: " << @2.first_line << std::endl;	}
/*
	
*/

int exec(DefaultNode* par) {
	bypassTree(par);
}

void init() {
	read_file();
	std::map<std::string, DefaultNode*> adder;
	func_variables.push_back(adder);
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

void delAll() {
	for (auto mp : func_variables)
		for (auto pr : mp)
			delete pr.second;
	func_variables.clear();
	for (auto pr : func_params)
		for (auto it : pr.second)
			delete it;
	func_params.clear();
	func_sentenses.clear();
}

DefaultNode* create_data(bool info) {
	DefaultNode* dt = nullptr;
	std::cout << info << std::endl;
	dt = new DataNode(info);
	if(dt == nullptr)
		yyerror("Out of memory");
	return dt;
}

DefaultNode* create_data(int info) {
	DefaultNode* dt = nullptr;
	dt = new DataNode(info);
	if(dt == nullptr)
		yyerror("Out of memory");
	return dt;
}

DefaultNode* oper(const std::string& type_of_oper, int num,...) {
	va_list argptr;
	va_start (argptr, num);
	DefaultNode* new_oper = new OperNode(type_of_oper);
	for(; num; num--) {
		auto add = va_arg(argptr, DefaultNode*);
		bool flag = new_oper->push_back_node(add);
		add->set_parent(new_oper);
	}
	va_end(argptr);
	return new_oper;
}

DefaultNode* get_type(std::string* new_type) {
	DefaultNode* type_node = new DataNode;
	type_node->set_type(*new_type);
	delete new_type;
	return type_node;
}

DefaultNode* get_var(std::string* str) {
	DefaultNode* var = new DataNode;
	var->set_var_name(*str);
	delete str;
	return var;
}

DefaultNode* find_var(DefaultNode* var) {
	return oper("get_var", 1, var);
}

// bison -d --debug gramm.y --verbose && flex -d tokenize.l && g++ -std=c++17 -o lab3 Parser.cpp Scanner.cpp nodes.cpp Game.cpp

int main() {
	yyin = fopen("maze.txt", "r");
	yyparse();
	fclose(yyin);
	return 0;
}