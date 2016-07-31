//#include "parser.h"
#ifndef _symbolTableDef_h   /* Include guard */
#define _symbolTableDef_h

struct rTable{
	char lexName[20];
	char type[20];
	int lineno;
	//treeNode* tn;
	int status;
	int offset;
};
typedef struct rTable recordTable;
typedef recordTable *RPtr;
typedef struct vTable variableTable;
typedef variableTable *VPtr;
struct vTable{
	RPtr r;
	char lexName[256];
	//treeNode* tn;
	int lineno;
	char type[256];
	int status;
	int size;
	int offset;
	int inparam;
	int outparam;
	int ifassigned;
	int num_fields;
};

typedef struct fTable functionTable;
typedef functionTable *FPtr;
struct fTable{
	VPtr v;
	char functionName[256];
	int lineno;
	int size;
	int status;
	int inparam;
	int outparam;
};

typedef struct sTable symbolTable;
typedef symbolTable *SPtr;
struct sTable{
	FPtr fp;
	int size;
};

#endif