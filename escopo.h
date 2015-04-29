typedef struct _escopo
{
	char nome[40];
	int id;
	struct _s_declarados * s_declarados;
	struct _s_usados * s_usados;
	struct _escopo * anterior;
}Escopo;

typedef struct _listaDeEscopo
{
	struct _listaDeEscopo * proximo;
	struct _escopo * escopo;
}ListaDeEscopo;

typedef struct _s_usados
{
	char tipo[6];
	char nome[15];
	char escopo[30];
	struct _s_usados * proximo; 
}S_usados;

typedef struct s_declarados
{
	char tipo[6];
	char nome[15];
	char escopo[30];
	struct _s_declarados * proximo; 
}S_declarados;



void initEscopo();
Escopo * addEscopo(char[40], Escopo *);
void criaEscopo(char[40]);
void entraEscopo(Escopo *);
ListaDeEscopo * addLista();
void imprimeEscopos(ListaDeEscopo *);



int idEscopo;
Escopo * escopo;
Escopo * escopoAtual;
ListaDeEscopo * listaDeEscopo;

void entraEscopo(Escopo * temp)
{
	escopoAtual = temp;	
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
		add->s_usados = NULL;
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
		Escopo * add = addEscopo( newName, escopoAtual);
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

void initEscopo()
{
	listaDeEscopo = NULL;
	criaEscopo("global");

}

