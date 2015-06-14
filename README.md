# CCOBOL: C IT AS COBOL

Projeto da disciplina de Fundamento de Compiladores da Universidade de Brasilia - Campus Gama

## Objetivos

Desenvolver o compilador "C it as Cobol" (ccobol) que receba um código em linguagem C e traduza para um código em linguagem Cobol. Este é um compilador com caráter educacional, que objetiva ser uma ferramenta de aprendizado para o programador C que deseja se aventurar na programação Cobol.

Para que o código C de entrada seja considerado válido e aceito pelo compilador, é necessário, ainda que não suficiente, que ele seja compilável pelo gcc. Alguns códigos em C, contudo, serão compiláveis pelo gcc, mas não serão compiláveis pelo ccobol, por possuírem linhas de comando não diretamente traduzíveis, ou por possuírem comportamentos não previstos.

O ccobol, ao receber um código em C válido, dentro de seu escopo, deve gerar um código em Cobol que:
* Seja compilável pelo gnuCobol;
* Seja preferencialmente executável pelo gnuCobol;
* Preze pela clareza do código.
* Ao ser compilado, gere um executável cujos outputs sejam os mesmos outputs do executável gerado pelo gcc para o código C de entrada, dado que os inputs sejam também os mesmos. Nos casos em que houver divergência de outputs, essa divergência deve ser razoável e, preferencialmente, prevista.

## Entregas


      DATA     |            RELEASE
:-------------:|:---------------------------:
06/04/2015     | [Release 0 - "Hello,world"] (https://github.com/brunobrg/CitasCobol/releases/tag/v0.0)
04/05/2015     |       [Release 1] (https://github.com/brunobrg/CitasCobol/releases/tag/v0.1)
08/06/2015     |       [Release 2] (https://github.com/brunobrg/CitasCobol/releases/tag/v0.2)
29/06/2015     |  Release 3 - "Stop Run"

