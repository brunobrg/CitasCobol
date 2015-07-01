/* ESTRUTURA COBOL:
   Aqui estao as principais estruturas de saida (Saida Cobol).

 * TokenList: Lista de tokens. Cada token deve ter 30 posicoes,
   =========  exceto se estiver entre aspas.
 *** token:   Um token. 
 *** tklen:   Tamanho do token.
 *** proximo: Proximo token.

 * Linha: Uma linha no arquivo de saida.
   =====   
 *** numeracao: Numero da linha.
 *** qntToks:  Quantidade de tokens na linha.
 *** marcador: '*' para linha de comentario;
               ' ' para linha de comando;
               '-' para linha de continuacao;  
 *** texto:    TokenList com o conteudo da linha.

 * BlocoCobol: Lista duplamente encadeada de linhas.
   ==========  Pode ser uma division, uma section ou um paragrafo.
 *** linha:    Uma linha.
 *** anterior: Linha anterior.
 *** proximo:  Proxima linha.

 * SaidaCobol: Arvore binaria composta por nos de blocosCobol.
   ==========  Cada bloco pode ser uma Division, uma Section ou
	           um Paragrafo.
 *** identificacao: Nome do trecho de codigo no BlocoCobol interno.
 *** conteudo:      BlocoCobol com o conteudo do trecho de codigo.
 *** qteLinhas:     Quantidade de linhas no BlocoCobol.
 *** lateral:       Proximo BlocoCobol, de igual hierarquia.
 *** inferior:      BlocoCobol interno, de hierarquia inferior.
 */


#ifndef ESTRUTURACOBOL_H_INCLUDED
#define ESTRUTURACOBOL_H_INCLUDED

/* STRUCTS */

typedef struct _TokenList
{
	char              * token;
	int                 tklen;
	struct _TokenList * proximo;
}TokenList;

typedef struct _Linha
{
    int         numeracao;
    int         qntToks;
    char        marcador;
	TokenList * texto;  
	
}Linha;

typedef struct _BlocoCobol
{
	Linha              * linha;
	struct _BlocoCobol * anterior;
	struct _BlocoCobol * proximo;
}BlocoCobol;

typedef struct _SaidaCobol
{
	char               * identificacao;
	BlocoCobol         * conteudo;
	int                  qntLinhas;
	struct _SaidaCobol * lateral;
	struct _SaidaCobol * inferior;
}SaidaCobol;

typedef struct _SaidaErro
{
	char mensagem[256];
	struct _SaidaErro * proximo;
}SaidaErro;
typedef struct _SaidaWarning
{
	char mensagem[256];
	struct _SaidaWarning * proximo;;
}SaidaWarning;

#endif /* !ESTRUTURACOBOL_H_INCLUDED */