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

typedef struct _SaidaCobol
{
	char palavra[30];
	struct _SaidaCobol * proximo;
}SaidaCobol;

/* variaveis globais */
int contLinhasCobol;
SaidaCobol * saidaCobol;
extern FILE * yyin;

/* prototipos */
void init(int, char *[]);
void initIdDivision(char[]);
void initProcDivision();
void inserirSaidaCobol(char[]);
SaidaCobol * addSaidaCobol(char[]);
void escreveArquivo(char[]);
void pulaLinha();


void init(int argc, char *argv[])
{
	char arq[20];


	if(argc < 2)
	{
		printf("Too few arguments.\n");
		exit(1);
	}

	initIdDivision(argv[1]);


	FILE * myfile = fopen(argv[1], "r");

	if(myfile)
	{
		yyin = myfile;
		yyparse();
		printf("Analise terminada\n");
		escreveArquivo(arq);
	}
	else
	{
		printf("arquivo nao encontrado. Abortando...");
		scanf("%*c");
		exit(1);
	}
}

void pulaLinha()
{
	char str[10];
	contLinhasCobol++;
	if(contLinhasCobol > 1)
		inserirSaidaCobol("\n");
	sprintf(str, "%04d", contLinhasCobol);
	inserirSaidaCobol(str);
	inserirSaidaCobol("00");
}

void initIdDivision(char arq[20])
{
	int i = 0;
	char auxArq[20];
	strcpy(auxArq, arq);
	for(i = 2; auxArq[i] != '.' ;i++);
		auxArq[i] = '\0';

	pulaLinha();
	inserirSaidaCobol("* ");
	inserirSaidaCobol(auxArq);
	inserirSaidaCobol(".cob ");
	inserirSaidaCobol("C it as Cobol ");
	inserirSaidaCobol("FAQ example");
	pulaLinha();
	inserirSaidaCobol(" IDENTIFICATION DIVISION.");
	pulaLinha();
	inserirSaidaCobol(" PROGRAM-ID. ");
	inserirSaidaCobol(auxArq);
	inserirSaidaCobol(".");
}

void initProcDivision()
{
	pulaLinha();
	inserirSaidaCobol(" PROCEDURE DIVISION.");
	pulaLinha();
}

SaidaCobol * addSaidaCobol(char strAux[30])
{
    SaidaCobol * add = (SaidaCobol*) malloc(sizeof(SaidaCobol));
    strcpy(add->palavra, strAux);
    add->proximo == NULL;
    return add;
}

void inserirSaidaCobol(char strAux[30])
{
	SaidaCobol * aux = addSaidaCobol(strAux);

	if(saidaCobol == NULL)
	{
		saidaCobol = aux;
	}
	else
	{
		SaidaCobol * aux2 = saidaCobol;
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
	SaidaCobol * aux = saidaCobol;

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
