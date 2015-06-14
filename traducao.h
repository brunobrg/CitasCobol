/* TRADUCAO:
   Aqui estao definidas as funcoes que criam a estrutura do arquivo de 
   saida (saidaCobol) e escrevem no arquivo todo o seu conteudo.

 * nomeProgramaCob: Recebe o nome do programa.c e devolve o nome do 
       programa .cob 
 * inserirToken: Isere um token ao final de uma linha.
 * inserirToksGrandes: Isere tokens que fazem parte de um quote ou 
   comentario e tem mais do que 30 caracteres.
 * criarLinhaA: Retorna uma linha com inicio na coluna A.
 * criarLinhaB: Retorna uma linha com inicio na coluna B.
 * criarComentario: Retorna uma linha de comentario vazia.
 * criarContinuacao: Retorna uma linha de continuacao.
 * linhaVazia: Retorna uma linha em branco.
 * inserirIdntDiv: Insere linha ao final da Identification Division.
 * inserirDataDiv: Insere linha ao final da Data Division.
 * inserirProcDiv: Insere linha ao final da Procedure Division.
 * criarDivisions: Cria o tronco principal da arvore saidaCobol 
       com as 4 divisions.
 * organizarSaida: Organiza uma saidaCobol, quebrando as linhas grandes 
       e numerando as linhas.
 * escreverArquivo: Imprime o conteudo de uma saidaCobol no arquivo 
     de saida.
 * hasBlankSpace: Verifica se uma string tem espaco em branco
 */

#ifndef TRADUCAO_H_INCLUDED
#define TRADUCAO_H_INCLUDED

#include "estruturaCobol.h"
#include <stdbool.h>

/* prototipos */
char            * nomeProgramaCob(char *);
void              inserirToken(Linha **, char *);
void              inserirToksGrandes(Linha **, char *, int);
Linha           * criarLinhaA();
Linha           * criarLinhaB();
Linha           * criarComentario();
Linha           * criarContinuacao();
Linha           * linhaVazia();
void              inserirIdntDiv(SaidaCobol **, Linha *);
void              inserirDataDiv(SaidaCobol **, Linha *);
void              inserirProcDiv(SaidaCobol **, Linha *);
void              criarDivisions(SaidaCobol **);
void              organizarSaida(SaidaCobol **);
void              escreverArquivo(FILE *, SaidaCobol *);

#endif /* !TRADUCAO_H_INCLUDED */