#ifndef TRADUCAO_H_INCLUDED
#define TRADUCAO_H_INCLUDED

#include "estruturaCobol.h"

/* prototipos */
char            * nomeProgramaCob(char *);
void              criarDivisions();
void              criarIdDivision(SaidaCobol **, char *);
/*
void              initProcDivision();
void 		      initDataDivision();
void              fechaMain();
*/
void              inserirSaida(SaidaCobol **, Linha *);
void              pularLinha(SaidaCobol **);
Linha           * criarLinhaA();
Linha           * criarLinhaB();
Linha           * criarComentario();
void              inserirToken(Linha **, char *);
void              organizarSaida(SaidaCobol **);
/*
void              limparPrintBuff();
*/
char            * imprimirTL(TokenList *);
void              escreverArquivo(SaidaCobol *, char *);
/*
void		      escreverErro(char *); 
*/

#endif /* !TRADUCAO_H_INCLUDED */