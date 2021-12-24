%option yylineno
%option c++

%{
	#include <iostream>
	#include "y.tab.h"
	#include "robot.h"
	
	void yyerrror(const std::string&);
%}

%%

([ \t]+)|(\.\.\.( )*\n)|(\[:?\])	{	yylloc.last_line = yylloc.first_line = yylineno;				}

"\n"						{	yylloc.last_line = yylloc.first_line = yylineno; return ENDL;	}

"<<"						{	yylloc.last_line = yylloc.first_line = yylineno; return SL;		}
">>"						{	yylloc.last_line = yylloc.first_line = yylineno; return SR;		}
"and"|"&&"					{	yylloc.last_line = yylloc.first_line = yylineno; return AND;	}
"for"						{	yylloc.last_line = yylloc.first_line = yylineno; return FOR;	}
"if"						{	yylloc.last_line = yylloc.first_line = yylineno; return IF;		}
"begin"						{	yylloc.last_line = yylloc.first_line = yylineno; return BEGIN;	}
"begin[if]"					{	yylloc.last_line = yylloc.first_line = yylineno; return BEGIN_IF;	}
"begin[for]"				{	yylloc.last_line = yylloc.first_line = yylineno; return BEGIN_FOR;	}
"end"						{	yylloc.last_line = yylloc.first_line = yylineno; return END;	}
"end[if]"					{	yylloc.last_line = yylloc.first_line = yylineno; return END_IF;	}
"end[for]"					{	yylloc.last_line = yylloc.first_line = yylineno; return END_FOR;	}

"int"|"bool"				{
								yylloc.last_line = yylloc.first_line = yylineno;
								yylval.sVal = new std::string(yytext);
								return TYPE;	
							}
							
"[c]"					{	yylloc.last_line = yylloc.first_line = yylineno; return CONST;	}
"[v]"					{	yylloc.last_line = yylloc.first_line = yylineno; return VECT;	}
"[m]"					{	yylloc.last_line = yylloc.first_line = yylineno; return MATRIX;	}
"<-"					{	yylloc.last_line = yylloc.first_line = yylineno; return ASSIGN;	}
"sum"					{	yylloc.last_line = yylloc.first_line = yylineno; return SUM;	}
"move"					{	yylloc.last_line = yylloc.first_line = yylineno; return MOVE;	}
"left"					{	yylloc.last_line = yylloc.first_line = yylineno; return LEFT;	}
"right"					{	yylloc.last_line = yylloc.first_line = yylineno; return RIGHT;	}
"exit"					{	yylloc.last_line = yylloc.first_line = yylineno; return EXIT;	}
"wall"					{	yylloc.last_line = yylloc.first_line = yylineno; return WALL;	}
"function"				{	yylloc.last_line = yylloc.first_line = yylineno; return WORD_FUNC;	}

"true"|"false"			{	
							yylloc.last_line = yylloc.first_line = yylineno; 
							yylval.sVal = new std::string(yytext);
							return BOOL_VAL;
						}
						
".*"					{	yylloc.last_line = yylloc.first_line = yylineno; return ELEM_MULT; }

"'"						{	yylloc.last_line = yylloc.first_line = yylineno; return TRANSPON;	}

"0"|([1-9][0-9]*)		{
							yylval.iVal = atoi(yytext);
							yylloc.last_line = yylloc.first_line = yylineno;
							return NUMBER;
						}
						
[a-zA-Z][_a-zA-Z0-9]*	{
							yylloc.last_line = yylloc.first_line = yylineno;
							yylval.sVal = yytext;
							return NAME;
						}
						
[()!<>+-*=:{}\[\]]		{	yylloc.last_line = yylloc.first_line = yylineno; return *yytext;	}

.						{
							yylloc.last_line = yylloc.first_line = yylineno;
							yyerror("Unknown character");
						}

%%

int yywrap() {
	return 1;
}



