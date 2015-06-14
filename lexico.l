%{option main
 #include "bison.h"
 extern int contLinhasC;   /* Contador de linha arq de entrada */ 
 extern int contColunaC;   /* Contador de coluna arq de entrada */ 

 void contaColn();
%}

/* TIPOS */
D		                    [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*
WORD                    {L}({L}|{D})*
FILENAME                {WORD}("\."{WORD})?

%% /*REGRAS:*/

"/*"(.|\n)*"*/"         { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(COMENT);
                        }
"//"(.)*"\n"            { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(COMENT);
                        }
"char"                  { contaColn(); 
                          yylval.strval = "CHAR"; 
                          return(TYPE); }
"do"                    { contaColn(); return(DO); }
"else"                  { contaColn(); return(ELSE); }
"float"                 { contaColn(); 
                          yylval.strval = "FLOAT"; 
                          return(TYPE); }
"if"                    { contaColn(); return(IF); }
"int"                   { contaColn(); 
                          yylval.strval = "INT"; 
                          return(TYPE); }
"printf"                { contaColn();
                          yylval.strval = strdup(yytext);
                          return(PRINTF); }
"return"                { contaColn(); return(RETURN); }
"while"                 { contaColn(); return(WHILE); }

(%+(d|f|lf|c|s|u|i))    { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(VARUSE); }

"\n"                    { contColunaC = 1;
                          contLinhasC++; }

{WORD}                  { contaColn(); 
                          yylval.strval = strdup(yytext); 
                          return(WORD); }
0[xX]{H}+{IS}?          { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }
0{D}+{IS}?              { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }
{D}+{IS}?               { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }
L?'(\\.|[^\\'])+'       { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }

{D}+{E}{FS}?            { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }
{D}*"."{D}+({E})?{FS}?  { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }
{D}+"."{D}*({E})?{FS}?  { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(NUMBER); }

L?\"(\\.|[^\\"])*\"     { contaColn(); 
	                        yylval.strval = strdup(yytext); 
                          return(QUOTE); }

("#")[ \t]*("include")[ \t]*"\""{FILENAME}"\""  { 
                          contaColn(); 
                          yylval.strval = strdup(yytext); 
                          return(INCLUDE); }   
("#")[ \t]*("include")[ \t]*"<"{FILENAME}">"    {
                          contaColn(); 
                          yylval.strval = strdup(yytext); 
                          return(INCLUDE); }                                                
("#")[ \t]*("define")[ \t]*{WORD}               { 
                          contaColn(); 
                          yylval.strval = strdup(yytext); 
                          return(DEFINE); } 
                          
"&&"                    { contaColn(); return(AND); }
"||"                    { contaColn(); return(OR); }
"<="                    { contaColn(); return(LE); }
">="                    { contaColn(); return(GE); }
"=="                    { contaColn(); return(EQUAL); }
"!="                    { contaColn(); return(NEQUAL); }
";"                     { contaColn(); return(';'); }
("{"|"<%")              { contaColn(); return('{'); }
("}"|"%>")              { contaColn(); return('}'); }
","                     { contaColn(); return(','); }
":"                     { contaColn(); return(':'); }
"="                     { contaColn(); return('='); }
"("                     { contaColn(); return('('); }
")"                     { contaColn(); return(')'); }
("["|"<:")              { contaColn(); return('['); }
("]"|":>")              { contaColn(); return(']'); }
"."                     { contaColn(); return('.'); }
"&"                     { contaColn(); return('&'); }
"!"                     { contaColn(); return('!'); }
"~"                     { contaColn(); return('~'); }
"-"                     { contaColn(); return('-'); }
"+"                     { contaColn(); return('+'); }
"*"                     { contaColn(); return('*'); }
"/"                     { contaColn(); return('/'); }
"%"                     { contaColn(); return('%'); }
"<"                     { contaColn(); return('<'); }
">"                     { contaColn(); return('>'); }
"^"                     { contaColn(); return('^'); }
"|"                     { contaColn(); return('|'); }
"?"                     { contaColn(); return('?'); }

[ \t\v\f]               { contaColn(); /*ignore*/ }

%%

int yywrap(void)
{
  return 1; 
}

void contaColn() 
{
	/* implementar */
}
