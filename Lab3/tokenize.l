%{
	#include "Parser.hpp"
	
	extern void yyerror(const std::string&);
%}

%option yylineno noyywrap outfile="Scanner.cpp"

space [ \t\r]
end_line \n
number 0|[1-9][0-9]*
next_str (\.){3}{space}*{end_line}
oper_and and|\&\&
name [a-zA-Z][_a-zA-Z0-9]*
skip [()!<>+-=:\*{}\[\],]

%%

({space})|({next_str}) {	yylloc.last_line = yylloc.first_line = yylineno;		}

{end_line}({space}|{end_line})* {	yylloc.last_line = yylloc.first_line = yylineno; return ENDL;	}

\<\< {	yylloc.last_line = yylloc.first_line = yylineno; return SL;		}
\>\> {	yylloc.last_line = yylloc.first_line = yylineno; return SR;		}
{oper_and} {	yylloc.last_line = yylloc.first_line = yylineno; return AND;	}
for {	yylloc.last_line = yylloc.first_line = yylineno; return FOR;	}
if {	yylloc.last_line = yylloc.first_line = yylineno; return IF;		}
begin {	yylloc.last_line = yylloc.first_line = yylineno; return BEG;	}
beginif {	yylloc.last_line = yylloc.first_line = yylineno; return BEGIN_IF;	}
beginfor {	yylloc.last_line = yylloc.first_line = yylineno; return BEGIN_FOR;	}
end {	yylloc.last_line = yylloc.first_line = yylineno; return END;	}
endif {	yylloc.last_line = yylloc.first_line = yylineno; return END_IF;	}
endfor {	yylloc.last_line = yylloc.first_line = yylineno; return END_FOR;	}

(int)|(bool) {
				yylloc.last_line = yylloc.first_line = yylineno;
				yylval.sVal = new std::string(yytext);
				return TYPE;
			 }
							
\[c\] {	yylloc.last_line = yylloc.first_line = yylineno; return CONST;	}
\[v\] {	yylloc.last_line = yylloc.first_line = yylineno; return VECT;	}
\[m\] {	yylloc.last_line = yylloc.first_line = yylineno; return MATRIX;	}
"<-" {	yylloc.last_line = yylloc.first_line = yylineno; return ASSIGN;	}
sum {	yylloc.last_line = yylloc.first_line = yylineno; return SUM;	}
map {	yylloc.last_line = yylloc.first_line = yylineno; return MAP_FUNC; }
move {	yylloc.last_line = yylloc.first_line = yylineno; return MOVE;	}
left {	yylloc.last_line = yylloc.first_line = yylineno; return LEFT;	}
right {	yylloc.last_line = yylloc.first_line = yylineno; return RIGHT;	}
exit {	yylloc.last_line = yylloc.first_line = yylineno; return EXIT_FUNC;	}
wall {	yylloc.last_line = yylloc.first_line = yylineno; return WALL_FUNC;	}
size {	yylloc.last_line = yylloc.first_line = yylineno; return SIZE;	}
push {	yylloc.last_line = yylloc.first_line = yylineno; return PUSH;	}
print {	yylloc.last_line = yylloc.first_line = yylineno; return PRINT;	}
function {	yylloc.last_line = yylloc.first_line = yylineno; return WORD_FUNC;	}

(true)|(false) {	
					yylloc.last_line = yylloc.first_line = yylineno;
					std::string str(yytext); 
					if(str == "true")
						yylval.bVal = true;
					else yylval.bVal = false;
					return BOOL_VAL;
				}
						
".*" {	yylloc.last_line = yylloc.first_line = yylineno; return ELEM_MULT; }

"\'" {	yylloc.last_line = yylloc.first_line = yylineno; return TRANSPON;	}

{number} {
			yylval.iVal = atoi(yytext);
			yylloc.last_line = yylloc.first_line = yylineno;
			return NUMBER;
		}
						
{name} {
		yylloc.last_line = yylloc.first_line = yylineno;
		yylval.sVal = new std::string(yytext);
		return NAME;
  	   }
						
{skip} {	yylloc.last_line = yylloc.first_line = yylineno; return *yytext;	}

. {
	yylloc.last_line = yylloc.first_line = yylineno;
	yyerror("Unknown character");
  }

%%
