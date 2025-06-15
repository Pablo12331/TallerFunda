%{
#include <string.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "Ast.h" 
    
%}

%option noyywrap 
%option yylineno

%%

"//"[^\n]* {}


"if"       { return IF; }
"unless"   { return UNLESS; }
"else"     { return ELSE; }
"while"    { return WHILE; }
"for"      { return FOR; }
"return"   { return RETURN; }
"int"      { return INT; }
"float"    { return FLOAT; }
"string"   { return STRING_TYPE; }
"void"     { return VOID; }
"print"    { return PRINT; }
"read"     { return READ; }
"bool"     { return BOOL_TYPE; }
"true"     { yylval.ival = 1; return TRUE_LITERAL; } 
"false"    { yylval.ival = 0; return FALSE_LITERAL; }
"break"    { return BREAK; }
"continue" { return CONTINUE; }

"{"        { return LBRACE; }
"}"        { return RBRACE; }
"("        { return LPAREN; }
")"        { return RPAREN; }
";"        { return SEMICOLON; }
"|>"       { return PIPE; }
"="        { return ASSIGN; }
","        { return ','; }

"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return MULT; }
"/"        { return DIV; }

"+="       { return PLUS_ASSIGN; }
"-="       { return MINUS_ASSIGN; }
"*="       { return MULT_ASSIGN; }
"/="       { return DIV_ASSIGN; }

"=="       { return EQ; }
"!="       { return NE; }
"<"        { return LT; }
">"        { return GT; }
"<="       { return LE; }
">="       { return GE; }
"&&"       { return AND_OP; }
"||"       { return OR_OP; }
"!"        { return NOT_OP; }

[0-9]+\.[0-9]+([eE][-+]?[0-9]+)?  { 
    yylval.fval = atof(yytext);
    return FLOAT_NUMBER;
}

[0-9]+     {
    yylval.ival = atoi(yytext);
    return NUMBER;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.sval = strdup(yytext);
    if (!yylval.sval) {perror("strdup failed"); exit(1);}
    return IDENTIFIER;
}

\"(\\.|[^\\"])*\" {
    int len = yyleng - 2;
    char* processed_sval = (char*)malloc(len + 1);
    if (!processed_sval) { perror("malloc failed"); exit(1); }
    char* write_ptr = processed_sval;
    char* read_ptr = yytext + 1;
    while (read_ptr < yytext + yyleng - 1) {
        if (*read_ptr == '\\') {
            read_ptr++;
            switch (*read_ptr) {
                case 'n': *write_ptr++ = '\n'; break;
                case 't': *write_ptr++ = '\t'; break;
                case '\\': *write_ptr++ = '\\'; break;
                case '"': *write_ptr++ = '"'; break;
                default: *write_ptr++ = *read_ptr;
            }
        } else {
            *write_ptr++ = *read_ptr;
        }
        read_ptr++;
    }
    *write_ptr = '\0';

    yylval.sval = processed_sval;
    return STRING_LITERAL;
}

[ \t\r\n]+ { }

.          {
    fprintf(stderr, "Error Léxico: Caracter inválido '%s' en la línea %d\n", yytext, yylineno);
}

%%