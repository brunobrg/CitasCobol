%{
#include <stdio.h>
#include "y.tab.h" 
%}

%%
    /* PALAVRAS */
\n                      /* ignora fim de linha */;
[ \t]+                  /* ignora espacos repetidos */;
[0-9]+   		return NUMERO;
[A-Za-z0-9]+            return STRING;
[\"STRING\"]            return QUOTE;

    /* PONTUACAO */
\(                      return LPARENTESE;
\)                      return RPARENTESE; 
\;                      return PONTO_VIRGULA;
\.                      return PONTO;

    /* ARITMETICA */
\<                      return MENOR;
\>                      return MAIOR;

%%

