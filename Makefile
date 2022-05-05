CC = cc
CFLAGS = -Wall -Wextra
LEX = flex
YACC = bison

%.c: %.y
%.c: %.l

%lexer.h %lexer.c: %lexer.l
	$(LEX) --header-file=$*lexer.h -o $*lexer.c $<

%parser.h %parser.c: %parser.y
	$(YACC) -Wall -o $*parser.c -d $<

main: main.o basic_lexer.o ast_lexer.o basic_parser.o ast_parser.o basic_calc.o ast_calc.o 
main.c: basic_lexer.h basic_parser.h ast_lexer.h ast_parser.h

.PHONY: clean
clean:
	$(RM) *.o *lexer.[ch] *parser.[ch] main
