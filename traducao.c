#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "estruturaC.h"
#include "estruturaCobol.h"
#include "traducao.h"

/* VARIAVEIS GLOBAIS */
int                contLinhasCobol = 1;   /* Contagem de linhas do 
                                             arquivo de saida */

/* IMPLEMENTACAO */

/* Recebe o nome do programa.c e devolve o nome do programa .cob */
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

/* Retorna string contendo os tokens de uma Tokenlist */
char * imprimirTL(TokenList * TL)
{
	int dentroDeQuote = 0;
	char saida[65] = "";

	if(TL != NULL)
	{
		TokenList * cursor = TL;
	    strcat(saida,cursor->token); 

	    if(cursor->token[strlen(cursor->token)-1] == '\"')
	    {
	        dentroDeQuote = 0;

	    } else if (cursor->token[0] == '\"')
	    {
	        dentroDeQuote = 1;
	    }  

		while(cursor->proximo != NULL)
		{
			if(!dentroDeQuote)
			    strcat(saida," "); 

			strcat(saida,cursor->proximo->token);

		    if(cursor->proximo->token[strlen(cursor->proximo->token)-1]
		       == '\"')
		    {
		        dentroDeQuote = 0;
		    } else if (cursor->proximo->token[0] == '\"')
		    {
		        dentroDeQuote = 1;
		    }  		

			cursor = cursor->proximo;
		}
	}

    return strdup(saida);

}

/* Isere um token em uma linha */
void inserirTokenApos(Linha ** linha, char * token, int posicao)
{

    TokenList * novoTk = 
        (TokenList *) malloc(sizeof(TokenList));
    novoTk->token = (char *) malloc((strlen(token)+1)*sizeof(char));
    strcpy(novoTk->token,token);
    novoTk->tklen = strlen(token);

    ((*linha)->qntToks)++;

    TokenList * cursor = (*linha)->texto;

    if(cursor == NULL)
    {
        (*linha)->texto = novoTk;
        novoTk->proximo = NULL;
    } 
    else
    {
    	Linha * guardaLinha = *linha;

        int i;
        for(i=0;i<posicao;i++)
        {
            if(cursor->proximo != NULL)
            {
        	    cursor = cursor->proximo;
        	} 
        }    
        novoTk->proximo = cursor->proximo;
        cursor->proximo = novoTk;
        *linha = guardaLinha;
    } 


}

/* Isere tokens que fazem parte de um comentario */
void inserirToksInsideQuote(Linha ** linha, char * token, int separador)
{
	if(separador == 34)
		separador++;  /* O caracter 34 eh a aspa ". Causa problemas. */

    char * quebraTk = (char *) malloc((strlen(token)+2)*sizeof(char));
    char * sep = (char *) malloc(2*sizeof(char));
    sprintf(sep,"%c",separador);
    char * guardaTk = (char *) malloc((strlen(token)+2)*sizeof(char));
    strcpy(guardaTk,token);

    while(quebraTk = strsep (&token, sep))
    {

    	/* Verifica se ha string apos o separador*/
    	char * guardaSeparador = (char *) 
    	    malloc((strlen(quebraTk)+2)*sizeof(char));
    	if (strcmp(quebraTk,guardaTk) != 0)
    	{
    		strcpy(guardaSeparador,quebraTk);
    		strcat(guardaSeparador,sep);
    		quebraTk = guardaSeparador;
    	} 

    	if(strlen(quebraTk) < 53)
    	{
            inserirTokenApos(linha,quebraTk,(*linha)->qntToks);
        } 
        else
        { 
            inserirToksInsideQuote(linha,quebraTk,separador+1);
        }
        if(token != NULL)
            strcpy(guardaTk,token);

    }
}

/* Isere um token ao final de uma linha */
void inserirToken(Linha ** linha, char * token)
{
	
	if (token[1] != '\"' && strlen(token) <= 30)
	{
        inserirTokenApos(linha,token,(*linha)->qntToks);
    } else if (token[0] != '\"')
    {
		erro(4);
	} else 
    {
    	inserirToksInsideQuote(linha,token,32); /* 32 eh o primeiro char
    	                                        ascii, que corresponde 
    	                                        ao espaco */
    }

}

/* Retorna uma linha com inicio na coluna A */
Linha * criarLinhaA()
{
    Linha * linha = (Linha *) malloc(sizeof(Linha));
    linha->numeracao = 0;
    linha->qntToks = 0;
    linha->marcador = ' ';
    linha->texto = NULL;
    return linha;
}

/* Retorna uma linha com inicio na coluna B */
Linha * criarLinhaB()
{
    Linha * linha = criarLinhaA();
    inserirToken(&linha,"   ");
    return linha;
}

/* Retorna uma linha de comentario vazia */
Linha * criarComentario()
{
    Linha * linha = criarLinhaA();
    linha->marcador = '*';
    return linha;
}

/* Retorna uma linha de continuacao */
Linha * criarContinuacao()
{
    Linha * linha = criarLinhaB();
    linha->marcador = '-';
    return linha;
}

/* retorna uma linha em branco */
Linha * linhaVazia()
{

	Linha * linha = criarLinhaA();
	return linha;
}

/* Insere linha em um BlocoCobol na posicao dada */
void inserirLinha(SaidaCobol ** saida, Linha * linha,  int posicao)
{

	BlocoCobol * novoBloco =
	    (BlocoCobol *) malloc(sizeof(BlocoCobol));
	novoBloco->linha = linha;
	novoBloco->proximo = NULL;

    if((*saida)->conteudo == NULL)
    {
        (*saida)->conteudo = novoBloco;
        (*saida)->conteudo->anterior = NULL;
        (*saida)->conteudo->proximo = NULL;
    	(*saida)->qntLinhas = 1;
    }
    else 
    {
    	BlocoCobol * cursor = (*saida)->conteudo;

        int i;
        for(i=0;i<posicao;i++)
        {
            if(cursor->proximo != NULL)
            {
        	    cursor = cursor->proximo;
        	    novoBloco->proximo = cursor->proximo;
        	}
        }
	    novoBloco->anterior = cursor;
	    cursor->proximo = novoBloco;
	    ((*saida)->qntLinhas)++;
	}

}

/* Insere linha ao final da Identification Division */
void inserirIdntDiv(SaidaCobol ** saidaCobol, Linha * linha)
{              
    SaidaCobol ** idtDiv = saidaCobol; /* identification division */
    int posicao = 0;
    if ((*idtDiv)->conteudo != NULL)
        posicao = (*idtDiv)->qntLinhas;
    inserirLinha(idtDiv,linha,posicao);
}

/* Insere linha ao final da Data Division */
void inserirDataDiv(SaidaCobol ** saidaCobol, Linha * linha)
{
	SaidaCobol ** dtaDiv = &((*saidaCobol)->lateral->lateral); 
	                                             /* data division */               
    int posicao = 0;
    if ((*dtaDiv)->conteudo != NULL)
        posicao = (*dtaDiv)->qntLinhas;
    inserirLinha(dtaDiv,linha,posicao);
}

/* Insere linha ao final da Procedure Division */
void inserirProcDiv(SaidaCobol ** saidaCobol, Linha * linha)
{
	SaidaCobol ** prcDiv = &((*saidaCobol)->lateral->lateral->lateral);
	                                        /* procedure division */               
    int posicao = 0;
    if ((*prcDiv)->conteudo != NULL)
        posicao = (*prcDiv)->qntLinhas;
    inserirLinha(prcDiv,linha,posicao);
}

/* Cria o tronco principal da arvore SaidaCobol com as 4 divisions */
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

	(*head)->qntLinhas = 0;
	environmentDivision->qntLinhas = 0;
	dataDivision->qntLinhas = 0;
	procedureDivision->qntLinhas = 0;

}

/* Quebra linhas com mais de 65 caracteres no conteudo de uma 
   saidaCobol */
void quebraLinhas(SaidaCobol ** saida)
{

	if (*saida != NULL)
	{
		int posicao = 0;
		int dentroDeQuote = 0;
		BlocoCobol * cursor = (*saida)->conteudo;

		while(cursor != NULL)
		{

			TokenList * tk = cursor->linha->texto;
			if(tk != NULL)
			{

			    if(tk->token[strlen(tk->token)-1] == '\"')
			    {
			        dentroDeQuote = 0;
			    } else if (tk->token[0] == '\"')
			    {
			        dentroDeQuote = 1;
			    }  

				int acumulaTam = tk->tklen + 1 - dentroDeQuote;

			    while(tk->proximo != NULL)
				{

				    acumulaTam += tk->proximo->tklen + 1 
				                               - dentroDeQuote;
				    if (acumulaTam > 64)
				    {

				        Linha * novaLinha = criarContinuacao();
				        if(dentroDeQuote)
				        {
				        	char * aspatok = (char *) 
				        	    malloc((strlen(tk->proximo->token)+2)
				        	    *sizeof(char));
				        	strcpy(aspatok,"\"");
				        	strcat(aspatok,tk->proximo->token);
				        	tk->proximo->token = aspatok;
				        	tk->proximo->tklen = tk->proximo->tklen + 1;

							char * tokaspa = (char *)
							    malloc((strlen(tk->token)+3)
							    *sizeof(char)+2);
							strcpy(tokaspa,tk->token);
							strcat(tokaspa,"\"");
				        	tk->token = tokaspa;
				        	tk->tklen = tk->tklen + 1;

				        } 
		        		inserirTokenApos(&novaLinha, "       ",1);
		        		novaLinha->texto->proximo->proximo = tk->proximo;
		        		tk->proximo = NULL;
		        	    inserirLinha(saida, novaLinha, posicao);

				    } else
				    {
				        tk = tk->proximo;
				    }

					if(tk->token[strlen(tk->token)-1] == '\"')
					{
					     dentroDeQuote = 0;
					} else if (tk->token[0] == '\"')
					{
					    dentroDeQuote = 1;
					}

				}
		    }
		    cursor = cursor->proximo;
		    posicao++;
		}

	}
}

/* Numera as linhas de um blocoCobol */
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

/* Organiza uma saidaCobol, quebrando as linhas grandes e numerando 
as linhas */
void organizarSaida(SaidaCobol ** saidaCobol)
{
	quebraLinhas(saidaCobol); 
    SaidaCobol * saidaAux = *saidaCobol;
	if(saidaAux != NULL)
	{
		organizarBloco(&(saidaAux->conteudo));
		organizarSaida(&(saidaAux->inferior));
		organizarSaida(&(saidaAux->lateral));
	}

}

/* Imprime o conteudo de um blocoCobol no arquivo de saida */
void escreverBloco(FILE * file, BlocoCobol * blocoCobol)
{

	while(blocoCobol != NULL)
	{

		if((blocoCobol->linha == NULL)  ||
		   (blocoCobol->linha->texto == NULL) ||
		   (blocoCobol->linha->marcador == '*') ||
		   (blocoCobol->proximo != NULL &&
		    blocoCobol->proximo->linha->marcador == '-'))
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

/* Imprime o conteudo de uma saidaCobol no arquivo de saida */
void escreverArquivo(FILE * file, SaidaCobol * saidaCobol)
{

	if(saidaCobol != NULL)
	{
		escreverBloco(file,saidaCobol->conteudo);
		escreverArquivo(file,saidaCobol->inferior);
		escreverArquivo(file,saidaCobol->lateral);
	}

}
