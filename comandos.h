#ifndef COMANDOS_H_INCLUDED
#define COMANDOS_H_INCLUDED

#include "estruturaCobol.h"

/* prototipos */
void   abreMain(SaidaCobol **);
void   fechaMain(SaidaCobol **);
void   imprimir(SaidaCobol **, char *, Linha **);
void   limparPrintBuff(SaidaCobol **, Linha **); 

#endif /* !COMANDOS_H_INCLUDED  */
 