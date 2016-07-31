/*
		BATCH 10
		Gargi Sharma 2013A7PS161P
		Ayushi Agarwal 2013A7PS065P

*/

#ifndef _lexer_h   /* Include guard */
#define _lexer_h

#include "lexerDef.h"

FILE *getStream(FILE *fp, int k);

tokenInfo getNextToken( FILE *fp,int k);

tokenInfo* ABC(char* filename,int k);
#endif
