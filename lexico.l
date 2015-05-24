%{option main
 #include "bison.h"
 extern int contLinhasC;

 void contaColn();
%}

/* TIPOS */
D		                    [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*

%% /*REGRAS:*/
"/*"                    { comentario(); }
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
"main"                  { contaColn(); 
                          yylval.strval = strdup(yytext); 
                          return(MAIN); }
"printf"                { contaColn(); return(PRINTF); }
"return"                { contaColn(); return(RETURN); }
"while"                 { contaColn(); return(WHILE); }
"#include"              { contaColn(); return(INCLUDE); }
(%+(d|f|lf|c|s|u|i))    { contaColn(); 
                          yylval.strval = strdup(yytext);
                          return(VARUSE); }
".h"                    { contaColn(); return PH; }
"\n"                    { contaColn(); contLinhasC++; }

{L}({L}|{D})*		        { contaColn(); 
                          yylval.strval = strdup(yytext); 
                          return(WORD); }
 
0[xX]{H}+{IS}?          { contaColn(); return(NUMBER); }
0{D}+{IS}?              { contaColn(); return(NUMBER); }
{D}+{IS}?               { contaColn(); return(NUMBER); }
L?'(\\.|[^\\'])+'       { contaColn(); return(NUMBER); }

{D}+{E}{FS}?            { contaColn(); return(NUMBER); }
{D}*"."{D}+({E})?{FS}?  { contaColn(); return(NUMBER); }
{D}+"."{D}*({E})?{FS}?  { contaColn(); return(NUMBER); }

L?\"(\\.|[^\\"])*\"     { contaColn(); 
	                        yylval.strval = strdup(yytext); 
                          return(QUOTE); }

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

[ \t\v\n\f]             { contaColn(); /*ignore*/ }

%%

int yywrap(void)
{
  return 1; 
}

comentario()
{
    /* implementar */
}

int coluna = 0;

void contaColn() 
{
	/* implementar */
}