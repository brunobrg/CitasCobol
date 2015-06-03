#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "estruturaC.h"
#include "estruturaCobol.h"
#include "traducao.h"

/* variaveis globais */
int                contLinhasCobol = 1;
extern Simbolos  * listaDeVariaveis;  


/* implementacao */
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

void inserirToken(Linha ** linha, char * token)
{

    TokenList * novoTk = 
        (TokenList *) malloc(sizeof(TokenList));
    novoTk->token = token;
    novoTk->proximo = NULL;
    novoTk->tklen = strlen(token);

    ((*linha)->qntToks)++;

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

Linha * criarLinhaA()
{
    Linha * linha = (Linha *) malloc(sizeof(Linha));
    linha->numeracao = 0;
    linha->qntToks = 0;
    linha->marcador = ' ';
    linha->texto = NULL;
    return linha;
}

Linha * criarLinhaB()
{
    Linha * linha = criarLinhaA();
    inserirToken(&linha,"   ");
    return linha;
}

Linha * criarComentario()
{
    Linha * linha = criarLinhaA();
    linha->marcador = '*';
    return linha;
}

Linha * linhaVazia()
{

	Linha * linha = criarLinhaA();
	return linha;
}

void inserirBloco(BlocoCobol ** bloco, Linha * linha,  int posicao)
{

	BlocoCobol * novoElemento =
	    (BlocoCobol *) malloc(sizeof(BlocoCobol));
	novoElemento->linha = linha;
	novoElemento->proximo = NULL;

    if(*bloco == NULL)
    {
    	*bloco = novoElemento;
        (*bloco)->anterior = NULL;
        (*bloco)->qntLinhas = 1;

    }
    else 
    {
    	BlocoCobol * cursor = *bloco;

        int i;
        for(i=0;i<posicao;i++)
        {
            if(cursor->proximo != NULL)
        	    cursor = cursor->proximo;
        }

	    cursor->proximo = novoElemento;
	    novoElemento->anterior = cursor;
	    ((*bloco)->qntLinhas)++;

	}

}

void inserirIdntDiv(SaidaCobol ** saidaCobol, Linha * linha)
{              
    BlocoCobol ** idtDiv = &((*saidaCobol)->conteudo);
    int posicao = 0;
    if (*idtDiv != NULL)
    	posicao = (*idtDiv)->qntLinhas;
    inserirBloco(idtDiv,linha,posicao);
}

void inserirDataDiv(SaidaCobol ** saidaCobol, Linha * linha)
{
	SaidaCobol * cursor = (*saidaCobol)->lateral; /* environment div */
	cursor = cursor->lateral; /* data division */               
    BlocoCobol ** dtaDiv = &(cursor->conteudo);
    int posicao = 0;
    if (*dtaDiv != NULL)
    	posicao = (*dtaDiv)->qntLinhas;
    inserirBloco(dtaDiv,linha,posicao);
}

void inserirProcDiv(SaidaCobol ** saidaCobol, Linha * linha)
{
	SaidaCobol * cursor = (*saidaCobol)->lateral; /* environment div */
	cursor = cursor->lateral; /* data division */               
    cursor = cursor->lateral; /* procedure division */
    BlocoCobol ** prcDiv = &(cursor->conteudo);
    int posicao = 0;
    if (*prcDiv != NULL)
    	posicao = (*prcDiv)->qntLinhas;
    inserirBloco(prcDiv,linha,posicao);
}

void criarDivisions(SaidaCobol ** head)
{
	*head = 
	  (SaidaCobol *) malloc (sizeof(SaidaCobol));
	SaidaCobol * environmentDivision = 
	  (SaidaCobol *) malloc (sizeof(SaidaCobol));
	SaidaCobol * dataDivision = 
	  (SaidaCobol *) malloc (sizeof(SaidaCobol));
	SaidaCobol * procedureDivision = 
	  (SaidaCobol *) malloc (sizeof(SaidaCobol));

	(*head)->identificacao = "IDENTIFICATION DIVISION";
	environmentDivision->identificacao = "ENVIRONMENT DIVISION";
	dataDivision->identificacao = "DATA DIVISION";
	procedureDivision->identificacao = "PROCEDURE DIVISION";

	(*head)->lateral = environmentDivision;
	environmentDivision->lateral = dataDivision;
	dataDivision->lateral = procedureDivision;
	procedureDivision->lateral = NULL;

	(*head)->inferior = NULL;
	environmentDivision->inferior = NULL;
	dataDivision->inferior = NULL;
	procedureDivision->inferior = NULL;

	(*head)->conteudo = NULL;
	environmentDivision->conteudo = NULL;
	dataDivision->conteudo = NULL;
	procedureDivision->conteudo = NULL;
}


void escreverIdntDivision(SaidaCobol ** saidaCobol, char * arqCob)
{

    // Coloca virgula
    char * arq_virgula = (char *) 
        malloc((strlen(arqCob)+1)*sizeof(char));
    strcpy(arq_virgula, arqCob);
    strcat(arq_virgula, ",");

    // Cria primera linha
	Linha * linha = criarComentario();
	inserirToken(&linha, arq_virgula);
	inserirToken(&linha, "criado por C it as Cobol v0.2");
	inserirIdntDiv(saidaCobol,linha);
    
    // Cria proximas linhas 
	Linha * linha2 = criarLinhaA();
	inserirToken(&linha2, "IDENTIFICATION DIVISION");
	inserirIdntDiv(saidaCobol,linha2);

	char nomeProg[strlen(arqCob)];
	strcpy(nomeProg,arqCob);

    int i;
	for(i = 2; nomeProg[i] != '.' ;i++);
        nomeProg[i] = '\0';

	Linha * linha3 = criarLinhaA();
    inserirToken(&linha3, "PROGRAM-ID.");
	inserirToken(&linha3, strdup(nomeProg));
	inserirIdntDiv(saidaCobol,linha3); 

	Linha * linhaV = linhaVazia();
	inserirIdntDiv(saidaCobol,linhaV);

}

void escreverDataDivision(SaidaCobol ** saidaCobol)
{
	if(listaDeVariaveis != NULL)
	{
		Linha * linha = criarLinhaA();
		inserirToken(&linha, "DATA DIVISION");
		inserirDataDiv(saidaCobol,linha);

		linha = criarLinhaA();
		inserirToken(&linha, "WORKING-STORAGE SECTION");
		inserirDataDiv(saidaCobol,linha);

		Simbolos * aux = listaDeVariaveis;
		while(aux != NULL)
		{
			
			linha = criarLinhaB();
			if(strcmp(aux->tipo, "int"))
			{
				inserirToken(&linha, "01");
				inserirToken(&linha, aux->nome);
				inserirToken(&linha, "PIC S9(05)");
			}
			else if(strcmp(aux->tipo, "float"))
			{
				inserirToken(&linha, "01");
				inserirToken(&linha, aux->nome);
				inserirToken(&linha, "PIC S9(18)V9(18)");
			}
			else if(strcmp(aux->tipo, "char"))
			{
				inserirToken(&linha, "01");
				inserirToken(&linha, aux->nome);
				inserirToken(&linha, "PICTURE X(10)");
			}
			if(aux->value != NULL)
			{
				inserirToken(&linha, "VALUE");
				inserirToken(&linha, aux->value);
			}

			inserirDataDiv(saidaCobol,linha);

			aux = aux->proximo;
		}
		linha = criarLinhaA();
		inserirDataDiv(saidaCobol,linha);

	}

}

void escreverProcDivision(SaidaCobol ** saidaCobol)
{

	Linha * linha = criarLinhaA();
    inserirToken(&linha,"PROCEDURE DIVISION");
	inserirProcDiv(saidaCobol,linha);

	Linha * linhaV = linhaVazia();
	inserirProcDiv(saidaCobol,linhaV);

    Linha * linha2 = criarLinhaA();
    inserirToken(&linha2,"000000-MAIN");
    inserirToken(&linha2,"SECTION");
	inserirProcDiv(saidaCobol,linha2);

}

void fechaMainSection(SaidaCobol ** saidaCobol)
{

    limparPrintBuff(saidaCobol);

	Linha * linhaV = linhaVazia();
	inserirProcDiv(saidaCobol,linhaV);

	Linha * linha = criarLinhaB();
    inserirToken(&linha,"STOP RUN");
	inserirProcDiv(saidaCobol,linha);

	Linha * linha2 = criarLinhaB();
    inserirToken(&linha2,"000000-EXIT");
	inserirProcDiv(saidaCobol,linha2);

    Linha * linha3 = criarLinhaB();
    inserirToken(&linha3,"EXIT");
	inserirProcDiv(saidaCobol,linha3);

}

void organizarBloco(BlocoCobol ** blocoCobol)
{
	if(*blocoCobol != NULL)
    {
        BlocoCobol * cursor = *blocoCobol;
	
	    while(cursor != NULL)
	    {
	    	cursor->linha->numeracao = contLinhasCobol;
            contLinhasCobol++;
	        cursor = cursor->proximo;
	    }
    }
}

void organizarSaida(SaidaCobol ** saidaCobol)
{
    SaidaCobol * saidaAux = *saidaCobol;
	if(saidaAux != NULL)
	{
		organizarBloco(&(saidaAux->conteudo));
		organizarSaida(&(saidaAux->inferior));
		organizarSaida(&(saidaAux->lateral));
	}
}


void escreverBloco(FILE * file, BlocoCobol * blocoCobol)
{

	while(blocoCobol != NULL)
	{

		if((blocoCobol->linha->marcador == '*') || 
			(blocoCobol->linha->texto == NULL))
		{
	        fprintf(file, "%06d%c%s\n", 
	        	blocoCobol->linha->numeracao,
	            blocoCobol->linha->marcador,
	            imprimirTL(blocoCobol->linha->texto));
	    }
	    else
	    {
	    	fprintf(file, "%06d%c%s.\n", 
	            blocoCobol->linha->numeracao,
	    		blocoCobol->linha->marcador,
	            imprimirTL(blocoCobol->linha->texto));
	    }
	    blocoCobol = blocoCobol->proximo;
	}

}

void escreverArquivo(FILE * file, SaidaCobol * saidaCobol)
{

	if(saidaCobol != NULL)
	{
		escreverBloco(file,saidaCobol->conteudo);
		escreverArquivo(file,saidaCobol->inferior);
		escreverArquivo(file,saidaCobol->lateral);
	}

}
