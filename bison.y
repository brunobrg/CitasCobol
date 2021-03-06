%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturaC.h"
#include "estruturaCobol.h"
#include "traducao.h"

/* Variaveis globais */
extern FILE     * yyin;              /* Arquivo do parser */
FILE            * arq_entrada;       /* Arquivo de entrada */
FILE            * arq_saida;         /* Arquivo de saida */
int               contLinhasC = 1;   /* Contador de linha arq de entrada */ 
int               contColunaC = 1;   /* Contador de coluna arq de entrada */ 
char            * nomePrograma;      /* Nome do programa */
SaidaCobol      * saidaCobol = NULL; /* Arvore de blocosCobol */
extern SaidaErro * saidaErro;   /* estrutura para os erros */
int               p;                 /* Passo de compilacao */
int               qntErros=0;        /* Quantidade de erros */
extern Escopo   * escopoAtual;


%}

%union {
   char * strval;
   int    intval;
}

%token <strval> COMENT
%token <strval> WORD FILENAME NUMBER QUOTE
%token LE GE EQUAL NEQUAL
%token AND OR
%token <strval> TYPE
%token IF ELSE WHILE DO RETURN
%token <strval> PRINTF VARUSE 
%token <strval> INCLUDE DEFINE

%type <strval> Expressao Operacao

%start Global

%%

Global
    :
    | Comando_global Global
    ;

Comando_global
    : INCLUDE 
      { if(p==0) inclui($1); }
    | DEFINE
    | DEFINE NUMBER
    | Declaracao ';'
    | Definicao
    | Comentario 
    ;

Declaracao
    : Dec_variavel 
    | Dec_funcao  
    | Dec_struct 
    ;

Definicao
    : Def_funcao
    ;

Comentario
    : COMENT { if(p==2) comentario(&saidaCobol,$1); }
    ;

Dec_variavel
    : TYPE WORD '=' NUMBER
      { 
        if(p==1)
        {
          adicionaSimbolo(escopoAtual,"declarada", contLinhasC, $1, $2, $4);
          adicionaSimbolo(escopoAtual, "usada", contLinhasC, $1, $2, $4); 
        }
      }
    | TYPE WORD '=' WORD
      { 
        if(p==1)
        {
        adicionaSimbolo(escopoAtual,"declarada", contLinhasC,$1, $2, $4); 
        adicionaSimbolo(escopoAtual, "usada", contLinhasC, $1, $2, $4);
        }
      }
    | TYPE WORD '=' QUOTE
      { 
        if(p==1) 
        {
        adicionaSimbolo(escopoAtual,"declarada", contLinhasC,$1, $2, $4); 
        adicionaSimbolo(escopoAtual, "usada", contLinhasC, $1, $2, $4);
        }
      }
    | TYPE WORD
      {  
        if(p==1) 
        {
          adicionaSimbolo(escopoAtual,"declarada", contLinhasC,$1, $2, NULL); 
        }
        }
    ;

Dec_funcao
    : TYPE WORD '(' Dec_argumentos ')'
    | TYPE WORD '(' ')'
    ;

Def_funcao
    : TYPE WORD '(' Def_argumentos ')'
      { if (p == 1) criaEscopo($2);
        if (p == 2) abreSection(&saidaCobol,$2); 
      }  
     '{' Bloco '}'
      { if (p == 1) saiEscopo();
        if (p == 2) fechaSection(&saidaCobol,$2);
      }
    | TYPE WORD '(' ')'
      {  if (p == 1) criaEscopo($2);
         if (p == 2) abreSection(&saidaCobol,$2);
      }    
     '{' Bloco '}'
      { if (p == 1) saiEscopo();
        if (p == 2) fechaSection(&saidaCobol,$2); 
      }
    ; 

Dec_argumentos
    : TYPE
    | Dec_argumentos ',' TYPE
    ;

Def_argumentos
    : TYPE WORD
    | Def_argumentos ',' TYPE WORD 
    ;

Dec_struct
    : "struct" WORD '{' '}' 
    ;

Bloco
    :  Comandos RETURN NUMBER ';'
    |  Comandos
    ;

Comandos
    :
    | Comando Comandos
    ;

Comando
    : Dec_variavel ';'
    | Atribuicao ';'
    | Printf ';'
    | Comentario
    ;

Atribuicao
	: Atribuicao_Simples
	| Atribuicao_Complexa
	;

Atribuicao_Simples
    : WORD '=' WORD
      { if (p==2)
        {
          /**** encapsular em uma funcao ****/
          Linha * linha = criarLinhaB();
          inserirToken(&linha, "MOVE");
          inserirToken(&linha, $3);
          inserirToken(&linha, "TO");
          inserirToken(&linha, $1);
          inserirProcDiv(&saidaCobol, linha);
          adicionaSimbolo(escopoAtual, "usada", contLinhasC, "null", $1, $3);
          /**** ****/
        }
      }
    | WORD '=' NUMBER
      { if (p==2)
        {
          /**** encapsular em uma funcao ****/
          Linha * linha = criarLinhaB();
          inserirToken(&linha, "MOVE");
          inserirToken(&linha, $3);
          inserirToken(&linha, "TO");
          inserirToken(&linha, $1);
          inserirProcDiv(&saidaCobol, linha);
          adicionaSimbolo(escopoAtual, "usada", contLinhasC, "null", $1, $3);
          /**** ****/
        }
      }
    | WORD '=' QUOTE
      { if (p==2)
        {
          /**** encapsular em uma funcao ****/
          Linha * linha = criarLinhaB();
          inserirToken(&linha, "MOVE");
          inserirToken(&linha, $3);
          inserirToken(&linha, "TO");
          inserirToken(&linha, $1);
          inserirProcDiv(&saidaCobol, linha);
          adicionaSimbolo(escopoAtual, "usada", contLinhasC, "null", $1, $3);
          /**** ****/
        }
      }
    ;

Atribuicao_Complexa
	: WORD '=' Expressao
      { if (p==2)
        {
          /**** encapsular em uma funcao ****/
          Linha * linha = criarLinhaB();
          inserirToken(&linha, "COMPUTE");
          inserirToken(&linha, $1);
          inserirToken(&linha, "=");
          inserirToken(&linha, $3);
          inserirProcDiv(&saidaCobol, linha);
          adicionaSimbolo(escopoAtual, "usada", contLinhasC, "null", $1, $3);
          /**** ****/
        }
      }

Printf
    : PRINTF '(' QUOTE ')'
      { 
        if(p==2) imprimir(&saidaCobol,$3); 
      }
    ;

Expressao 
	: NUMBER Operacao NUMBER
	  { if (p==2)
        {
          char * expressao = (char *) malloc(sizeof(char)) ;
          strcpy(expressao,$1);
          strcat(expressao,$2);
          strcat(expressao,$3);
          $$ = expressao;
        }
      }
    ;

Operacao
    : '+' {$$ = " + ";}
    | '-' {$$ = " - ";}
    | '*' {$$ = " * ";}
    | '/' {$$ = " / ";}
    | '^' {$$ = " ** ";}
    ;


%%

void main(int argc, char *argv[]){
	
	/*--- Abre input: arq_entrada ---*/

	if(argc < 2)
		erro(0);

  arq_entrada = fopen(argv[1], "r");
  nomePrograma = nomeProgramaCob(argv[1]);
  arq_saida = fopen(nomePrograma, "w+");

	/* --- --- --- --- --- --- --- ---*/
  
	if(arq_entrada)
	{
      for(p=0;p<=2;p++)
      {
        switch ( p )
        {
          case 0:   /* p=0: Pré-processamento */
            printf("* Pré-processamento...\n");
            yyin = arq_entrada; 
            yyparse();  
            criarDivisions(&saidaCobol);
            escreverIdntDivision(&saidaCobol,nomePrograma);
            break;
          case 1:   /* p=1: Escopo e Tabela de Variaveis*/
            printf("* Montando tabela de variáveis...\n"); 
            rewind(yyin);     
            contLinhasC = 1; 
            initEscopo();    
            yyparse();     
            terminaEscopo();
            break;
          case 2:   /* p=2: Traducao */
            printf("* Lendo o programa...\n");
            rewind(yyin);
            contLinhasC = 1; 
            yyparse();
            fclose(arq_entrada);

            if(qntErros == 0 && verificaListaEscopo())
            {
              escreverDataDivision(&saidaCobol);
              organizarSaida(&saidaCobol);
		          escreverArquivo(arq_saida,saidaCobol);
              printf("***** Tradução completa: %s.\n", nomePrograma);
              fclose(arq_saida);
            }
            else
            {
              SaidaErro * aux = saidaErro;
              while(aux!=NULL)
              {
                fprintf(arq_saida, "%s\n", aux->mensagem);
                aux = aux->proximo;
              }
              printf("* Log de erros salvo em %s.\n", nomePrograma);
              fclose(arq_saida);
            }

            free(saidaCobol);
            break;

        }
      }
	}
	else /* Arquivo nao encontrado. */
	{
	    erro(1);
	}
}

yyerror(char * msg)
{
  qntErros++;
  printf("*** ERRO ");
  printf("(linha %d):", contLinhasC);
  printf(" %s\n", msg);
}

erro(int error_code)
{
  qntErros++;
  printf("*** ERRO %i", error_code);
  switch ( error_code )
  {
    case 0 :
      printf(": Nome do arquivo não informado.\n");
	    exit(1);
    case 1 :
      printf(": Arquivo não encontrado.\n");
      exit(1);   
    case 2:
      printf(": Erro no arquivo de saída.\n");
      fprintf(arq_saida, "Erro no arquivo, por favor arrumar os erros.\n");
      exit(1);
    case 3:
      printf(" (linha %d): ", contLinhasC);
      printf("Printf() vazio.\n");
      fprintf(arq_saida,"*** ERRO %i  (linha %d): ", error_code, contLinhasC);
      fprintf(arq_saida,"Printf() vazio.\n");
      break;
    case 4:
      printf(" (linha %d): ", contLinhasC);
      printf("Identificador muito grande.\n");
      fprintf(arq_saida,"*** ERRO %i  (linha %d): ", error_code, contLinhasC);
      fprintf(arq_saida,"Identificador muito grande.\n");
      break;
    case 5:
      printf(" (linha %d): ", contLinhasC);
      printf("Tipo não suportado.\n");
      fprintf(arq_saida,"*** ERRO %i  (linha %d): ", error_code, contLinhasC);
      fprintf(arq_saida,"Tipo não suportado.\n");
      break;
    case 6:
      printf(" (linha %d): ", contLinhasC);
      printf("Include stdio.h nao encontrado.\n");
      fprintf(arq_saida,"*** ERRO %i  (linha %d): ", error_code, contLinhasC);
      fprintf(arq_saida,"Include stdio.h nao encontrado.\n");
      break;
    default : 
      printf(" (linha %d).\n", contLinhasC);
      fprintf(arq_saida,"*** ERRO %i  (linha %d): ", error_code, contLinhasC);
      break;
  }
}

warning(char * msg, int l)
{
  char buffer[256];
  sprintf(buffer,"*** WARNING: linha %d.\n***          %s\n", l, msg);
  inserirSaidaErros(buffer);
  printf("*** WARNING: linha %d.\n***          %s\n", l, msg);
}
