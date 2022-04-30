CC = cc
CFLAGS = -Wall -Wextra -g
LEX = flex
YACC = bison

%.lex.h %.lex.c: %.l
	$(LEX) --header-file=$*.lex.h -o $*.lex.c $<

%.tab.h %.tab.c: %.y
	$(YACC) -d $<

main: main.o parser.lex.o ast.lex.o parser.tab.o ast.tab.o parser.o ast.o

main.c: parser.lex.h parser.tab.h ast.lex.h ast.tab.h

.PHONY: clean
clean:
	rm -rf *.tab.[cho] *.lex.[cho] ast.o main.o main