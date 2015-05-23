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

typedef struct _BlocoCobol
{
	/* 
    Lista duplamente encadeada de linhas.
    Forma o texto do arquivo de saida.
    */
	Linha              * linha;
	struct _BlocoCobol * anterior;
	struct _BlocoCobol * proximo;
}BlocoCobol;

typedef struct _SaidaCobol
{
	/*
	Arvore binaria composta por nos de blocosCobol.
	Cada bloco pode ser:
	  uma Division;
	  uma Section; ou
	  um Paragrafo.
	*/
	char               * identificacao;
	BlocoCobol         * conteudo;
	struct _SaidaCobol * lateral;
	struct _SaidaCobol * inferior;
}SaidaCobol;

#endif /* !ESTRUTURACOBOL_H_INCLUDED */