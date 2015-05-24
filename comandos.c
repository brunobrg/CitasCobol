#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estruturaCobol.h"
#include "traducao.h"

/* implementacao */

void imprimir(SaidaCobol ** saidaCobol, char * str, Linha ** printbuff)
{
    // erro se o printf for vazio
	if(strlen(str)<3)
       yyerror(3);

    // busca linha no buffer
	Linha * linha;
	char * token;

	if(*printbuff == NULL)
    {
        linha = criarLinhaB();
        inserirToken(&linha, "DISPLAY");
    }
    else
    {
      	linha = *printbuff;
      	*printbuff = NULL;
    }

	// remove aspas
	str = str + 1;
	str[strlen(str)-1] = '\0';

	// quebra linhas por \n
	char * novaLinha = strsep (&str, "\\");

    while(novaLinha != NULL)
    {
    	token = (char *) malloc(strlen(novaLinha)*sizeof(char));
    	if(str == NULL)
    	{
            sprintf(token,"\"%s\"", novaLinha);
    		inserirToken(&linha, token);
            *printbuff = linha;
	        novaLinha = NULL;
    	}
    	else if (str[0] == 'n')
        {
        	sprintf(token,"\"%s\"", novaLinha);
		    inserirToken(&linha, token);
		    inserirProcDiv(saidaCobol,linha);
		    linha = NULL;
            *printbuff = NULL;
            if (strlen(str) > 1)
            {
                novaLinha = strsep (&str, "\\");
                novaLinha = novaLinha + 1;
            }
            else
            {
            	novaLinha = NULL;
            }
        }
        else
        {
    	    sprintf(novaLinha,"%s\\%s",novaLinha,str);
    	    strsep (&str, "\\");
        }
	    if (linha == NULL)
        {
    	    linha = criarLinhaB();
            inserirToken(&linha, "DISPLAY");
        }
	}

}

void limparPrintBuff(SaidaCobol ** saidaCobol, Linha * printbuff)
{
 
	if ( printbuff != NULL )
	{
		inserirProcDiv(saidaCobol,printbuff);
	}
}
