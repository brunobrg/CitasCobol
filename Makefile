all: removeold compilabison compilaflex compilac clean

removeold:
	rm -rf programa
compilabison: bison.y
	bison -d -o bison.c bison.y
compilaflex: lexico.l
	flex lexico.l
compilac: bison.c lex.yy.c
	gcc bison.c lex.yy.c -o ./programa
clean:
	rm -rf bison.c lex.yy.c y.tab.c bison.h bison.tab.c
