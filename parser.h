/*
		BATCH 10
		Gargi Sharma 2013A7PS161P
		Ayushi Agarwal 2013A7PS065P

*/

#ifndef _parser_h   /* Include guard */
#define _parser_h

#include "lexerDef.h"
#include "parserDef.h"


void popGram(char* filename);

void popRule(rule *ru,char str[]);

void popFirst(rule* ru);

void populateFollows();

tree* parseInputSourceCode(tokenInfo *ti, int* dem);

int printParseTree(tree* t);

void populatesyncset();

#endif
