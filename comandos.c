#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estruturaCobol.h"
#include "escopo.h"
#include "traducao.h"

/* VARIAVEIS GLOBAIS */
Linha            * printbuff = NULL; /* Buffer de string a imprimir */
extern Simbolos  * listaDeVariaveis;
int                include_stdio = 0;     

/* IMPLEMENTACAO */

/* Cria um DISPLAY com o string armazenado no buffer. */
void limparPrintBuff(SaidaCobol ** saidaCobol)
 {
    if ( printbuff != NULL )
    {
        inserirProcDiv(saidaCobol,printbuff);
    }
}

/* Escreve a Identification Division */
void escreverIdntDivision(SaidaCobol ** saidaCobol, char * arqCob)
{

    // Coloca virgula
    char * arq_virgula = (char *) 
        malloc((strlen(arqCob)+1)*sizeof(char));
    strcpy(arq_virgula, arqCob);
    strcat(arq_virgula, ",");

    // Cria primera linha
    Linha * linha = criarComentario();
    inserirToken(&linha, arq_virgula);
    inserirToken(&linha, " criado por C it as Cobol v0.2");
    inserirIdntDiv(saidaCobol,linha);
    
    // Cria proximas linhas 
    Linha * linha2 = criarLinhaA();
    inserirToken(&linha2, "IDENTIFICATION DIVISION");
    inserirIdntDiv(saidaCobol,linha2);

    char nomeProg[strlen(arqCob)];
    strcpy(nomeProg,arqCob);

    int i;
    for(i = 2; nomeProg[i] != '.' ;i++);
        nomeProg[i] = '\0';

    Linha * linha3 = criarLinhaA();
    inserirToken(&linha3, "PROGRAM-ID.");
    inserirToken(&linha3, strdup(nomeProg));
    inserirIdntDiv(saidaCobol,linha3); 

    Linha * linhaV = linhaVazia();
    inserirIdntDiv(saidaCobol,linhaV);

}

/* Escreve a Data Division. */
void escreverDataDivision(SaidaCobol ** saidaCobol)
{
    if(listaDeVariaveis != NULL)
    {
        Linha * linha = criarLinhaA();
        inserirToken(&linha, "DATA DIVISION");
        inserirDataDiv(saidaCobol,linha);

        linha = criarLinhaA();
        inserirToken(&linha, "WORKING-STORAGE SECTION");
        inserirDataDiv(saidaCobol,linha);

        Simbolos * aux = listaDeVariaveis;
        while(aux != NULL)
        {
            linha = criarLinhaB();
            if(strcmp(aux->tipo, "INT") == 0)
            {
                inserirToken(&linha, "01");
                inserirToken(&linha, aux->nome);
                inserirToken(&linha, "PIC S9(05)");
            }
            else if(strcmp(aux->tipo, "FLOAT") == 0)
            {
                inserirToken(&linha, "01");
                inserirToken(&linha, aux->nome);
                inserirToken(&linha, "PIC S9(18)V9(18)");
            }
            else if(strcmp(aux->tipo, "CHAR") == 0)
            {
                inserirToken(&linha, "01");
                inserirToken(&linha, aux->nome);
                inserirToken(&linha, "PIC X(1)");
            } 
            else
            {
                erro(5);
            }
            
            if(aux->value != NULL)
            {
                inserirToken(&linha, "VALUE");
                inserirToken(&linha, aux->value);
            } 

            inserirDataDiv(saidaCobol,linha); 

            aux = aux->proximo; 
        }
        linha = linhaVazia();
        inserirDataDiv(saidaCobol,linha);

    }

}

/* Escreve o cabecalho da Procedure Division. */
void escreverProcDivision(SaidaCobol ** saidaCobol)
{

    Linha * linha = criarLinhaA();
    inserirToken(&linha,"PROCEDURE DIVISION");
    inserirProcDiv(saidaCobol,linha);

    Linha * linhaV = linhaVazia();
    inserirProcDiv(saidaCobol,linhaV);

    Linha * linha2 = criarLinhaA();
    inserirToken(&linha2,"000000-MAIN");
    inserirToken(&linha2,"SECTION");
    inserirProcDiv(saidaCobol,linha2);

}

/* Finaliza a 000000-MAIN SECTION */
void fechaMainSection(SaidaCobol ** saidaCobol)
{

    limparPrintBuff(saidaCobol);

    Linha * linhaV = linhaVazia();
    inserirProcDiv(saidaCobol,linhaV);

    Linha * linha = criarLinhaB();
    inserirToken(&linha,"STOP RUN");
    inserirProcDiv(saidaCobol,linha);

    //Linha * linha2 = criarLinhaA();
    //inserirToken(&linha2,"000000-EXIT");
    //inserirProcDiv(saidaCobol,linha2);

    Linha * linha3 = criarLinhaA();
    inserirToken(&linha3,"EXIT");
    inserirProcDiv(saidaCobol,linha3);

}

/* Escreve o cabecalho de uma Section. */
void abreSection(SaidaCobol ** saidaCobol, char * nome)
{
   
    if(strcmp(nome,"main") == 0)
        escreverProcDivision(saidaCobol);

}

/* Escreve as ultimas linhas de uma Section. */
void fechaSection(SaidaCobol ** saidaCobol, char * nome)
{

    if(strcmp(nome,"main") == 0)
        fechaMainSection(saidaCobol);

}

/* comentario: Cria linhas de comentario */
void comentario(SaidaCobol ** saidaCobol, char * coment)
{
    char segundochar = coment[1];
    coment = coment + 2;     // remove // ou /* 

    /* Comentario entre // ... \n */
    if(segundochar == '/') 
    {    
        Linha * linha = criarComentario();
        coment[strlen(coment)-1] = '\0';       // remove \n
        inserirToksGrandes(&linha,coment,32);
        inserirProcDiv(saidaCobol,linha);
    }
    /* Comentario entre / * ... * / */
    else                  
    {
       
       coment[strlen(coment)-2] = '\0';       // remove */
       char * quebraTk = (char *) malloc((strlen(coment)+2)*sizeof(char));
       while(quebraTk = strsep (&coment,"\n"))
       {
           Linha * linha = criarComentario();
           inserirToksGrandes(&linha,quebraTk,32);
           inserirProcDiv(saidaCobol,linha);
       }
    }

   Linha * linha = linhaVazia();
   inserirProcDiv(saidaCobol,linha);

}

/* Cria um DISPLAY para os printf terminados em \n.
   Os printfs nao terminados em \n sao armazenados no buffer para 
   ser impressos futuramente.*/
void imprimir(SaidaCobol ** saidaCobol, char * str)
{
    // erro se o printf for vazio
	if(strlen(str)<3)
        erro(3);

    // erro se nao houver o include do stdio.h
    if(include_stdio == 0)
        erro(6);

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
    	token = (char *) malloc(strlen(novaLinha)*sizeof(char)+3);
    	if(str == NULL)
    	{
            sprintf(token,"\"%s\"", novaLinha);
    		inserirToken(&linha, token);
            printbuff = linha;
	        novaLinha = NULL;
    	}
    	else if (str[0] == 'n')
        {
        	sprintf(token,"\"%s\"", novaLinha);
		    inserirToken(&linha, token);
		    inserirProcDiv(saidaCobol,linha);
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

void imprimirComArgs(SaidaCobol ** saidaCobol, char * str, char * simbolos)
{

    if(strlen(str)<3)
        erro(3);

    // erro se nao houver o include do stdio.h
    if(include_stdio == 0)
        erro(6);

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
      token = (char *) malloc(strlen(novaLinha)*sizeof(char)+3);
      if(str == NULL)
      {
            sprintf(token,"\"%s\"", novaLinha);
        inserirToken(&linha, token);
            printbuff = linha;
          novaLinha = NULL;
      }
      else if(str[0] == '%')
      { 
          printf("entrou aqui");
      }
      else if (str[0] == 'n')
      {
        sprintf(token,"\"%s\"", novaLinha);
        inserirToken(&linha, token);
        inserirProcDiv(saidaCobol,linha);
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


/* Armazena includes */
void inclui(char * include){

    while (include[0] != '<' && include[0] != '\"')
      { include = include + 1;
      }

    char cfim;
    if (include[0] == '<')
    {   
        cfim = '>';
    } else
    {
        cfim = '\"';
    }

    include = include + 1; 
    include[strlen(include)-1] = '\0'; 

    if (!strcmp(include,"stdio.h") == true)
        include_stdio = 1;
}

/* Armazena defines */
void define(char * define, char * valor)
{
    /* implementar */
}

atribuiValor(SaidaCobol ** saidaCobol, Escopo * escopoAtual, 
    int contLinhasC, char * simbolo, char * valor)
{
      Linha * linha = criarLinhaB();

      char * mais = strchr(valor,'+');
      char * menos = strchr(valor,'-');
      char * neg = strchr(valor,'n');
      char * vezes = strchr(valor,'*');
      char * divide = strchr(valor,'/');

      if(mais == NULL && vezes == NULL 
         && divide == NULL && menos == NULL)
      {
          char valor2[strlen(valor)];
          valor2[0] = '\0';

          while(valor[0] == '(' || valor[0] == 'n'  )
          {
              if(valor[0] == '(')
              {
                  valor[strlen(valor)-1] = '\0';
              } else if(valor[0] == 'n')
              {
                  strcat(valor2,"-");
              } 
              valor = valor + 1;

          }
          strcat(valor2,valor);
 
          inserirToken(&linha, "MOVE");
          inserirToken(&linha, valor2);
          inserirToken(&linha, "TO");
          inserirToken(&linha, simbolo);
          inserirProcDiv(saidaCobol, linha);
      }
      else
      {

          while(neg != NULL)
          {
              *neg = '-';
              neg = strchr(valor,'n');
          }
          inserirToken(&linha, "COMPUTE");
          inserirToken(&linha, simbolo);
          inserirToken(&linha, "=");
          inserirToken(&linha, valor);
          inserirProcDiv(saidaCobol, linha);
      }

      adicionaSimbolo(escopoAtual, "usada", contLinhasC, "null", simbolo, valor);
      //atualizaValor(&escopoAtual, simbolo, valor);
}


atribuiString(SaidaCobol ** saidaCobol, Escopo * escopoAtual, 
    int contLinhasC, char * simbolo, char * string)
{
    Linha * linha = criarLinhaB();
    inserirToken(&linha, "MOVE");
    inserirToken(&linha, string);
    inserirToken(&linha, "TO");
    inserirToken(&linha, simbolo);
    inserirProcDiv(saidaCobol, linha);
    adicionaSimbolo(escopoAtual, "usada", contLinhasC, "null", simbolo, string);
}