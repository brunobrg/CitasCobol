#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "escopo.h"

/* structs */
typedef struct _TokenList
{
	/* 
    Lista de tokens. Cada token deve ter 30 posicoes,
                     exceto se estiver entre aspas.
	*/
	char              * token;
	struct _TokenList * proximo;
}TokenList;

typedef struct _Linha
{
	/* 
	numeracao: 1,2,3,....
    marcador: '*' para linha de comentario;
              ' ' para linha de comando;
              '-' para continuacao de linha;  
    texto: lista de tokens.
	*/
    int         numeracao;
    char        marcador;
	TokenList * texto;  
	
}Linha;

typedef struct _SaidaCobol
{
	/* 
    Lista duplamente encadeada de linhas.
    Forma o texto do arquivo de saida.
    */
	Linha              * linha;
	struct _SaidaCobol * anterior;
	struct _SaidaCobol * proximo;
}SaidaCobol;

/* variaveis globais */
extern Escopo * escopo;
extern Escopo * escopoAtual;
extern FILE * yyin;
extern int idEscopo;
SaidaCobol * saidaCobol;
Linha * printbuff;

/* prototipos */
void         init(int, char * []);
char       * nomeProgramaCob(char *);
void         initIdDivision(char *);
void         initProcDivision();
void         fechaMain();
void         inserirSaida(Linha *);
void         pularLinha();
Linha      * criarLinhaA();
Linha      * criarLinhaB();
Linha      * criarComentario();
void         inserirToken(Linha **, char *);
void         organizarSaida();
void         limparPrintBuff();
char       * imprimirTL(TokenList *);
void         escreverArquivo(char *);

/* implementacao */
void init(int argc, char *argv[])
{

	if(argc < 2)
	{
		printf("Too few arguments.\n");
		exit(1);
	}

	escopo = NULL;
	idEscopo = 0;

	FILE * myfile = fopen(argv[1], "r");
  
	if(myfile)
	{

		char * nomePrograma = nomeProgramaCob(argv[1]);
		initIdDivision(nomePrograma);

		yyin = myfile;
	    initEscopo();
		yyparse();
        //imprimeEscopos(listaDeEscopo);
        imprimeSimbolos(listaDeEscopo);


	    organizarSaida();
		escreverArquivo(nomePrograma);
	}
	else
	{
		printf("Arquivo nao encontrado. Abortando...");
		scanf("%*c");
		exit(1);
	}
}

char * nomeProgramaCob(char * argv)
{
	int tamanho = strlen(argv) + 4;
	char auxArq[tamanho];
	strcpy(auxArq, argv);

	int i;
	for(i = 2; auxArq[i] != '.' ;i++);
        auxArq[i] = '\0';

	strcat(auxArq,".cob");
    return strdup(auxArq);
}

void initIdDivision(char * arqCob)
{

	//init saidaCobol e printbuff
	saidaCobol = NULL;
	printbuff = NULL;

	Linha * linha = criarComentario();
	inserirToken(&linha, arqCob);
	inserirToken(&linha, "C it as Cobol");
	inserirToken(&linha, "FAQ example.");
	inserirSaida(linha);
    
	Linha * linha2 = criarLinhaA();
	inserirToken(&linha2, "IDENTIFICATION DIVISION");
	inserirSaida(linha2);

	char nomeProg[strlen(arqCob)];
	strcpy(nomeProg,arqCob);

    int i;
	for(i = 2; nomeProg[i] != '.' ;i++);
        nomeProg[i] = '\0';

	Linha * linha3 = criarLinhaA();
    inserirToken(&linha3, "PROGRAM-ID.");
	inserirToken(&linha3, strdup(nomeProg));
	inserirSaida(linha3); 

}

void initProcDivision(char * argmain)
{
    pularLinha();

	Linha * linha = criarLinhaA();
    inserirToken(&linha,"PROCEDURE DIVISION");
	inserirSaida(linha);

	pularLinha();

	criaEscopo(argmain);
}

void fechaMain()
{
    limparPrintBuff();

    pularLinha();

	Linha * linha = criarLinhaB();
    inserirToken(&linha,"STOP RUN");
	inserirSaida(linha);

    Linha * linha2 = criarLinhaB();
    inserirToken(&linha2,"EXIT");
	inserirSaida(linha2);

}


void inserirSaida(Linha * linha)
{

	SaidaCobol * novoElemento =
	    (SaidaCobol *) malloc(sizeof(SaidaCobol));
	novoElemento->linha = linha;
	novoElemento->proximo = NULL;
    
    if(saidaCobol == NULL)
    {
    	saidaCobol = novoElemento;
        saidaCobol->anterior = NULL;

    }
    else 
    {
    	SaidaCobol * cursor = saidaCobol;

	    while (cursor->proximo != NULL)
	    {
		    cursor = cursor->proximo;
	    }
	    cursor->proximo = novoElemento;
	    novoElemento->anterior = cursor;
	}

}

void pularLinha()
{

	Linha * linha = criarLinhaA(linha);
	inserirSaida(linha);

}

Linha * criarLinhaA()
{
    Linha * linha = (Linha *) malloc(sizeof(Linha));
    linha->numeracao = 0;
    linha->marcador = ' ';
    linha->texto = NULL;
    return linha;
}


Linha * criarLinhaB()
{
    Linha * linha = (Linha *) malloc(sizeof(Linha));
    linha->numeracao = 0;
    linha->marcador = ' ';
    linha->texto = NULL;
    inserirToken(&linha,"   ");
    return linha;

}

Linha * criarComentario()
{
    Linha * linha = (Linha *) malloc(sizeof(Linha));
    linha->numeracao = 0;
    linha->marcador = '*';
    linha->texto = NULL;
    return linha;
}

void inserirToken(Linha ** linha, char * token)
{

    TokenList * novoTk = 
        (TokenList *) malloc(sizeof(TokenList));
    novoTk->token = token;
    novoTk->proximo = NULL;

    TokenList * cursor = (*linha)->texto;

    if(cursor == NULL)
    {
        (*linha)->texto = novoTk;
    }
    else
    {
    	Linha * guardaLinha = *linha;

        while (cursor->proximo != NULL)
	    {
		    cursor = cursor->proximo;
	    }
        cursor->proximo = novoTk;

        *linha = guardaLinha;
    }

}

void organizarSaida()
{
	if(saidaCobol == NULL)
    {
		printf("%s\n", "Saida em branco.");
		scanf("%*c");
		exit(1);
    }

    SaidaCobol * cursor = saidaCobol;
	
	int i = 1;
	while(cursor != NULL)
	{
		cursor->linha->numeracao = i;
        i++;
	    cursor = cursor->proximo;
	}

}

char * imprimirTL(TokenList * TL)
{
	char saida[65] = "";

	if(TL != NULL)
	{
		TokenList * cursor = TL;
	    strcat(saida,cursor->token);

		while(cursor->proximo != NULL)
		{
			strcat(saida," ");
			strcat(saida,cursor->proximo->token);
			cursor = cursor->proximo;
		}
	}

    return strdup(saida);

}

void escreverArquivo(char * arq)
{
	FILE * file = fopen(arq, "w+");

    if(saidaCobol == NULL)
    {
		printf("%s\n", "Saida em branco.");
		scanf("%*c");
		exit(1);
    }

	while(saidaCobol != NULL)
	{

		if((saidaCobol->linha->marcador == '*') || 
			(saidaCobol->linha->texto == NULL))
		{
	        fprintf(file, "%06d%c%s\n", 
	        	saidaCobol->linha->numeracao,
	            saidaCobol->linha->marcador,
	            imprimirTL(saidaCobol->linha->texto));
	    }
	    else
	    {
	    	fprintf(file, "%06d%c%s.\n", 
	            saidaCobol->linha->numeracao,
	    		saidaCobol->linha->marcador,
	            imprimirTL(saidaCobol->linha->texto));
	    }
	    saidaCobol = saidaCobol->proximo;
	}

	fclose(file);
}