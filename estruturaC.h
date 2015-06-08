/* ESTRUTURA C:
   Aqui estao as principais estruturas de entrada.

 * Escopo: 
   ======  
 *** nome:  
 *** id: 
 *** s_declarados:
 *** s_usados:
 *** anterior:


 * ListaDeEscopo:
   =============   
 *** proximo: 
 *** escopo:


 * Simbolos: 
   ========  
 *** tipo:  
 *** nome: 
 *** value:
 *** proximo: 

 */

#ifndef ESTRUTURAC_H_INCLUDED
#define ESTRUTURAC_H_INCLUDED

/* STRUCTS */
 
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
	int linha;
	char tipo[6];
	char nome[30];
	char * value;
	struct _simbolos * proximo; 
}Simbolos;

#endif /* !ESTRUTURAC_H_INCLUDED */