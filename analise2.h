#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#define MAX 100

/* structs */
struct stat st = {0};

typedef struct _Tokens
{
	/* 
	CADEIA DE TOKENS QUE COMPOE 1 LINHA
    token: deve ter 30 posicoes, exceto se for quote;
    linha: deve ter 65 posicoes;
	*/
	char * token;
	struct _Tokens * proximo;
}Tokens;

typedef struct _SaidaCobol
{
	/* 
	CADEIA DE LINHAS QUE COMPOE 1 ARQUIVO
	numLinha: 1,2,3,....
    marcador: '*' para linha de comentario;
              ' ' para linha de comando;
              '-' para continuacao de linha;  
    linha: lista de tokens. Deve ter 65 posicoes;
	*/
	int     numLinha;
    char    marcador;
	Tokens * linha;  
	struct _SaidaCobol * anterior;
	struct _SaidaCobol * proximo;
}SaidaCobol;

/* variaveis globais */
int contLinhasCobol;
SaidaCobol * saidaCobol;
extern FILE * yyin;

/* prototipos */
void init(int, char *[]);
SaidaCobol * initIdDivision(SaidaCobol *,char *);
SaidaCobol * initProcDivision(SaidaCobol *);
SaidaCobol * inserirLinha(SaidaCobol *, char, Tokens *);
Tokens * criaLinha();
Tokens * inserirToken(Tokens *, char[]);
void escreveArquivo(SaidaCobol *, char *);


void init(int argc, char *argv[])
{

	if(argc < 2)
	{
		printf("Too few arguments.\n");
		exit(1);
	}

	saidaCobol = initIdDivision(saidaCobol, argv[1]);


	FILE * myfile = fopen(argv[1], "r");

	if(myfile)
	{
		yyin = myfile;
		yyparse();
		printf("Analise terminada\n");
		escreveArquivo(saidaCobol,argv[1]);
	}
	else
	{
		printf("arquivo nao encontrado. Abortando...");
		scanf("%*c");
		exit(1);
	}
}

SaidaCobol * initIdDivision(SaidaCobol * saidaCobol, char * arq)
{

	char auxArq[strlen(arq)];
	strcpy(auxArq, arq);

	int i;
	for(i = 2; auxArq[i] != '.' ;i++);
		auxArq[i] = '\0';

	char auxArqCob[strlen(auxArq + 4)];
	strcat(auxArqCob,auxArq);
	strcat(auxArqCob,".cob");

	Tokens * linha = (Tokens *) malloc(sizeof(Tokens));
	linha = inserirToken(linha, auxArqCob);
	linha = inserirToken(linha, "C it as Cobol");
	linha = inserirToken(linha, "FAQ example");
	saidaCobol = inserirLinha(saidaCobol,'*',linha);

	Tokens * linha2 = (Tokens *) malloc(sizeof(Tokens));
	linha2 = inserirToken(linha2, "IDENTIFICATION DIVISION.");
	saidaCobol = inserirLinha(saidaCobol,' ',linha2);

	Tokens * linha3 = (Tokens *) malloc(sizeof(Tokens));
    linha3 = inserirToken(linha3,"PROGRAM-ID.");
	linha3 = inserirToken(linha3,auxArq);
	linha3 = inserirToken(linha3,".");
	saidaCobol = inserirLinha(saidaCobol,' ',linha3);

}

void initProcDivision()
{
	pulaLinha();
	inserirToken(" PROCEDURE DIVISION.");
	pulaLinha();
}

void inserirLinha()
{
	char str[10];
	contLinhasCobol++;
	if(contLinhasCobol > 1)
		inserirSaidaCobol("\n");
	sprintf(str, "%04d", contLinhasCobol);
	inserirToken(str);
	inserirToken("00");
}


Token * criaToken(char strAux[30])
{
    Token * add = (Token*) malloc(sizeof(Token));
    strcpy(add->palavra, strAux);
    add->proximo == NULL;
    return add;
}

void inserirToken(char strAux[30])
{
	Token * aux = addToken(strAux);

	if(token == NULL)
	{
		token = aux;
	}
	else
	{
		Token * aux2 = token;
		while(aux2->proximo != NULL)
		{
			aux2 = aux2->proximo;
		}
		aux2->proximo = aux;
	}
}

void escreveArquivo(char arq[30])
{
	int i = 0;
	FILE * file;
	Token * aux = token;

	for(i = 2; arq[i] != '.' ;i++);
		arq[i] = '\0';
	strcat(arq, ".cob");

	file = fopen(arq, "w+");

	while(aux != NULL)
	{
		fprintf(file, "%s", aux->palavra);
		aux = aux->proximo;
	}

	fclose(file);
}
