/*
        BATCH 10
        Gargi Sharma 2013A7PS161P
        Ayushi Agarwal 2013A7PS065P
*/

#include "symbolTableDef.h"
#ifndef _parserDef_h   /* Include guard */
#define _parserDef_h

#define TOT_RULES 83
#define NUM_TERMINALS 56
#define NUM_NONTERMINALS 48
//int correctParseTree=0;
//extern VPtr;
int table[NUM_NONTERMINALS][NUM_TERMINALS];
struct node{
	char name[30];
    int isTerm;
    struct node* next;
    struct node* prev;
};

typedef struct rule1 rule;
struct rule1{
	int rnum;
	struct node *lNode;
	struct node *front;
	struct node *rear;
	int epsilon;
};

typedef struct Fnode1 Fnode;
struct Fnode1
{
    char terminal[15];
    struct Fnode1* next;
};

typedef struct Fhead1 Fhead;
struct Fhead1
{
    char nonTerminal[30];
    Fnode* head;
    Fnode* rear;
    int isCal;
    int eflag;
};

typedef struct treeNode1 treeNode;
struct treeNode1
{
    char lexeCurNode[30];
    int lineno;
    char token[30];
    float valuIfNumber;
    char parentNodeSymbol [30];
    char isLeafNode [5];
    int isTerm;
    char NodeSymbol[30];
    treeNode* child;
    treeNode* prev;
    treeNode* next;
    treeNode* parent;
    VPtr SymPtr;
};

typedef struct tree1 tree;
struct tree1
{
    treeNode* root;
    treeNode* rear;
    treeNode* front;
    int height;
};

rule *ru;
Fhead* h;
Fhead* fo;
Fhead* ss;

#endif