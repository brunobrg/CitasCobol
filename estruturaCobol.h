#ifndef ESTRUTURACOBOL_H_INCLUDED
#define ESTRUTURACOBOL_H_INCLUDED

/* STRUCTS */

typedef struct _TokenList
{
	/* 
    Lista de tokens. Cada token deve ter 30 posicoes,
                     exceto se estiver entre aspas.
    tklen: Token lenght (tamanho do token).
	*/
	char              * token;
	int                 tklen;
	struct _TokenList * proximo;
}TokenList;

typedef struct _Linha
{
	/* 
	numeracao: 1,2,3,....
	qntToks: quantidade de tokens na linha.
    marcador: '*' para linha de comentario;
              ' ' para linha de comando;
              '-' para continuacao de linha;  
    texto: lista de tokens.
	*/
    int         numeracao;
    int         qntToks;
    char        marcador;
	TokenList * texto;  
	
}Linha;

typedef struct _BlocoCobol
{
	/* 
    Lista duplamente encadeada de linhas.
    Pode ser uma division, uma section ou um paragrafo.
    */
	Linha              * linha;
	struct _BlocoCobol * anterior;
	struct _BlocoCobol * proximo;
}BlocoCobol;

typedef struct _SaidaCobol
{
	/*
	Arvore binaria composta por nos de blocosCobol.
	Cada bloco pode ser uma Division uma Section ou
	um Paragrafo.
	qteLinhas: quantidade de linhas no bloco.
	*/
	char               * identificacao;
	BlocoCobol         * conteudo;
	int                  qntLinhas;
	struct _SaidaCobol * lateral;
	struct _SaidaCobol * inferior;
}SaidaCobol;

#endif /* !ESTRUTURACOBOL_H_INCLUDED */