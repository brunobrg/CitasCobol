%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analise.h"

extern char * yytext;
extern FILE * yyin;

%}

%union {
   char * strval;
   int    intval;
}

%token <strval> T_NUMBER TIPO
%token <strval> T_STRING TEXTO  STR
%token AND OU IF DO THEN WHILE ELSE NOT PH
%token TERMINOU DECLARACAO FUNCAO ESPERA EXECUTE PASSA
%token IGUAL SOMA SUBT MULT DIVIDE ATRIBUI MOD ENDERECO
%token NUMERICAL


//novos
%token INCLUDE   
%token PVIRGULA LEFT_PAR RIGHT_PAR VIRGULA
%token ABRE_CHAVE FECHA_CHAVE RETURN
%token <strval> PRINTF SCANF VARUSE
%token MAIN error

%start Etapas

%%

Etapas:
	TIPO MAIN {	initProcDivision(); } LEFT_PAR RIGHT_PAR Bloco
	;

Bloco:
	| ABRE_CHAVE Bloco
	| Comandos Bloco
	| FECHA_CHAVE
	;

Comandos:
	Printf Comandos
	| RETURN {inserirSaidaCobol("     STOP RUN.");} T_NUMBER Comandos
	| PVIRGULA {pulaLinha();} Comandos
	| PVIRGULA
	;

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
