%{
#include "parser.tab.h"   // Tokens generados por Bison
#include "ast.h"          // Para el AST
void yyerror(const char *s);
%}

%option noyywrap

%%

[ \t\n]                 ;  // Ignorar espacios

"int"                   { return T_INT; }
"float"                 { return T_FLOAT; }
"print"                 { return T_PRINT; }
"if"                    { return T_IF; }
"else"                  { return T_ELSE; }
"while"                 { return T_WHILE; }
"for"                   { return T_FOR; }
"in"                    { return T_IN; }

"=="                    { return T_EQ; }
"!="                    { return T_NEQ; }
"<="                    { return T_LEQ; }
">="                    { return T_GEQ; }
".."                    { return T_RANGE; }

[0-9]+                  { yylval.num = atoi(yytext); return T_NUM; }
[0-9]+\.[0-9]+          { yylval.fnum = atof(yytext); return T_FNUM; }
\"[^\"]*\"              { yylval.str = strdup(yytext); return T_STRING; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return T_ID; }

.                       { yyerror("Invalid character"); }

%%