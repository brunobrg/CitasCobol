%{
#include <stdio.h>
#include <stdlib.h>
#include "estruturaC.h"
#include "estruturaCobol.h"
#include "traducao.h"

/* Variaveis globais */
extern FILE     * yyin;
int               contLinhasC = 1;
ListaDeEscopo   * listaDeEscopo;
Simbolos        * listaDeVariaveis;
Escopo          * escopo = NULL;
Escopo          * escopoAtual;
int               idEscopo = 0;
char            * nomePrograma;
Linha           * printbuff = NULL;
SaidaCobol      * saidaCobol;
SaidaCobol      * saidaVariaveis;
int               passoComp;


%}

%union {
   char * strval;
   int    intval;
}

%token <strval> WORD NUMBER QUOTE
%token LE GE EQUAL NEQUAL
%token AND OR
%token <strval> TYPE
%token IF ELSE WHILE DO RETURN
%token <strval> PRINTF VARUSE MAIN
%token INCLUDE PH
%start Global

%%

Global 
	: Main
	;

Main
    : TYPE MAIN Argumentos 
      {/* initProcDivision($2); */}  
      Bloco
      {/* fechaMain(); */}
    ;

Argumentos
    : '(' ')'
    ;

Bloco
    : '{' Comandos '}'
    ;

Comandos
    :
	| Comando Comandos
	;

Comando
    : Printf ';'
	| Declaracao ';'
	| Atribuicao ';'
	| RETURN NUMBER ';'
	;

Atribuicao
    : WORD '=' NUMBER '+' NUMBER 
	{		
	  /* calcula(expressao);
    **** encapsular:
		Linha * linha = criarLinhaB();
		inserirToken(&linha, "COMPUTE");
		inserirToken(&linha, $1);
		inserirToken(&linha, "=");
		inserirToken(&linha, $3);
		inserirToken(&linha, "+");
		inserirToken(&linha, $5);
		inserirSaida(linha);
    ****
		*/
	}
	;

Declaracao
    : TYPE WORD
    { /* 
      adicionaSimbolo(escopoAtual, "declarada", $1, $2);
      */
    }
	| TYPE WORD '=' NUMBER 
	{ /* 
	  adicionaSimbolo(escopoAtual, "declarada", $1, $2); valorSimbolo(escopoAtual, $1, $2, $4);
	  */
	}
	;

Printf
    : PRINTF '(' QUOTE ')'
      { /*imprimir($3,&printbuff); */}
	;

%%

void main(int argc, char *argv[]){
	
	/*--- Abre input: arq_entrada ---*/

	if(argc < 2)
		yyerror(0);

  FILE * arq_entrada = fopen(argv[1], "r");
  nomePrograma = nomeProgramaCob(argv[1]);
  FILE * arq_saida = fopen(nomePrograma, "w+");

	/* --- --- --- --- --- --- --- ---*/
  
	if(arq_entrada)
	{
      for(passoComp=0;passoComp<=2;passoComp++)
      {
        switch ( passoComp )
        {
          case 0: /* Pré-processamento */
            printf("* Pré-processamento...\n");
            //yyparse();
            yyin = arq_entrada;
            criarDivisions(&saidaCobol);
            escreverIdntDivision(&saidaCobol->conteudo,nomePrograma);
            break;
          case 1: /* Escopo e Tabela de Variaveis*/
            printf("* Montando tabela de variáveis...\n");          
            //yyparse();
            //initEscopo();
            //escreverDataDivision();
            break;
          case 2: /* Traducao */
            printf("* Gerando programa Cobol...\n");
            //yyparse();
            fclose(arq_entrada);

            organizarSaida(&saidaCobol);
		        escreverArquivo(arq_saida,saidaCobol);
            free(saidaCobol);
            fclose(arq_saida);

            /*
   	        if(verificaLista(listaDeEscopo) || 1)
   	        {
	          organizarSaida(&saidaCobol->conteudo);
		        escreverArquivo(saidaCobol->conteudo);
	          }
	          else
	          {
		         escreverErro(nomePrograma);
	          }
	          */
            printf("*** Tradução completa.\n");
            break;
        }
      }
	}
	else /* Arquivo nao encontrado. */
	{
	    yyerror(1);
	}
}

yyerror(int error_code){

  printf("*** ERRO %i: ", error_code);
  switch ( error_code )
  {
    case 0 :
      printf("Nome do arquivo não informado.\n");
	    exit(1);
    case 1 :
      printf("Arquivo não encontrado.\n");
      exit(1);    
    default : 
      printf("Erro encontrado na linha %d.\n", contLinhasC);
      break;
  }


}


