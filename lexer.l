%{
#include "parser.tab.h"
#include "ast.h"
void yyerror(const char *s);
%}

%option noyywrap

%%

[ \t\n]                 ;  // Ignore whitespace

"int"                   { return T_INT; }
"float"                 { return T_FLOAT; }
"string"                { return T_STRING; }
"if"                    { return T_IF; }
"else"                  { return T_ELSE; }
"while"                 { return T_WHILE; }
"for"                   { return T_FOR; }
"in"                    { return T_IN; }       // Used for range operator
"print"                 { return T_PRINT; }    
"read"                  { return T_READ; }     

"=="                    { return T_EQ; }
"!="                    { return T_NEQ; }
"<="                    { return T_LEQ; }
">="                    { return T_GEQ; }
".."                    { return T_RANGE; }    // Range operator (e.g., 1..10)

[0-9]+                  { yylval.num = atoi(yytext); return T_NUM; }
[0-9]+\.[0-9]+          { yylval.fnum = atof(yytext); return T_FNUM; }
\"[^\"]*\"              { yylval.str = strdup(yytext); return T_STRING_LIT; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return T_ID; }

.                       { yyerror("Unrecognized character"); }

%%