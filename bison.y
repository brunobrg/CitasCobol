%{
#include "comandos.h"

extern FILE * yyin;
extern int contLinhasC;

%}

%union {
   char * strval;
   int    intval;
}

%token <strval> NUMBER TIPO SOMA SUBT MULT DIVIDE
%token <strval> STRING STR 
%token AND OU IF DO THEN WHILE ELSE NOT PH
%token IGUAL  ATRIBUI MOD

%type <strval> Atribuicao_Simples_Valor Sinais

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
    : ABRE_CHAVE Comandos FECHA_CHAVE
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
	| Atribuicoes
	| RETURN NUMBER;
	;

Sinais:
	SOMA {$$ = $1;}
	| SUBT {$$ = $1;}
	| MULT {$$ = $1;}
	| DIVIDE {$$ = $1;}
	;

Atribuicoes:
	Atribuicao_Simples
	| Atribuicao_Complex
	;

Atribuicao_Complex:
	PALAVRA ATRIBUI NUMBER Sinais NUMBER 
	{		
		Linha * linha = criarLinhaB();
		inserirToken(&linha, "COMPUTE");
		inserirToken(&linha, $1);
		inserirToken(&linha, "=");
		inserirToken(&linha, $3);
		inserirToken(&linha, $4);
		inserirToken(&linha, $5);
		inserirSaida(linha);
	}
	;

Atribuicao_Simples:
	PALAVRA ATRIBUI Atribuicao_Simples_Valor
	{
		Linha * linha = criarLinhaB();
		inserirToken(&linha, "MOVE");
		inserirToken(&linha, $1);
		inserirToken(&linha, "TO");
		inserirToken(&linha, $3);
		inserirSaida(linha);
	}
	;

Atribuicao_Simples_Valor:
	PALAVRA {$$ = $1;}
	| NUMBER {$$ = $1;}
	;

Declaracao:
	TIPO PALAVRA {adicionaSimbolo(escopoAtual, "declarada", $1, $2);}
	| TIPO PALAVRA ATRIBUI NUMBER { adicionaSimbolo(escopoAtual, "declarada", $1, $2); valorSimbolo(escopoAtual, $1, $2, $4);}
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
