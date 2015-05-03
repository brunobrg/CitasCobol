#include "traducao.h"

/* variaveis globais */

/* prototipos */
void imprimir(char *);
void guardaBuffer(Linha *);
void limparPrintBuff();

/* implementacao */
void imprimir(char * str)
{
    // erro se o printf for vazio
	if(strcmp(str,"\"\"") == 0)
	{
		char erro[] = "Zero-length printf";
		yyerror(erro);
	}

    // busca linha no buffer
	Linha * linha;
	char * token;

	if(printbuff == NULL)
    {
        linha = criarLinhaB();
        inserirToken(&linha, "DISPLAY");
    }
    else
    {
      	linha = printbuff;
      	printbuff = NULL;
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
    		guardaBuffer(linha);
	        novaLinha = NULL;
    	}
    	else if (str[0] == 'n')
        {
        	sprintf(token,"\"%s\"", novaLinha);
		    inserirToken(&linha, token);
		    inserirSaida(linha);
		    linha = NULL;
            printbuff = NULL;
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

void guardaBuffer(Linha * linha)
{
    printbuff = linha;
}

void limparPrintBuff()
{
 
	if (printbuff != NULL)
	{
		inserirSaida(printbuff);
	}
}