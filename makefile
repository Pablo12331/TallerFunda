CC = gcc
CFLAGS = -g -Wall

all: compiler

compiler: lex.yy.c parser.tab.c ast.c codegen.c main.c
	$(CC) $(CFLAGS) -o $@ $^

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h compiler *.o

.PHONY: all cleans