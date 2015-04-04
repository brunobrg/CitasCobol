%{
#include <stdio.h>
#include <string.h>
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
main()
{
        yyparse();
} 

%}

/* TOKENS */

%token NUMERO STRING QUOTE
%token LPARENTESE RPARENTESE PONTO_VIRGULA PONTO
%token MENOR MAIOR

/* GRAMATICA */

%%

linhas_comando: /* vazio */
        | linhas_comando linha_comando PONTO_VIRGULA
        ;

linha_comando: 
         display
        ;

display: 
         STRING LPARENTESE QUOTE RPARENTESE
        {
            printf("\tDISPLAY \"textoooo\" \n");
        }
        ;

%%