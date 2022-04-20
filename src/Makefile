CC = cc
CFLAGS = -Wall -Wextra
LEX = flex
YACC = bison

%.tab.h %.tab.c : %.y
	$(YACC) -d $<

%.lex.h %.lex.c : %.l
	$(LEX) --header-file=$*.lex.h -o $*.lex.c $<

main : main.o calc.tab.o calc.lex.o

main.c : calc.lex.h calc.tab.h

.PHONY : clean
clean :
	rm -rf *.tab.[ch] *.lex.[ch] main.o main
