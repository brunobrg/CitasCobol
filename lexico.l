%{option main

 #include "bison.h"
 int contLinhasC =1;

%}

/* TIPOS */
L           [a-zA-Z_]
WORD		[a-zA-Z_][a-zA-Z0-9_]*
SPACE		[ \r\t]+
DIGIT		[0-9]

ASPAS [\"]

%% /*REGRAS:*/
"\n"							{contLinhasC++;}
";"								return PVIRGULA;
","								return VIRGULA;
\"								return ASPAS;	
"("								return LEFT_PAR;
")"								return RIGHT_PAR;
"{"								return ABRE_CHAVE;
"}"								return FECHA_CHAVE;
"+"								return SOMA;
"-"								return SUBT;
"/"								return DIVIDE;
"="								return ATRIBUI;
"<"								return ('<');
">"								return ('>');
"==" 							return IGUAL;
"%"								return MOD;
"!"								return NOT;
"||" 							return OU;
"&&" 							return AND;
"&"								return ENDERECO;
"main"							{yylval.strval = strdup(yytext); return MAIN;}
"return"						return RETURN;
"#include"						return INCLUDE;
(printf)	  					return PRINTF;
(scanf)		            		return SCANF;
(int|float|double|char)+ 		{yylval.strval = strdup(yytext); return TIPO;}
(%+(d|f|lf|c|s|u|i))			{yylval.strval = strdup(yytext); return VARUSE;}
(if)	                   		return IF;
"else" 				    		return ELSE;
"while"				    		return WHILE;
"do"			    			return DO;

L?\"(\\.|[^\\"])*\"				{yylval.strval = strdup(yytext); return TEXTO;}
{DIGIT}+						{yylval.strval = strdup(yytext); return NUMBER;}
{WORD}							{yylval.strval = strdup(yytext); return PALAVRA;}
".h"							return PH;
{SPACE}							/*ignore*/


%%
