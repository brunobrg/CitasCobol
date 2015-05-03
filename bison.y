%{
#include "comandos.h"

extern FILE * yyin;
extern int contLinhasC;

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

Etapas 
	: Main
	;

Main
    : TIPO MAIN Argumentos 
      {initProcDivision($2);}  
      Bloco
      {fechaMain(); adicionaSimbolos(); saiEscopo();}
    ;

Argumentos
    : LEFT_PAR RIGHT_PAR
    ;

Bloco
    : ABRE_CHAVE FECHA_CHAVE
    | ABRE_CHAVE Comandos FECHA_CHAVE
    ;

Comandos
    :
	| Linha_Comando Comandos
	;

Linha_Comando
    : Comando PVIRGULA
	;

Comando:
	Printf 
	| Declaracao
	| RETURN NUMBER;
	;

Declaracao:
	TIPO PALAVRA {adicionaSimbolo(escopoAtual, "declarada", $1, $2);}
	;

Printf
    : PRINTF LEFT_PAR TEXTO RIGHT_PAR 
      { imprimir($3); }
	;


%%

void main(int argc, char *argv[]){
	init(argc, argv);
}

yyerror(char *s){
	printf("Erro encontrado na linha %d.\n", contLinhasC);
  printf("%s.\n",s);
}

int yywrap(void)
{
	return 1; 
}
