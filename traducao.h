#ifndef TRADUCAO_H_INCLUDED
#define TRADUCAO_H_INCLUDED

#include "estruturaCobol.h"

/* prototipos */
char            * nomeProgramaCob(char *);
void              criarDivisions(SaidaCobol **);
void              escreverIdntfDivision(BlocoCobol **, char *);
/*
void              initProcDivision();
void 		      initDataDivision();
void              fechaMain();
*/
void              organizarSaida(SaidaCobol **);
/*
void              limparPrintBuff();
*/
void              escreverArquivo(FILE *, SaidaCobol *);
/*
void		      escreverErro(char *); 
*/

#endif /* !TRADUCAO_H_INCLUDED */