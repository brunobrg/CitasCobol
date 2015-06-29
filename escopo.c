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
	//printf("*** entrou no escopo: %s\n", temp->nome);
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
    //printf("*** saiu do escopo: %s\n", escopoAtual->nome);
}

/* Sai do escopo atual e volta para o escopo anterior */
void saiEscopo()
{
	terminaEscopo();
	escopoAtual = escopoAtual->anterior;	
}

/* */
Simbolos * addSimbolo(int l, char * tipo, char * nome, char * valor)
{
	if(!strcmp(tipo, "INT"))
	{
		if(valor != NULL)
		{
			//tratando float em int...
			int i;
			for(i = 0; i <= (int) strlen(valor); i++)
			{
				if(valor[i] == '.')
				{
					valor[i] = '\0';
					break;
				}
					
			}
		}

	}


	Simbolos * add = (Simbolos *) malloc(sizeof(Simbolos));
	add->linha = l;
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
void adicionaSimbolo(Escopo * escTemp, char pos[40], int l, char tipo[40], 
	char nomeSimbolo[40], char * valor)
{
	if(escTemp != NULL)
	{
		if(strcmp(pos, "declarada") == 0)
		{
			Simbolos * add = addSimbolo(l, tipo, nomeSimbolo, valor);

			if(escTemp->s_declarados == NULL)
			{
				escTemp->s_declarados = add;
			}
			else
			{
				Simbolos * aux = escTemp->s_declarados;
				if(strcmp(aux->nome, add->nome)==0)
				{
					char msg[100];
					sprintf(msg,"'%s' e '%s'  são iguais. Não devem haver variáveis com o mesmo nome.\n", aux->nome, add->nome);
					yyerror(msg);
					return;					
				}	
				while(aux->proximo!= NULL)
					aux = aux->proximo;

				aux->proximo = add;
			}

			Simbolos * add2 = addSimbolo(l, tipo, nomeSimbolo, valor);

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
			Simbolos * add = addSimbolo(l, tipo, nomeSimbolo, valor);

			if(escTemp->s_usados == NULL)
			{
				escTemp->s_usados = add;
			}
			else
			{
				Simbolos * aux = escTemp->s_usados;
				while(aux->proximo!= NULL)
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

void atualizaSimbolo(Simbolos **usada)
{
	Simbolos * temp = listaDeVariaveis;
	while(strcmp(temp->nome,(*usada)->nome) != 0)
		temp = temp->proximo;

	strcpy(temp->value,(*usada)->value);
}

/* */
int validaSimboloUsado(Simbolos **declarada, Simbolos **usada)
{

	Simbolos * auxAll = listaDeVariaveis;

	while(auxAll != NULL)
	{
		if(strcmp((*usada)->value, auxAll->nome) == 0)
		{
			sprintf((*usada)->value,"%s", auxAll->value);			
			atualizaSimbolo(usada);
		}
		auxAll = auxAll->proximo;
	}

	if(!strcmp((*declarada)->tipo, "INT") || !strcmp((*declarada)->tipo, "FLOAT"))
	{
		if((*usada)->value != NULL)
		{
			//tratando float em int...
			if(!strcmp((*declarada)->tipo, "INT"))
			{
				int i;
				for(i = 0; i <= (int) strlen((*usada)->value); i++)
				{
					
					if((*usada)->value[i] == '.')
					{
						(*usada)->value[i] = '\0';
						atualizaSimbolo(usada);
						break;
					}
						
				}
			}

			//tratando string em int...
			if((*usada)->value[0] == '\"')
			{
				char msg[256];
				sprintf(msg, "Variavel %s do tipo %s, porem estah sendo atribuindo um valor string", (*declarada)->nome, (*declarada)->tipo);
				warning(msg, (*usada)->linha);
				return 0;
			}
		}

	}

	if(!strcmp((*declarada)->tipo, "CHAR"))
	{
		if((*usada)->value[0] == '\"')
		{
			char msg[256];
			sprintf(msg, "Variavel %s do tipo %s, porem estah sendo atribuindo um valor string", (*declarada)->nome, (*declarada)->tipo);
			warning(msg, (*usada)->linha);
			return 0;
		}
		if(strlen((*usada)->value) > 3)
		{
			char msg[256];
			sprintf(msg, "Variavel %s do tipo %s, porem estah sendo atribuido mais de um char", (*declarada)->nome, (*declarada)->tipo);
			warning(msg, (*usada)->linha);
			return 0;
		}
	}


	
	return 1;
}

/* */
int verificaSimbolosUsados(Simbolos * decHead, Simbolos * usa)
{
	if(usa == NULL)
		return 1;
	else
	{
		Simbolos * dec = decHead;;
		while(dec !=NULL)
		{
			if(!strcmp(usa->nome, dec->nome))
			{
				return 1 * validaSimboloUsado(&dec, &usa) * verificaSimbolosUsados(decHead, usa->proximo);
			}
			
			dec = dec->proximo;
		}

		char msg[50];
		sprintf(msg, "\"%s %s\" utilizada e nao declarada.",
		    usa->tipo, usa->nome);
		warning(msg, usa->linha);
		return 0 * verificaSimbolosUsados(decHead, usa->proximo);
	}
}

/* */
int verificaSimbolosDeclarados(Simbolos * dec, Simbolos * usaHead)
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
				return 1 * verificaSimbolosDeclarados(dec->proximo, usaHead);
			}
			
			usa = usa->proximo;
		}

		char msg[50];
		sprintf(msg, "\"%s %s\" declarada e nao utilizada.",
		    dec->tipo, dec->nome);
		warning(msg, dec->linha);
		return 1 * verificaSimbolosDeclarados(dec->proximo, usaHead);
	}
}

/* */
int verificaEscopo(Escopo * aux)
{
	if(aux == NULL)
		return 1;
	else
	{
	//	printf("*** Dentro de: %s\n", aux->nome);
		return 1 * verificaSimbolosDeclarados(aux->s_declarados, aux->s_usados) * verificaSimbolosUsados(aux->s_declarados, aux->s_usados);
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

}