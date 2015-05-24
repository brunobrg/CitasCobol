#include "estruturaC.h"
#include <stdio.h>


/*
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
*/

/* variaveis 
int idEscopo;
Escopo * escopo;
Escopo * escopoAtual;
ListaDeEscopo * listaDeEscopo;
Simbolos * listaDeVariaveis;

*/

/* implementacao 
void entraEscopo(Escopo * temp)
{
	escopoAtual = temp;	
	printf("entrou no escopo: %s\n", escopoAtual->nome);
}

ListaDeEscopo * addLista()
{
	ListaDeEscopo * addLista = (ListaDeEscopo*) malloc(sizeof(ListaDeEscopo));
	addLista->proximo = NULL;
	addLista->escopo = NULL;

	return addLista;
}

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
		add = addEscopo( newName, escopoAtual);
		ListaDeEscopo * aux = listaDeEscopo;
		ListaDeEscopo * newLista = addLista();
		
		while(aux->proximo != NULL)
			aux = aux->proximo;

		aux->proximo = newLista;
		aux->proximo->escopo = add;
	}
	entraEscopo(add);
}

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

void saiEscopo()
{
	printf("saiu do escopo: %s\n", escopoAtual->nome);
	entraEscopo(escopoAtual->anterior);
}

void initEscopo()
{
	listaDeVariaveis = NULL;
	listaDeEscopo = NULL;
	criaEscopo("global");

}


Simbolos * addSimbolo(char tipo[40], char nome[40])
{
	Simbolos * add = (Simbolos *) malloc(sizeof(Simbolos));
	strcpy(add->tipo, tipo);
	strcpy(add->nome, nome);
	add->proximo = NULL;
	add->value = NULL;
}

void valorSimbolo(Escopo * escTemp, char tipo[40], char nomeSimbolo[40], char * valor)
{
	if(escTemp != NULL)
	{
		Simbolos * aux = escTemp->s_declarados;
		while(aux != NULL)
		{
			if(!strcmp(aux->nome, nomeSimbolo) && !strcmp(aux->tipo, tipo))
			{
				aux->value = valor;
				printf("\nEncontrou!\n %s = %s\n", aux->nome, aux->value);
				return;
			}

			aux = aux->proximo;
		}
	}
}

void adicionaSimbolo(Escopo * escTemp, char pos[40], char tipo[40], char nomeSimbolo[40])
{
	if(escTemp != NULL)
	{
		if(!strcmp(pos, "declarada"))
		{
			Simbolos * add = addSimbolo(tipo, nomeSimbolo);

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

			Simbolos * add2 = addSimbolo(tipo, nomeSimbolo);

			if(listaDeVariaveis == NULL)
				listaDeVariaveis = add2;
			else
			{
				Simbolos * aux2 = listaDeVariaveis;
				
				while(aux2->proximo!= NULL)
					aux2 = aux2->proximo;

				aux2->proximo = add2;
			}

		}
		else if(!strcmp(pos, "usada"))
		{
			Simbolos * add = addSimbolo(tipo, nomeSimbolo);

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

void imprimeSimbolos(ListaDeEscopo * aux)
{
	if(aux!= NULL)
	{
		if(aux->escopo->s_declarados != NULL)
		{
			printf("\nEscopo: %s\nvariaveis: ", aux->escopo->nome);
			Simbolos * temp = aux->escopo->s_declarados;

			while(temp != NULL)
			{
				printf("%s %s, ", temp->tipo, temp->nome);
				temp = temp->proximo;
			}
		}

		imprimeSimbolos(aux->proximo);
	}
}

*/

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
		printf("\" %s %s \" declarada e nao utilizada\n", dec->tipo, dec->nome);
		return 0 * verificaSimbolo(dec->proximo, usaHead);
	}
}



int verificaEscopo(Escopo * aux)
{
	if(aux == NULL)
		return 1;
	else
	{
		printf("\nDentro de: %s\n", aux->nome);
		return 1 * verificaSimbolo(aux->s_declarados, aux->s_usados) ;
	}
}

int verificaLista(ListaDeEscopo * aux)
{
	if(aux != NULL)
	{
		return 1 * verificaLista(aux->proximo) * verificaEscopo(aux->escopo);
	}
	else
		return 1;
}

/*

void adicionaSimbolos()
{
	adicionaSimbolo(escopoAtual, "usada", "int", "x");
	adicionaSimbolo(escopoAtual, "usada", "int", "y");
	adicionaSimbolo(escopoAtual, "usada", "int", "z");
	adicionaSimbolo(escopoAtual, "usada", "int", "x1");
	adicionaSimbolo(escopoAtual, "usada", "int", "x2");
	adicionaSimbolo(escopoAtual, "usada", "int", "x3");
	adicionaSimbolo(escopoAtual, "usada", "int", "x4");
	adicionaSimbolo(escopoAtual, "usada", "int", "x5");

}

*/