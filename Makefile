CC=cc
LEX=flex
YACC=bison

all: calc

calc: calc.y calc.l
	$(YACC) -d calc.y
	$(LEX) calc.l
	$(CC) -o $@ calc.tab.c lex.yy.c -ll

clean:
	rm -f calc *.tab.? *.yy.?
