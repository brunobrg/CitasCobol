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
int               contLinhasC = 1;   /* Contador do arq de entrada */   
char            * nomePrograma;      /* Nome do programa */
SaidaCobol      * saidaCobol = NULL; /* Arvore de blocosCobol */
int               p;                 /* Passo de compilacao */
int               qntErros=0;        /* Quantidade de erros */
extern Escopo   * escopoAtual;


%}

%union {
   char * strval;
   int    intval;
}

%token <strval> COMENT
%token <strval> WORD NUMBER QUOTE
%token LE GE EQUAL NEQUAL
%token AND OR
%token <strval> TYPE
%token IF ELSE WHILE DO RETURN
%token <strval> PRINTF VARUSE 
%token INCLUDE PH

%type <strval> Expressao Operacao

%start Global

%%

Global
    :
    | Comando_global Global
    ;

Comando_global
    : Include
    | Define
    | Declaracao ';'
    | Definicao
    | Comentario 
    ;

Include
    : INCLUDE '<' Include_Sintaxe PH '>'
    | INCLUDE '\"' Include_Sintaxe PH '\"'
    ;

Include_Sintaxe
	: WORD { printf("%s", $1); if (hasBlankSpace($1)) erro(6); }
	;

Define
    : "#define" WORD NUMBER
    | "#define" WORD
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
      { if(p==1) adicionaSimbolo(escopoAtual,"declarada", $1, $2, $4); }
    | TYPE WORD '=' QUOTE
      { if(p==1) adicionaSimbolo(escopoAtual,"declarada", $1, $2, $4); }
    | TYPE WORD
      { if(p==1) adicionaSimbolo(escopoAtual,"declarada", $1, $2, NULL); }
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
          /**** ****/
        }
      }
    | WORD '=' Expressao
      { if (p==2)
        {
          /**** encapsular em uma funcao ****/
          Linha * linha = criarLinhaB();
          inserirToken(&linha, "COMPUTE");
          inserirToken(&linha, $1);
          inserirToken(&linha, "=");
          inserirToken(&linha, $3);
          inserirProcDiv(&saidaCobol, linha);
          /**** ****/
        }
      }
    ;

Printf
    : PRINTF '(' QUOTE ')'
      { if(p==2) imprimir(&saidaCobol,$3); }
    ;

Expressao
    : NUMBER Operacao NUMBER
      { if (p==2)
        {
          /**** encapsular em uma funcao ****/
          char * tok1 = (char *) malloc(sizeof(char)) ;
          char * tok2 = (char *) malloc(sizeof(char)) ;
          char * tok3 = (char *) malloc(sizeof(char)) ;
          char * expressao = (char *) malloc(sizeof(char)) ;
          strcpy(expressao,tok1);
          strcat(expressao,tok2);
          strcat(expressao,tok3);
          $$ = expressao;
          /**** ****/
        }
      }
    ;

Operacao
    : '+' {$$ = "+";}
    | '-' {$$ = "-";}
    | '*' {$$ = "*";}
    | '/' {$$ = "/";}
    ;


%%

void main(int argc, char *argv[]){
	
	/*--- Abre input: arq_entrada ---*/

	if(argc < 2)
		erro(0);

  arq_entrada = fopen(argv[1], "r");
  nomePrograma = nomeProgramaCob(argv[1]);

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
            escreverDataDivision(&saidaCobol);
            break;
          case 2:   /* p=2: Traducao */
            printf("* Lendo o programa...\n");
            rewind(yyin);
            contLinhasC = 1; 
            yyparse();
            fclose(arq_entrada);

            //if(verificaListaEscopo()) {}
            //selse erro(2);

            if(qntErros == 0)
            {
              arq_saida = fopen(nomePrograma, "w+");
              organizarSaida(&saidaCobol);
		          escreverArquivo(arq_saida,saidaCobol);
              printf("***** Tradução completa.\n");
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
      break;
    case 4:
      printf(" (linha %d): ", contLinhasC);
      printf("Identificador muito grande.\n");
      break;
    case 5:
      printf(" (linha %d): ", contLinhasC);
      printf("Tipo não suportado.\n");
      break;
    case 6:
      printf(" (linha %d): ", contLinhasC);
      printf("Nome do arquivo de include invalido.\n");
      break;
    default : 
      printf(" (linha %d).\n", contLinhasC);
      break;
  }
}

warning(char * msg)
{
  printf("*** WARNING: linha %d.\n", contLinhasC);
  printf("***          %s\n",msg);
}
