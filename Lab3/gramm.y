%require "3.2"
%language "c++"

%{
	#include <cstdarg>
	#include "Lab_gramm/actions.cpp"
	
	int yylex();
	int yyparse();
	
	template <typename T>
	DefaultNode* create_data(T info);

	DefaultNode* oper(const std::string& type_of_oper, int num,...);
	DefaultNode* upg_dim(DefaultNode* node);
	
	std::map<std::string, DefaultNode*> variables;
	
	Robot* my_robot = nullptr;
%}

%union {
	bool bVal;
	int iVal;
	std::string* sVal;
	DefaultNode* dNode;
};

%token <bVal> BOOL_VAL
%token <iVal> NUMBER
%token <sVal> NAME TYPE
%token CONST VECT MATRIX
%token FOR IF BEGIN BEGIN_FOR BEGIN_IF END END_IF END_FOR WORD_FUNC
%token SUM SR SL AND ELEM_MULT ASSIGN TRANSPON ENDL

%token MOVE WALL EXIT RIGHT LEFT

%left '>' '<' AND
%left '+' '-'
%left '*' ELEM_MULT
%left SR SL
%left TRANSPON
%left '!'

%type <dNode> data expr var_declar argument_list empty_var_declar expr_matr_or_vect
%type <dNode> empty_argument_list function_actions sent sent_list program error

%%

program:
		sent_list						{ exec($1); delTree($1); exit(0);}
		| 								{ $$ = nullptr; init(); }		
		;

sent_list:
		sent						{ $$ = $1; }
		| sent_list sent			{ $$ = oper("endl", 2, $1, $2); }
		;

sent:
		var_declar ENDL						{ $$ = $1; }
		| CONST var_declar ENDL				{ $$ = oper("const", 1, $2); }
		| function_actions ENDL				{ $$ = oper("func", 1, $1); }
		| FOR NAME '=' expr ':' expr ENDL beg_for ENDL sent_list ENDL end_for ENDL			{ $$ = oper("for", 4, $2, $4, $6, $10); }
		| IF expr ENDL beg_if ENDL sent_list ENDL end_if ENDL								{ $$ = oper("if", 2, $2, $6); }
		| expr ENDL							{ $$ = $1; }
		| LEFT ENDL							{ $$ = oper("left", 0); }
		| RIGHT	ENDL						{ $$ = oper("right", 0); }
		;

beg_for:
		BEGIN_FOR
		| BEGIN
		;

beg_if:
		BEGIN_IF
		| BEGIN
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
	'[' empty_argument_list ']' '=' WORD_FUNC NAME '(' empty_argument_list ',' argument_list ')' ENDL BEGIN ENDL sent_list END	{ $$ = oper("cr_func1", 5, $2, $6, $8, $10, $15); }
	| '[' empty_argument_list ']' '=' WORD_FUNC NAME '(' argument_list ')' ENDL BEGIN ENDL sent_list END						{ $$ = oper("cr_func2", 4, $2, $6, $8, $10, $13); }
	| '[' empty_argument_list ']' '=' WORD_FUNC NAME '(' empty_argument_list ')' ENDL BEGIN ENDL sent_list END					{ $$ = oper("cr_func3", 4, $2, $6, $8, $10, $13); }
	| '[' empty_argument_list ']' '=' WORD_FUNC NAME '(' ')' ENDL BEGIN ENDL sent_list END										{ $$ = oper("cr_func4", 3, $2, $6, $8, $10, $12); }
	| NAME '[' expr_matr_or_vect ']'		{ $$ = oper("call", 2, $1, $3); }
	| NAME '[' ']'							{ $$ = oper("e_call", 1, $1); }
	;

empty_argument_list:
		empty_var_declar 								{ $$ = $1; }
		| empty_argument_list ',' empty_var_declar		{ $$ = oper(",", 2, $1, $3); }
		;

empty_var_declar:
		MATRIX TYPE NAME				{ $$ = oper("m_e_decl", 2, $2, $3); }
		| VECT TYPE NAME				{ $$ = oper("v_e_decl", 2, $2, $3); }
		| TYPE NAME						{ $$ = oper("e_decl", 2, $1, $2); }
		;

argument_list:
		var_declar						{ $$ = $1; }
		| argument_list ',' var_declar	{ $$ = oper(",", 2, $1, $3); }
		;

var_declar:
		MATRIX TYPE NAME '=' expr		{ $$ = oper("m_decl", 3, $2, $3, $5); }
		| VECT TYPE NAME '=' expr		{ $$ = oper("v_decl", 3, $2, $3, $5); }
		| TYPE NAME '=' expr			{ $$ = oper("decl", 3, $1, $2, $4); }
		;


expr_matr_or_vect:
		expr							{ $$ = $1; }
		| expr_matr_or_vect ',' expr	{ $$ = oper(",", 2, $1, $3); }
		;

expr:
		data							{ $$ = $1; }
		| '{' expr_matr_or_vect '}'		{ $$ = oper("matr_vect", 1, $2); }
		| NAME							{ $$ = oper("get_var", 1, $1); }
		| NAME '(' expr ',' expr ')'	{ $$ = oper("coord", 3, $1, $3, $5); }
		| MOVE '(' expr ')'				{ $$ = oper("move", 1, $3); }
		| WALL							{ $$ = oper("wall", 0); }
		| EXIT							{ $$ = oper("exit", 0); }
		| expr '+' expr					{ $$ = oper("+", 2, $1, $3); }
		| expr '-' expr					{ $$ = oper("-", 2, $1, $3); }
		| expr SL						{ $$ = oper("sl", 1, $1); }
		| expr SR						{ $$ = oper("sr", 1, $1); }
		| expr'<' expr					{ $$ = oper("lt", 2, $1, $3); }
		| expr '>' expr					{ $$ = oper("gt", 2, $1, $3); }
		| expr AND expr					{ $$ = oper("and", 2, $1, $3); }
		| '!' expr						{ $$ = oper("not", 1, $2); }
		| NAME '(' expr ',' '[' ':' ']' ')'	{ $$ = oper("get_col", 2, $1, $3); }
		| NAME '(' expr ',' '[' ']' ')'		{ $$ = oper("get_col", 2, $1, $3); }
		| NAME '(' '[' ':' ']' ',' expr ')'	{ $$ = oper("get_str", 2, $1, $7); }
		| NAME '(' '[' ']' ',' expr ')'		{ $$ = oper("get_str", 2, $1, $6); }
		| expr ELEM_MULT expr			{ $$ = oper("el_mul", 2, $1, $3); }
		| expr '*' expr					{ $$ = oper("mul", 2, $1, $3); }
		| expr TRANSPON					{ $$ = oper("transpon", 1, $1); }
		| NAME '(' expr ')'				{ $$ = oper("log_matr", 2, $1, $3); }
		| SUM '(' expr ')'				{ $$ = oper("sum", 1, $3); }
		| NAME ASSIGN expr				{ $$ = oper("eq", 2, $1, $3); }
		| TYPE '(' expr ')'				{ $$ = oper("ch_tp", 2, $1, $3); }
		| VECT '(' expr ',' NUMBER ')'	{ $$ = oper("make_v", 2, $3, $5); }
		| MATRIX '(' expr ',' NUMBER ',' NUMBER ')'	{ $$ = oper("make_m1", 3, $3, $5, $7); }
		| MATRIX '(' expr ',' NUMBER ')'	{ $$ = oper("make_m2", 2, $3, $5); }
		;
		
data:
		BOOL_VAL						{ $$ = create_data($1); }
		| NUMBER						{ $$ = create_data($1); }
		;

%%

// {	std::cout << "Error at line: " << @2.first_line << std::endl;	}

template <typename T>
DefaultNode* create_data(T info) {
	std::string str = typeid(info).name();
	DefaultNode* dt = nullptr;
	if((str == "bool") || (str == "int")) {
		dt = new DataNode(info);
		if(dt == nullptr)
			yyerror("Out of memory");
	}
	else yyerror("Incorrect type of data");	
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

DefaultNode* upg_dim(DefaultNode* node) {
	std::string str, buf;
	node->get_type(str);
	buf = str[0];
	if(buf == "v") {
		buf = "m_";
		buf.append(str.cbegin() + 2, str.cend());
	}
	else if(buf == "m") {
		yyerror("Unknown dimension of data.");
		buf = str;
	}
	else {
		buf = "v_";
		buf.append(str);
	}
	node->set_type(buf);
	return node;
}

int main() {
	std::ifstream MyFile;
	MyFile.open("robot.txt");
	
	yyparse();
	fclose (yyin);
	return 0;
}