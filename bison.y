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
%token PVIRGULA LEFT_PAR RIGHT_PAR VIRGULA ASPAS
%token ABRE_CHAVE FECHA_CHAVE RETURN
%token <strval> PRINTF SCANF VARUSE
%token MAIN error

%type <strval> Quote

%start Etapas

%%

Etapas:
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
	| PVIRGULA {pulaLinha();} Comandos
	| PVIRGULA
	;

Quote:
	ASPAS STRING ASPAS {$$=$2;}
	;

Printf:
	PRINTF {inserirSaidaCobol("     DISPLAY ");} LEFT_PAR Quote  { inserirSaidaCobol($4); inserirSaidaCobol(".");} RIGHT_PAR
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
