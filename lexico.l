%{option main

 #include "bison.h"
 int contadorDeLinhas =1;
%}
/* TIPOS */
STRING [a-z]+[a-zA-Z0-9]*
SPACE [ \r\t]+
NUMBER [0-9]



%% /*REGRAS:*/
"\n"								{contadorDeLinhas++;}
"execute"							return EXECUTE;
"passa"								return PASSA;
","								return VIRGULA;
"espera"							return ESPERA;
"senao se"							return ELSEIF;
"fimFunc"					{yyterminate(); return FIMFUNC;}
(f|F)+(uncao)+							return FUNCAO;
"declare"							return DECLARACAO;
"("								return LEFT_PAR;
")"								return RIGHT_PAR;
"numero"							return NUMERICAL;
"texto"								return STR;
(\".*\")+							{yylval.strval = strdup(yytext); return TEXTO;}
(imprima|imprimir|escreva|escrever)+    			return PRINT;
(ler|leia|receba|receber)+ 	            			return READ;
(se)+                   		    			return IF;
(declaracao|declare|declarar)+ 		    			return LOCAL;
"+"								return SOMA;
"-"								return SUBT;
"*"								return MULT;
"/"								return DIVIDE;
"="								return ATRIBUI;
(\>)+ 					    			return MAIOR;
(\<)+ 					    			return MENOR;
(\=)+ 					    			return IGUAL;
"!="								return NOT;
(ou)+ 					    			return OU;
(e)+ 					    			return AND;
(senao)+ 				    			return ELSE;
(enquanto)+ 				    			return WHILE;
(faca)+ 				    			return DO;
(entao)+ 		                    			return THEN;
(fim)+ 					    			return TERMINOU;
{NUMBER}+                               			{yylval.strval = strdup(yytext); return T_NUMBER;}
{STRING}				    			{yylval.strval = strdup(yytext); return T_STRING;}
{SPACE}					    			/*ignore*/



%%
