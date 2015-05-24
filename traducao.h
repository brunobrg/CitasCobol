#ifndef TRADUCAO_H_INCLUDED
#define TRADUCAO_H_INCLUDED

#include "estruturaCobol.h"

/* prototipos */
char            * nomeProgramaCob(char *);
Linha           * criarLinhaB();
void              inserirIdntDiv(SaidaCobol **, Linha *);
void              inserirDataDiv(SaidaCobol **, Linha *);
void              inserirProcDiv(SaidaCobol **, Linha *);
void              criarDivisions(SaidaCobol **);
void              escreverIdntfDivision(SaidaCobol **, char *);
void 		      escreverDataDivision(SaidaCobol **);
void              escreverProcDivision(SaidaCobol **);
void              fechaMainSection(SaidaCobol **);
void              organizarSaida(SaidaCobol **);
void              escreverArquivo(FILE *, SaidaCobol *);

#endif /* !TRADUCAO_H_INCLUDED */