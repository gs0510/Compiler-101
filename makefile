#        BATCH 10
#        Gargi Sharma 2013A7PS161P
#        Ayushi Agarwal 2013A7PS065P

all:
	gcc -g  parser.c lexer.c typechecker.c semanticAnalyzer.c  symbolTable.c codeGen.c driver.c -o stage1exe -lm


