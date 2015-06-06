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
void            valorSimbolo(char *, char *, char *);
void            imprimeEscopos(ListaDeEscopo *);
int             verificaListaEscopo();
void            saiEscopo();


#endif /* !ESCOPO_H_INCLUDED  */

