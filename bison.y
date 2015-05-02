%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traducao.h"


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
      {initProcDivision();criaEscopo($2);}  
      Bloco
      {fechaMain();}
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

Comando
    : Printf 
	| RETURN NUMBER;
	;

Printf
    : PRINTF LEFT_PAR TEXTO RIGHT_PAR 
      { Linha * linha = criarLinhaB();
        inserirToken(&linha, "DISPLAY");
        inserirToken(&linha, $3);
        inserirSaida(linha);
      }
	;
%%

void main(int argc, char *argv[]){
	init(argc, argv);
}

yyerror(char *s){
	printf("Erro encontrado na linha %d.\n", contLinhasC);
}

int yywrap(void)
{
	return 1; 
}
