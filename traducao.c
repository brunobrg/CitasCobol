#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "estruturaC.h"
#include "estruturaCobol.h"
#include "traducao.h"

/*
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "escopo.h"
#include "comandos.h"
*/

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


void inserirBloco(BlocoCobol ** bloco, Linha * linha)
{

	BlocoCobol * novoElemento =
	    (BlocoCobol *) malloc(sizeof(BlocoCobol));
	novoElemento->linha = linha;
	novoElemento->proximo = NULL;
    
    if(*bloco == NULL)
    {
    	*bloco = novoElemento;
        (*bloco)->anterior = NULL;

    }
    else 
    {
    	BlocoCobol * cursor = *bloco;

	    while (cursor->proximo != NULL)
	    {
		    cursor = cursor->proximo;
	    }
	    cursor->proximo = novoElemento;
	    novoElemento->anterior = cursor;
	}

}

void inserirProcDiv(SaidaCobol ** saidaCobol, Linha * linha)
{
	SaidaCobol * cursor = (*saidaCobol)->lateral; /* environment div */
	cursor = cursor->lateral; /* data division */               
    cursor = cursor->lateral; /* procedure division */
    BlocoCobol ** prcDiv = &(cursor->conteudo);
    inserirBloco(prcDiv,linha);
}

void inserirDataDiv(SaidaCobol ** saidaCobol, Linha * linha)
{
	SaidaCobol * cursor = (*saidaCobol)->lateral; /* environment div */
	cursor = cursor->lateral; /* data division */               
    BlocoCobol ** dtaDiv = &(cursor->conteudo);
    inserirBloco(dtaDiv,linha);
}

void pularLinha(BlocoCobol ** saida)
{

	Linha * linha;
	linha = criarLinhaA();
	inserirBloco(saida, linha);

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

    BlocoCobol ** idntDiv = &((*saidaCobol)->conteudo);

    // Coloca virgula
    char * arq_virgula = (char *) 
        malloc((strlen(arqCob)+1)*sizeof(char));
    strcpy(arq_virgula, arqCob);
    strcat(arq_virgula, ",");

    // Cria primera linha
	Linha * linha = criarComentario();
	inserirToken(&linha, arq_virgula);
	inserirToken(&linha, "criado por C it as Cobol v0.2");
	inserirBloco(idntDiv,linha);
    
    // Cria proximas linhas 
	Linha * linha2 = criarLinhaA();
	inserirToken(&linha2, "IDENTIFICATION DIVISION");
	inserirBloco(idntDiv,linha2);

	char nomeProg[strlen(arqCob)];
	strcpy(nomeProg,arqCob);

    int i;
	for(i = 2; nomeProg[i] != '.' ;i++);
        nomeProg[i] = '\0';

	Linha * linha3 = criarLinhaA();
    inserirToken(&linha3, "PROGRAM-ID.");
	inserirToken(&linha3, strdup(nomeProg));
	inserirBloco(idntDiv,linha3); 

	pularLinha(idntDiv);

}

void escreverProcDivision(SaidaCobol ** saidaCobol)
{
	SaidaCobol * cursor = (*saidaCobol)->lateral; /* environment div */
	cursor = cursor->lateral; /* data division */               
    cursor = cursor->lateral; /* procedure division */
    BlocoCobol ** prcDiv = &(cursor->conteudo);

	Linha * linha = criarLinhaA();
    inserirToken(&linha,"PROCEDURE DIVISION");
	inserirBloco(prcDiv,linha);

	pularLinha(prcDiv);

    Linha * linha2 = criarLinhaA();
    inserirToken(&linha2,"000000-MAIN");
    inserirToken(&linha2,"SECTION");
	inserirBloco(prcDiv,linha2);

	//criaEscopo("main");
}

void fechaMain(SaidaCobol ** saidaCobol)
{
	SaidaCobol * cursor = (*saidaCobol)->lateral; /* environment div */
	cursor = cursor->lateral; /* data division */               
    cursor = cursor->lateral; /* procedure division */
    BlocoCobol ** prcDiv = &(cursor->conteudo);

    //limparPrintBuff();

    pularLinha(prcDiv);

	Linha * linha = criarLinhaB();
    inserirToken(&linha,"STOP RUN");
	inserirBloco(prcDiv,linha);

	Linha * linha2 = criarLinhaB();
    inserirToken(&linha2,"000000-EXIT");
	inserirBloco(prcDiv,linha2);

    Linha * linha3 = criarLinhaB();
    inserirToken(&linha3,"EXIT");
	inserirBloco(prcDiv,linha3);

	//adicionaSimbolos();
    //saiEscopo();

}

void organizarBloco(BlocoCobol ** blocoCobol)
{
	if(*blocoCobol != NULL)
    {
        BlocoCobol * cursor = *blocoCobol;
	
	    int i = 1;
	    while(cursor != NULL)
	    {
	    	cursor->linha->numeracao = i;
            i++;
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

/*
void inserirSaidaVariaveis(Linha * linha)
{

	BlocoCobol * novoElemento =
	    (BlocoCobol *) malloc(sizeof(BlocoCobol));
	novoElemento->linha = linha;
	novoElemento->proximo = NULL;
    
    if(saidaVariaveis == NULL)
    {
    	saidaVariaveis = novoElemento;
        saidaVariaveis->anterior = NULL;

    }
    else 
    {
    	BlocoCobol * cursor = saidaVariaveis;

	    while (cursor->proximo != NULL)
	    {
		    cursor = cursor->proximo;
	    }
	    cursor->proximo = novoElemento;
	    novoElemento->anterior = cursor;
	}

}

void initDataDivision()
{
	saidaVariaveis = NULL;
	if(listaDeVariaveis != NULL)
	{
		Linha * linha = criarLinhaA();
		inserirToken(&linha, "DATA DIVISION");
		inserirSaidaVariaveis(linha);
		linha = criarLinhaA();
		inserirToken(&linha, "WORKING-STORAGE SECTION");
		inserirSaidaVariaveis(linha);

		Simbolos * aux = listaDeVariaveis;
		while(aux != NULL)
		{
			
			linha = criarLinhaB();
			if(!strcmp(aux->tipo, "int"))
			{
				inserirToken(&linha, "01");
				inserirToken(&linha, aux->nome);
				inserirToken(&linha, "PIC S9(05)");
			}
			else if(!strcmp(aux->tipo, "float"))
			{
				inserirToken(&linha, "01");
				inserirToken(&linha, aux->nome);
				inserirToken(&linha, "PIC S9(18)V9(18)");
			}
			else if(!strcmp(aux->tipo, "char"))
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

			inserirSaidaVariaveis(linha);

			aux = aux->proximo;
		}
		linha = criarLinhaA();
		inserirSaidaVariaveis(linha);

	}

	if(saidaVariaveis != NULL)
	{
		BlocoCobol * aux = saidaCobol;
		while(aux != NULL)
		{
			if(aux->linha->texto != NULL)
			{
				if(!strcmp(aux->linha->texto->token, "PROCEDURE DIVISION"))
				{
					break;
				}
			}
			aux = aux->proximo;
			
		}
		BlocoCobol * tail = aux;
		BlocoCobol * head = aux->anterior;

		head->proximo = saidaVariaveis;

		BlocoCobol * aux2 = head->proximo;
		while(aux2->proximo != NULL)
			aux2 = aux2->proximo;
		aux2->proximo = tail;
	}
}

*/