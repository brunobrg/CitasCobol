%{

#include <stdio.h>
#include <stdlib.h>
#include "estruturaC.h"
#include "estruturaCobol.h"
#include "traducao.h"

/* Variaveis globais */
extern FILE     * yyin;            /* Arquivo do parser */
FILE            * arq_entrada;     /* Arquivo de entrada */
FILE            * arq_saida;       /* Arquivo de saida */
int               contLinhasC = 1; /* Contador do arquivo de entrada */
ListaDeEscopo   * listaDeEscopo;
Simbolos        * listaDeVariaveis;
Escopo          * escopo = NULL;
Escopo          * escopoAtual;
int               idEscopo = 0;      
char            * nomePrograma;     /* Nome do programa */
Linha           * printbuff = NULL; /* Buffer de string a imprimir */
SaidaCobol      * saidaCobol;       /* Arvore de blocosCobol */
SaidaCobol      * saidaVariaveis;   
int               p;                /* Passo de compilacao */
int               qntErros=0;       /* Quantidade de erros */

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
      { if(p==2) escreverProcDivision(&saidaCobol); }  
      Bloco
      { if(p==2) fechaMain(&saidaCobol,printbuff); }
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
      { if(p==2) imprimir(&saidaCobol,$3,&printbuff); }
	;

%%

void main(int argc, char *argv[]){
	
	/*--- Abre input: arq_entrada ---*/

	if(argc < 2)
		yyerror(0);

  arq_entrada = fopen(argv[1], "r");
  nomePrograma = nomeProgramaCob(argv[1]);

	/* --- --- --- --- --- --- --- ---*/
  
	if(arq_entrada)
	{
      for(p=0;p<=2;p++)
      {
        switch ( p )
        {
          case 0: /* p=0: Pré-processamento */
            printf("* Pré-processamento...\n");
            //yyin = arq_entrada;
            //yyparse();
            criarDivisions(&saidaCobol);
            escreverIdntDivision(&saidaCobol,nomePrograma);
            break;
          case 1: /* p=1: Escopo e Tabela de Variaveis*/
            printf("* Montando tabela de variáveis...\n"); 
            //yyin = arq_entrada;
            //yyparse();
            //initEscopo();
            //escreverDataDivision();
            break;
          case 2: /* p=2: Traducao */
            printf("* Lendo o programa...\n");
            yyin = arq_entrada;
            yyparse();
            fclose(arq_entrada);

            if(verificaLista(listaDeEscopo)) {}
            else yyerror(2);

            if(qntErros == 0)
            {
              arq_saida = fopen(nomePrograma, "w+");
              organizarSaida(&saidaCobol);
		          escreverArquivo(arq_saida,saidaCobol);
              printf("*** Tradução completa.\n");
              fclose(arq_saida);
            }

            free(saidaCobol);
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

  qntErros++;
  printf("*** ERRO %i: ", error_code);
  switch ( error_code )
  {
    case 0 :
      printf("Nome do arquivo não informado.\n");
	    exit(1);
    case 1 :
      printf("Arquivo não encontrado.\n");
      exit(1);   
    case 2:
      printf("Erro no arquivo de saída.\n");
      fprintf(arq_saida, "Erro no arquivo, por favor arrumar os erros.\n");
      exit(1);
    case 3:
      printf("linha %d.\n", contLinhasC);
      printf("***         Printf() vazio.\n");
      break;
    default : 
      printf("Erro encontrado na linha %d.\n", contLinhasC);
      break;
  }
}


