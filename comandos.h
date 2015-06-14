/* COMANDOS:
   Aqui estao definidas as funcoes que traduzem para Cobol os comandos 
   dados pelo arquivo de entrada.c.

 * limparPrintBuff: Cria um DISPLAY com o string armazenado no buffer.
 * escreverIdntfDivision: Escreve a Identification Division.
 * escreverDataDivision: Escreve a Data Division.
 * escreverProcDivision: Escreve o cabecalho da Procedure Division.
 * abreSection: Escreve o cabecalho de uma Section.
 * fechaSection: Escreve as ultimas linhas de uma Section.
 * comentario: Cria linhas de comentario.
 * imprimir: Cria um DISPLAY para os printf terminados em \n.
       Os printfs nao terminados em \n sao armazenados no buffer para 
       ser impressos futuramente.
 * inclui_includeStdio: Armazena um valor booleano que informa se o #include<stdio.h> foi adicionado.
 * verifica_includeSt: verifica se o #include<stdio.h> foi adicionado ou nao.
 */

#ifndef COMANDOS_H_INCLUDED
#define COMANDOS_H_INCLUDED

#include "estruturaCobol.h"

/* prototipos */
void   limparPrintBuff(SaidaCobol **); 
void   escreverIdntfDivision(SaidaCobol **, char *);
void   escreverDataDivision(SaidaCobol **);
void   escreverProcDivision(SaidaCobol **);
void   abreSection(SaidaCobol **, char *);
void   fechaSection(SaidaCobol **, char *);
void   comentario(SaidaCobol **, char *);
void   imprimir(SaidaCobol **, char *, Linha **);
void   inclui(char *);
bool   verifica_includeStdio();

#endif /* !COMANDOS_H_INCLUDED  */
 