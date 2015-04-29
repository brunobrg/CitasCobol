typedef struct _escopo
{
	char nome[40];
	int id;
	struct _s_declarados * s_declarados;
	struct _s_usados * s_usados;
	struct _escopo * proximo;
	struct _escopo * anterior;
}Escopo;

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
void entraEscopo(char[40]);

int idEscopo;
Escopo * escopo;
Escopo * escopoAtual;


void entraEscopo(char name[40])
{
	
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
		add->proximo = NULL;
		add->anterior = atual;
	}
	else
	{
		strcpy(add->nome, newName);
		add->id = idEscopo;
		add->s_declarados = NULL;
		add->s_usados = NULL;
		add->proximo = NULL;
		add->anterior = NULL;
	}

	return add;
}

void criaEscopo(char newName[40])
{
	Escopo * aux = escopo;
	Escopo * add = addEscopo( newName, escopoAtual);

	idEscopo+=1;

	if(aux == NULL)
		aux = add;
	else
	{
		while(aux->proximo != NULL)
			aux = aux->proximo;

		aux = add;
	}
}

void initEscopo()
{
	escopo = addEscopo("global", NULL);

}