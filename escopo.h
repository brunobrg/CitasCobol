/* ESCOPO:
   Aqui estao definidas as funcoes que criam e acessam a tabela 
   de variaveis.

 * initEscopo:
 * terminaEscopo:
 * valorSimbolo:
 * imprimeEscopos:
 * verificaListaEscopo:
 * saiEscopo:
 */

#ifndef ESCOPO_H_INCLUDED
#define ESCOPO_H_INCLUDED

#include "estruturaC.h"

/* prototipos */
void            initEscopo();
void            terminaEscopo();
void            adicionaSimbolo(Escopo *, char *, int, char *, char *, char *);
void            imprimeEscopos(ListaDeEscopo *);
int             verificaListaEscopo();
void            saiEscopo();
void			entraEscopo(struct _escopo *);
void 			entraEscopoNome(struct _escopo *, char *);
void   			atualizaValor(Escopo **, char* , char * );

#endif /* !ESCOPO_H_INCLUDED  */

