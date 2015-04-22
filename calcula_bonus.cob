
      *VRS001-12/12/2014-F8367856-Rafael   Implantacao
      *-----------------------*
       IDENTIFICATION DIVISION.
      *-----------------------*
       PROGRAM-ID.    T99P1237.
       AUTHOR.        Rafael.
       DATE-WRITTEN.  12/12/2014.
       DATE-COMPILED.
      *REMARKS.       Calcula bônus para empresas exportadoras
      *---------------------------------------------------------------*
       ENVIRONMENT DIVISION.
      *---------------------
       CONFIGURATION SECTION.
      *---------------------
       SPECIAL-NAMES.
           DECIMAL-POINT IS COMMA.
      *--------------------
       INPUT-OUTPUT SECTION.
      *--------------------
       FILE-CONTROL.
            SELECT T99F434E  ASSIGN  TO  "T99F434E.txt".
            SELECT T99F132S  ASSIGN  TO  "T99F132S.txt".
      *------------
       DATA DIVISION.
      *------------
       FILE SECTION.
      *------------
       FD  T99F434E.
       01  434E-REGISTRO-FD            PIC  X(063).

       FD  T99F132S.
       01  132S-REGISTRO-FD            PIC  X(046).
      *-----------------------
       WORKING-STORAGE SECTION.
      *-----------------------
       77  CTE-PROG                    PIC  X(016) VALUE
                     '*** T99P1237 ***'.

       77  CNT-SEQ-132                 PIC  S9(09)       VALUE 1.
       77  ACM-SEQ-132                 PIC  S9(09)       VALUE 1.
       77  ACM-SEQ-434                 PIC  S9(09)       VALUE 0.
       77  ACM-TOT-BONUS               PIC  S9(15)V99    VALUE 0.

      *    Arquivo de importaçoes e exportaçoes

      *    Detalhe
       01  434-REG-GERL.
           03  434-AGE                 PIC   9(04).
           03  434-CTA                 PIC   9(11).
           03  434-MCI                 PIC   9(09).
           03  434-VL-EXP              PIC   9(15)V99.
           03  434-VL-IMP              PIC   9(15)V99.
           03  434-SEQ                 PIC   9(05).

      *    Header
       01  FILLER REDEFINES 434-REG-GERL.
           03  FILLER                  PIC   X(15).
           03  434-NOM-ARQ             PIC   X(08).
           03  434-AMD-GER             PIC   9(08).
           03  FILLER                  PIC   X(32).

      *    Trailer
       01  FILLER REDEFINES 434-REG-GERL.
           03  FILLER                  PIC   X(52).
           03  434-SOMAT-SEQ           PIC   9(11).
      *    Arquivo de bonus

      *    Detalhe
       01  132-REG-GERL.
           03  132-AGE                 PIC   9(04).
           03  132-CTA                 PIC   9(11).
           03  132-MCI                 PIC   9(09).
           03  132-VL-BONUS            PIC   9(15)V99.
           03  132-SEQ                 PIC   9(05).

      *    Header
       01  FILLER REDEFINES 132-REG-GERL.
           03  FILLER                  PIC   X(15).
           03  132-NOM-ARQ             PIC   X(08).
           03  132-AMD-GER             PIC   9(08).
           03  FILLER                  PIC   X(15).

      *    Trailer
       01  FILLER REDEFINES 132-REG-GERL.
           03  FILLER                  PIC   X(15).
           03  132-VL-TOT-BONUS        PIC   9(15)V99.
           03  FILLER                  PIC   X(03).
           03  132-SOMAT-SEQ           PIC   9(11).

      *-------------------
       PROCEDURE DIVISION.
      *-------------------
       000000-ROTINA-PRINCIPAL SECTION.
      *--------------------------------

           PERFORM 100000-PROCED-INICIAIS.

           PERFORM 860000-LE-434.

           PERFORM UNTIL 434-AGE EQUAL 9999
               PERFORM 200000-CALCULA-BONUS
               PERFORM 860000-LE-434
           END-PERFORM.

           PERFORM 110000-PROCED-FINAIS.

           IF  ACM-TOT-BONUS GREATER 0
               MOVE 0 TO RETURN-CODE
           ELSE
               MOVE 1 TO RETURN-CODE
           END-IF

           DISPLAY '999 ' CTE-PROG ' 999 - FIM NORMAL'.
           STOP RUN.

       000000-SAIDA.
           EXIT.

      *-------------------------------
       100000-PROCED-INICIAIS SECTION.
      *-------------------------------
           OPEN INPUT  T99F434E
                OUTPUT T99F132S.

           READ T99F434E INTO 434-REG-GERL
                AT END PERFORM 999001-ERRO-001.

           IF  434-AGE     NOT EQUAL 0
               PERFORM 999002-ERRO-002.

           IF  434-CTA     NOT EQUAL 0
               PERFORM 999003-ERRO-003.

           IF  434-NOM-ARQ NOT EQUAL 'T99F434'
               PERFORM 999004-ERRO-004.

           IF  434-AMD-GER     EQUAL 0
               PERFORM 999005-ERRO-005.
           IF  434-SEQ     NOT EQUAL 1
               PERFORM 999006-ERRO-006.

           MOVE SPACES      TO 132-REG-GERL.
           MOVE 0           TO 132-AGE
                               132-CTA.
           MOVE 'T99F132'   TO 132-NOM-ARQ.
           MOVE 434-AMD-GER TO 132-AMD-GER.
           MOVE 1           TO 132-SEQ.

           WRITE 132S-REGISTRO-FD FROM 132-REG-GERL.

       100000-SAIDA.
           EXIT.

      *-----------------------------
       110000-PROCED-FINAIS SECTION.
      *-----------------------------
           IF  434-CTA NOT EQUAL 99999999999
               PERFORM 999008-ERRO-008.
           IF  434-SOMAT-SEQ NOT EQUAL ACM-SEQ-434
               PERFORM 999009-ERRO-009.
           READ T99F434E
                NOT AT END PERFORM 999010-ERRO-010.

           MOVE SPACES        TO 132-REG-GERL.
           MOVE 9999          TO 132-AGE.
           MOVE 99999999999   TO 132-CTA.
           MOVE ACM-TOT-BONUS TO 132-VL-TOT-BONUS.
           MOVE ACM-SEQ-132   TO 132-SOMAT-SEQ.
           WRITE 132S-REGISTRO-FD FROM 132-REG-GERL.

           CLOSE T99F434E
                 T99F132S.
       110000-SAIDA.
           EXIT.

      *-----------------------------
       200000-CALCULA-BONUS SECTION.
      *-----------------------------
           IF  434-VL-EXP GREATER 434-VL-IMP
               COMPUTE 132-VL-BONUS = (434-VL-EXP - 434-VL-IMP) * 0,005
               IF  132-VL-BONUS GREATER 0
                   MOVE 434-AGE     TO 132-AGE
                   MOVE 434-CTA     TO 132-CTA
                   MOVE 434-MCI     TO 132-MCI
                   ADD 132-VL-BONUS TO ACM-TOT-BONUS
                   PERFORM 870000-GRAVA-132
               END-IF
           END-IF
           .
       200000-SAIDA.
           EXIT.

      *--------------------------
       860000-LE-434 SECTION.
      *--------------------------
           ADD 434-SEQ TO ACM-SEQ-434.
           READ T99F434E INTO 434-REG-GERL
                AT END PERFORM 999007-ERRO-007.
       860000-SAIDA.
           EXIT.


      *-------------------------
       870000-GRAVA-132 SECTION.
      *-------------------------
           ADD 1            TO CNT-SEQ-132
           ADD CNT-SEQ-132  TO ACM-SEQ-132
           MOVE CNT-SEQ-132 TO 132-SEQ
           WRITE 132S-REGISTRO-FD FROM 132-REG-GERL
           .
       870000-SAIDA.
           EXIT.
      *---------------------
       999000-ERROS SECTION.
      *---------------------
       999001-ERRO-001.
           DISPLAY '888 ' CTE-PROG ' 001 - Arquivo T99F434 vazio.'.
           PERFORM 999999-CANCELAR.

       999002-ERRO-002.
           DISPLAY '888 ' CTE-PROG ' 002 - Arquivo T99F434 sem header.'.
           PERFORM 999999-CANCELAR.

       999003-ERRO-003.
           DISPLAY '888 ' CTE-PROG ' 003 - NUMERO DA CONTA NO HEADER'.
           DISPLAY ' NãO ZERO.'.
           PERFORM 999999-CANCELAR.

       999004-ERRO-004.
           DISPLAY '888 ' CTE-PROG ' 004 - Arquivo não é T99F434.'.
           DISPLAY '888 ' CTE-PROG ' 004 - Recebido: ' 434-NOM-ARQ.
           PERFORM 999999-CANCELAR.

       999005-ERRO-005.
           DISPLAY '888 ' CTE-PROG ' 005 - Data do T99F434 inválida.'.
           DISPLAY '888 ' CTE-PROG ' 005 - Recebido: ' 434-AMD-GER.
           PERFORM 999999-CANCELAR.

       999006-ERRO-006.
           DISPLAY '888 ' CTE-PROG ' 006 - SEQUENCIAL DO HEADER DO'.
           DISPLAY ' T99FF434 INVALIDO'.
           DISPLAY '888 ' CTE-PROG ' 006 - Esperado:      1'.
           DISPLAY '888 ' CTE-PROG ' 006 - Recebido: ' 434-SEQ.
           PERFORM 999999-CANCELAR.

       999007-ERRO-007.
           DISPLAY '888 ' CTE-PROG ' 007 - T99F434 sem trailer.'.
           DISPLAY '888 ' CTE-PROG ' 007 -  ultimo lido: ' 434-SEQ.
           PERFORM 999999-CANCELAR.

       999008-ERRO-008.
           DISPLAY '888 ' CTE-PROG ' 008 - CTA do trailer inválida.'.
           DISPLAY '888 ' CTE-PROG ' 008 - Esperado: 99999999999'.
           DISPLAY '888 ' CTE-PROG ' 008 - Recebido: ' 434-CTA.
           PERFORM 999999-CANCELAR.

       999009-ERRO-009.
           DISPLAY '888 ' CTE-PROG ' 009 - ACUMULADOR DE SEQUENCIAL DO'.
           DISPLAY ' TRAILER.'.
           DISPLAY '888 ' CTE-PROG ' 009 - do T99F434 não confere.'.
           DISPLAY '888 ' CTE-PROG ' 009 - Esperado: ' ACM-SEQ-434.
           DISPLAY '888 ' CTE-PROG ' 009 - Recebido: ' 434-SOMAT-SEQ.
           PERFORM 999999-CANCELAR.

       999010-ERRO-010.
           DISPLAY '888 ' CTE-PROG ' 010 - T99F434 COM REGISTROS APóS '
                   ' O TRAILER.'.
           PERFORM 999999-CANCELAR.
      *------------------------
       999999-CANCELAR SECTION.
      *------------------------
           DISPLAY '999 ' CTE-PROG ' CANCELADO.'.
           STOP RUN.
       999999-SAIDA-ABENDA.
           EXIT.
