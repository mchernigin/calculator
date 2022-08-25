CC = cc
CFLAGS = -Wall -Wextra -ggdb3
LEX = flex
YACC = bison

.PHONY: all benchmark clean

all: main
	mv main calc

lexer.h lexer.c: lexer.l
	$(LEX) --header-file=lexer.h -o lexer.c $<

parser.h parser.c: parser.y
	$(YACC) -Wall -o parser.c -d $<

main: main.o ast.o basic_calc.o ast_calc.o lexer.o
main.c: lexer.h basic_calc.h ast_calc.h
basic_calc.c ast_calc.c: parser.h

benchmark: all
	python benchmark.py

clean:
	$(RM) *.o lexer.[ch] parser.[ch] main calc graph.png
