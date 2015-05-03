       IDENTIFICATION DIVISION.
       PROGRAM-ID. soma.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
      * estou assumindo que x,y e soma sao int.
       01 x    PIC  S9(05) VALUE 30.
       01 y    PIC  S9(05).
       01 soma PIC  S9(05).

       PROCEDURE DIVISION.

       MOVE 10 to y.

       COMPUTE soma = x + y.
       DISPLAY soma.

       STOP RUN.
       EXIT.