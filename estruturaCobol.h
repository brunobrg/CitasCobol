#ifndef ESTRUTURACOBOL_H_INCLUDED
#define ESTRUTURACOBOL_H_INCLUDED

/* structs */

typedef struct _TokenList
{
	/* 
    Lista de tokens. Cada token deve ter 30 posicoes,
                     exceto se estiver entre aspas.
	*/
	char              * token;
	struct _TokenList * proximo;
}TokenList;

typedef struct _Linha
{
	/* 
	numeracao: 1,2,3,....
    marcador: '*' para linha de comentario;
              ' ' para linha de comando;
              '-' para continuacao de linha;  
    texto: lista de tokens.
	*/
    int         numeracao;
    char        marcador;
	TokenList * texto;  
	
}Linha;

typedef struct _SaidaCobol
{
	/* 
    Lista duplamente encadeada de linhas.
    Forma o texto do arquivo de saida.
    */
	Linha              * linha;
	struct _SaidaCobol * anterior;
	struct _SaidaCobol * proximo;
}SaidaCobol;

typedef struct _HierarquiaCobol
{
	/*
	Arvore binaria composta por:
	  Divisions
	  Sections
	  Paragrafos
	*/
	char                    * identificacao;
	SaidaCobol              * conteudo;
	struct _HierarquiaCobol * lateral;
	struct _HierarquiaCobol * inferior;
}HierarquiaCobol;

#endif /* !ESTRUTURACOBOL_H_INCLUDED */