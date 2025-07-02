# Tiny Language Context Free Grammer

## `PROGRAM -> STATEMENT PROGRAM | STATEMENT` >> *left factoring*

## PROGRAM -> STATEMENT PROGRAM'
## PROGRAM' -> PROGRAM | ε


## STATEMENT -> READ | WRITE | IF | REPEAT | ASSIGNMENT


## READ -> read RM;
## `RM -> ID | ID, RM` >> *left factoring*

## RM -> ID RM'
## RM' = ε | , RM

## WRITE -> write WM;
## `WM -> ID | ID, WM | "STRING" | "STRING", WM` >> *left factoring*

## WM -> ID WM' | "STRING" WM'
## WM' -> ε | , WM

## STRING -> CHAR STRING | ε
## CHAR -> [^"]


## IF -> if COND then PROGRAM OPTION
## OPTION -> end | else PROGRAM end
## COND -> EXP COMPAOP EXP

## REPEAT -> repeat PROGRAM until COND;
## ASSIGNMENT -> ID ASSIGNOP EXP;


## `EXP -> EXP ARITHOP EXP | TERM | (EXP)` >> *left recursion*
 
## EXP -> TERM EXP' | (EXP) EXP'
## EXP' -> ARITHOP EXP EXP' | ε

## TERM -> ID | NUMBER


## ARITHOP -> * | / | + | -
## COMPAOP -> < | =
## ASSIGNOP -> :=


## NUMBER -> DIGIT NUMBER | DIGIT
## DIGIT -> [0 – 9]


## ID -> LETTER X
## X -> LETTER X | DIGIT X | ε
## LETTER -> [A – Z] | [a – z]
