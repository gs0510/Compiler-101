/*
		BATCH 10
		Gargi Sharma 2013A7PS161P
		Ayushi Agarwal 2013A7PS065P

*/

#ifndef _lexerDef_h   /* Include guard */
#define _lexerDef_h

typedef struct tokenInfo1 tokenInfo;
struct tokenInfo1{
	int lineno;
	char token[20];	
	char lexeme[100];
	int value;
	tokenInfo *next;
};

#endif
