all: removeold compilabison compilaflex compilac clean
	
removeold:
	rm -rf ccobol
compilabison: bison.y
	bison -d -o bison.c bison.y
compilaflex: lexico.l
	flex lexico.l
compilac: bison.c lex.yy.c
	gcc  traducao.c escopo.c comandos.c bison.c lex.yy.c -o ./ccobol
clean:
	rm -rf bison.c lex.yy.c y.tab.c bison.h bison.tab.c
