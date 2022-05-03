CC = cc
CFLAGS = -Wall -Wextra
LEX = flex
YACC = bison

%.c: %.y
%.c: %.l

%.lex.h %.lex.c: %.l
	$(LEX) --header-file=$*.lex.h -o $*.lex.c $<

%.tab.h %.tab.c: %.y
	$(YACC) -Wall -d $<

main: main.o basic_lexer.lex.o ast_lexer.lex.o basic_parser.tab.o ast_parser.tab.o basic_calc.o ast_calc.o 

main.c: basic_lexer.lex.h basic_parser.tab.h ast_lexer.lex.h ast_parser.tab.h

.PHONY: clean
clean:
	rm -rf *.tab.[cho] *.lex.[cho] ast_calc.o basic_calc.o main.o main
