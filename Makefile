CC = cc
CFLAGS = -Wall -Wextra -g
LEX = flex
YACC = bison

%.tab.h %.tab.c : %.y
	$(YACC) -d $<

%.lex.h %.lex.c : %.l
	$(LEX) --header-file=$*.lex.h -o $*.lex.c $<

main : main.o calc.lex.o ast.o calc.tab.o

main.c : calc.lex.h calc.tab.h

.PHONY : clean
clean :
	rm -rf *.tab.[cho] *.lex.[cho] ast.o main.o main
