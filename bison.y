%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analise.h"

extern FILE * yyin;
extern int contadorDeLinhas;

%}

%union {
   char * strval;
   int    intval;
}

%token <strval> NUMBER TIPO
%token <strval> STRING STR
%token AND OU IF DO THEN WHILE ELSE NOT PH
%token IGUAL SOMA SUBT MULT DIVIDE ATRIBUI MOD 


//novos
%token INCLUDE   
%token PVIRGULA LEFT_PAR RIGHT_PAR VIRGULA ASPAS ENDERECO
%token ABRE_CHAVE FECHA_CHAVE RETURN END 
%token <strval> PRINTF SCANF VARUSE PALAVRA TEXTO MAIN
%token  error

%start Etapas

%%

Etapas: 
	Main
	;


Main:
	TIPO MAIN {	initProcDivision(); criaEscopo($2); } LEFT_PAR RIGHT_PAR Bloco {inserirSaidaCobol("     STOP RUN.\n\n");}
	;

Bloco:
	| ABRE_CHAVE  Comandos FECHA_CHAVE
	;

Comandos:
	| Linha_Comando Comandos
	;

Linha_Comando:
	Comando PVIRGULA {pulaLinha();}
	;

Comando:
	Printf 
	| RETURN NUMBER;
	;


Printf:
	PRINTF {inserirSaidaCobol("     DISPLAY ");} LEFT_PAR TEXTO  { inserirSaidaCobol($4); inserirSaidaCobol(".");} RIGHT_PAR
	;
%%

void main(int argc, char *argv[]){
	init(argc, argv);
}

yyerror(char *s){
	printf("Erro encontrado na linha %d.\n", contadorDeLinhas);
}

int yywrap(void)
{
	return 1; 
}
