%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analise.h"

extern FILE * yyin;

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
%token ABRE_CHAVE FECHA_CHAVE RETURN ENDE 
%token <strval> PRINTF SCANF VARUSE PALAVRA TEXTO
%token MAIN error

%start Etapas

%%

Etapas: 
	Main
	;


Main:
	TIPO MAIN {	initProcDivision(); } LEFT_PAR RIGHT_PAR Bloco {inserirSaidaCobol("     STOP RUN.\n\n");}
	;

Bloco:
	| ABRE_CHAVE Bloco
	| Comandos Bloco
	| FECHA_CHAVE
	;

Comandos:
	Printf Comandos
	| RETURN NUMBER Comandos
	| PVIRGULA {pulaLinha();}

Printf:
	PRINTF {inserirSaidaCobol("     DISPLAY ");} LEFT_PAR TEXTO  { inserirSaidaCobol($4); inserirSaidaCobol(".");} RIGHT_PAR
	;
%%

void main(void){
	init();
}

yyerror(char *s){
	printf("Erro encontrado");
}

int yywrap(void)
{
	return 1; 
}
