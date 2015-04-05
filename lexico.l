%{option main

 #include "bison.h"
 int contadorDeLinhas =1;
%}
/* TIPOS */
STRING [a-z]+[a-zA-Z0-9]*
SPACE [ \r\t]+
NUMBER [0-9]



%% /*REGRAS:*/
";"									return PVIRGULA;
","									return VIRGULA;
"espera"							return ESPERA;
"main"								return MAIN;
(f|F)+(uncao)+						return FUNCAO;
"declare"							return DECLARACAO;
"("									return LEFT_PAR;
")"									return RIGHT_PAR;
"{"									return ABRE_CHAVE;
"}"									return FECHA_CHAVE;
"return"							return RETURN;
"numero"							return NUMERICAL;
"texto"								return STR;
"#include"							return INCLUDE;
(\".*\")+							{yylval.strval = strdup(yytext); return TEXTO;}
(printf)	  						return PRINTF;
(scanf)		            			return SCANF;
(if)+                   		    			return IF;
(int|float|double|char)+ 		    			{yylval.strval = strdup(yytext); return TIPO;}
(%+(d|f|lf|c|s|u|i))					{yylval.strval = strdup(yytext); return VARUSE;}

"+"								return SOMA;
"-"								return SUBT;
"/"								return DIVIDE;
"="								return ATRIBUI;
"<"								return('<');
">"								return('>');
"==" 					    			return IGUAL;
"%"								return MOD;
"!"								return NOT;
"||" 					    			return OU;
"&&" 					    			return AND;
"&"									return ENDERECO;
"else" 				    			return ELSE;
"while"				    			return WHILE;
"do"			    			return DO;

{NUMBER}+                               			{yylval.strval = strdup(yytext); return T_NUMBER;}
{STRING}				    			{yylval.strval = strdup(yytext); return T_STRING;}
".h"									return PH;
{SPACE}					    			/*ignore*/


%%
