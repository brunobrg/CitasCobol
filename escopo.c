#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturaC.h"

/* VARIAVEIS GLOBAIS */
int               idEscopo = 0;
Escopo          * escopoAtual;
ListaDeEscopo   * listaDeEscopo;
Simbolos        * listaDeVariaveis;  

/* IMPLEMENTACAO */

/* */
void entraEscopo(Escopo * temp)
{
	escopoAtual = temp;	
	printf("*** entrou no escopo: %s\n", temp->nome);
}

/* */
ListaDeEscopo * addLista()
{
	ListaDeEscopo * addLista = (ListaDeEscopo*) malloc(sizeof(ListaDeEscopo));
	addLista->proximo = NULL;
	addLista->escopo = NULL;

	return addLista;
}

/* */
Escopo * addEscopo(char newName[40], Escopo * atual)
{
	
	Escopo * add = (Escopo*) malloc(sizeof(Escopo));
	if(atual != NULL)
	{	
		strcpy(add->nome, atual->nome);
		strcat(add->nome, ".");
		strcat(add->nome, newName);
		add->id = idEscopo;
		add->s_declarados = atual->s_declarados;
		add->s_usados = atual->s_usados;
		add->anterior = atual;
	}
	else
	{
		strcpy(add->nome, newName);
		add->id = idEscopo;
		add->s_declarados = NULL;
		add->s_usados = NULL;
		add->anterior = NULL;
	}
	return add;
}

/* */
void criaEscopo(char newName[40])
{
	idEscopo+=1;
	Escopo * add;
	if(listaDeEscopo == NULL)
	{
		add = addEscopo(newName, NULL);
		listaDeEscopo = addLista();
		listaDeEscopo->escopo = add;
	}
	else
	{
		add = addEscopo(newName, escopoAtual);
		ListaDeEscopo * aux = listaDeEscopo;
		ListaDeEscopo * newLista = addLista();
		
		while(aux->proximo != NULL)
			aux = aux->proximo;

		aux->proximo = newLista;
		aux->proximo->escopo = add;
	}
	entraEscopo(add);
}

/* Inicializa o escopo global */
void initEscopo()
{
	listaDeVariaveis = NULL;
	listaDeEscopo = NULL;
	criaEscopo("global");
}

/* Diz que saiu do escopo atual */
void terminaEscopo()
{
    printf("*** saiu do escopo: %s\n", escopoAtual->nome);
}

/* Sai do escopo atual e volta para o escopo anterior */
void saiEscopo()
{
	terminaEscopo();
	escopoAtual = escopoAtual->anterior;	
}

/* */
Simbolos * addSimbolo(char * tipo, char * nome, char * valor)
{
	Simbolos * add = (Simbolos *) malloc(sizeof(Simbolos));
	strcpy(add->tipo, tipo);
	strcpy(add->nome, nome);
	add->proximo = NULL;
	if (valor == NULL)
	{
	    add->value = NULL;
	}
	else
    {
    	add->value = (char *) malloc((strlen(valor)+1)*sizeof(char));
    	strcpy(add->value, valor);
    }
    return add;
}

/* */
void adicionaSimbolo(Escopo * escTemp, char pos[40], char tipo[40], 
	char nomeSimbolo[40], char * valor)
{
	if(escTemp != NULL)
	{
		if(strcmp(pos, "declarada") == 0)
		{
			Simbolos * add = addSimbolo(tipo, nomeSimbolo, valor);

			if(escTemp->s_declarados == NULL)
			{
				escTemp->s_declarados = add;
			}
			else
			{
				Simbolos * aux = escTemp->s_declarados;
				
				while(aux->proximo!= NULL)
					aux = aux->proximo;

				aux->proximo = add;
			}

			Simbolos * add2 = addSimbolo(tipo, nomeSimbolo, valor);

			if(listaDeVariaveis == NULL)
			{
				listaDeVariaveis = add2;
			}
			else
			{
				Simbolos * aux2 = listaDeVariaveis;
				
				while(aux2->proximo!= NULL)
					aux2 = aux2->proximo;

				aux2->proximo = add2;
			}



		}
		else if(strcmp(pos, "usada") == 0)
		{
			Simbolos * add = addSimbolo(tipo, nomeSimbolo, valor);

			if(escTemp->s_usados == NULL)
			{
				escTemp->s_usados = add;
			}
			else
			{
				Simbolos * aux = escTemp->s_usados;
				
				while(aux->proximo!= NULL)
					aux = aux->proximo;

				aux->proximo = add;
			}
		}

	}
}

/* */
void imprimeEscopos(ListaDeEscopo * aux)
{
	int posLista = 1;
	while(aux != NULL)
	{
		printf("posLista: %d, ", posLista); 
		if(aux->escopo != NULL)
		{
			printf("escopo.nome: %s\n", aux->escopo->nome);
			printf("anterior? %s\n", aux->escopo->anterior->nome);
		}
		posLista += 1;
		aux = aux->proximo;
	}
}

/* */
int verificaSimbolo(Simbolos * dec, Simbolos * usaHead)
{
	if(dec == NULL)
		return 1;
	else
	{
		Simbolos * usa = usaHead;
		while(usa !=NULL)
		{
			if(!strcmp(dec->nome, usa->nome))
			{
				return 1 * verificaSimbolo(dec->proximo, usaHead);
			}
			
			usa = usa->proximo;
		}
		char msg[50];
		sprintf(msg, "\"%s %s\" declarada e nao utilizada.",
		    dec->tipo, dec->nome);
		warning(msg);
		return 0 * verificaSimbolo(dec->proximo, usaHead);
	}
}

/* */
int verificaEscopo(Escopo * aux)
{
	if(aux == NULL)
		return 1;
	else
	{
		printf("*** Dentro de: %s\n", aux->nome);
		return 1 * verificaSimbolo(aux->s_declarados, aux->s_usados) ;
	}
}

/* */
int verificaLista(ListaDeEscopo * aux)
{
	if(aux != NULL)
	{
		return 1 * verificaLista(aux->proximo) * verificaEscopo(aux->escopo);
	}
	else
		return 1;
}

/* */
int verificaListaEscopo()
{
    return verificaLista(listaDeEscopo);
}

/* */
void adicionaSimbolos()
{
adicionaSimbolo(escopoAtual, "usada", "int", "x", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "y", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "z", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "x1", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "x2", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "x3", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "x4", NULL);
adicionaSimbolo(escopoAtual, "usada", "int", "x5", NULL);
}