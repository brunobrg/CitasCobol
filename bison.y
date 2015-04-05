%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
%token TERMINOU DECLARACAO FIMFUNC FUNCAO ESPERA EXECUTE PASSA
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
	TIPO MAIN LEFT_PAR RIGHT_PAR Bloco
	;

Bloco:
	| ABRE_CHAVE Bloco
	| Comandos Bloco
	| FECHA_CHAVE
	;

Comandos:
	Printf Comandos
	| PVIRGULA
	;

Printf:
	PRINTF LEFT_PAR TEXTO RIGHT_PAR
	;

%%

void main(void){
	char arquivo[20];
	printf("informe o arquivo: ");
	scanf("%s", arquivo);

	FILE * myfile = fopen(arquivo, "r");
	yyin = myfile;

	yyparse();

	printf("Analise terminada\n");
}

yyerror(char *s){
	printf("Erro encontrado");
}

int yywrap(void)
{
	return 1; 
}
