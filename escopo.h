typedef struct _escopo
{
	char nome[40];
	int id;
	struct _simbolos * s_declarados;
	struct _simbolos * s_usados;
	struct _escopo * anterior;
}Escopo;

typedef struct _listaDeEscopo
{
	struct _listaDeEscopo * proximo;
	struct _escopo * escopo;
}ListaDeEscopo;

typedef struct _simbolos
{
	char tipo[6];
	char nome[15];
	struct _simbolos * proximo; 
}Simbolos;



void initEscopo();
Escopo * addEscopo(char[40], Escopo *);
void criaEscopo(char[40]);
void saiEscopo();
void entraEscopo(Escopo *);
ListaDeEscopo * addLista();
void imprimeEscopos(ListaDeEscopo *);

//Simbolos
void adicionaSimbolo(Escopo * , char[40] , char[40], char[40]);



int idEscopo;
Escopo * escopo;
Escopo * escopoAtual;
ListaDeEscopo * listaDeEscopo;

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
	listaDeEscopo = NULL;
	criaEscopo("global");

}


Simbolos * addSimbolo(char tipo[40], char nome[40])
{
	Simbolos * add = (Simbolos *) malloc(sizeof(Simbolos));
	strcpy(add->tipo, tipo);
	strcpy(add->nome, nome);
	add->proximo = NULL;
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

		}
		else if(!strcmp(pos, "usada"))
		{

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
