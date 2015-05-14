#ifndef ESTRUTURAC_H_INCLUDED
#define ESTRUTURAC_H_INCLUDED

/* structs */
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
	char * value;
	struct _simbolos * proximo; 
}Simbolos;

#endif /* !ESTRUTURAC_H_INCLUDED */