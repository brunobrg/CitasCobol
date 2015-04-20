all: removeold compilabison compilaflex compilac

removeold:
	rm -rf programa
compilabison: bison.y
	bison -d -o bison.c bison.y
compilaflex: lexico.l
	flex lexico.l
compilac: bison.tab.c lex.yy.c
	gcc bison.c lex.yy.c -o ./programa
