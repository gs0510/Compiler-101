#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
//#include "symbolTable.h"
#include "parserDef.h"
#define MAX_SIZE 40

int OFFSET = 0;
int first = 0;

int hashing(char stri[],int sz)
{
	int i=0,s=0;
	while(stri[i]!='\0'){
		s = s + toascii(stri[i]);
		i++;	
	}
	return s%sz;
}

int globaloff(FPtr fglobal, char *recid)
{
	int offset = 0;
	int s = fglobal->size;
	VPtr v = fglobal->v;
	int i=0;
	while(i<s)
	{
		if((v+i)->status==1){
			if(strcmp(recid,(v+i)->lexName)==0)
			{
				RPtr r = (v+i)->r;
				int s1 = (v+i)->size;
				int j=0;
				while(j<s){
					if(strcmp((r+j)->type,"INT")==0&&(r+j)->status == 1 )
					{
						offset +=2;
					}
					if(strcmp((r+j)->type,"REAL")==0&&(r+j)->status == 1 )
					{
						offset +=4;
					}	
					j++;
				}
			}
		}
		i++;
	}
	return offset;
}

void initializeVPtr(FPtr f,VPtr v,int index)
{
	(f+index)->v = v;
	(f+index)->size = MAX_SIZE;
	(f+index)->status = 0;
	(f+index)->inparam = 0;
	(f+index)->outparam = 0;
}

void initializeRPtr(VPtr v,RPtr r,int index)
{
	(v+index)->size = MAX_SIZE;
	(v+index)->status = 0;
	(v+index)->r = r;
	(v+index)->offset=0;
	(v+index)->inparam = 0;
	(v+index)->outparam = 0;
	(v+index)->ifassigned = 0;
}


SPtr initializeSymbolTable(){
	
	FPtr fnPtr = (FPtr)malloc(MAX_SIZE*sizeof(functionTable));
	SPtr symTabPtr = (SPtr)malloc(sizeof(symbolTable));
	symTabPtr->fp= fnPtr;
	symTabPtr->size = MAX_SIZE;
	int a=0,b=0,c=0;
	for(a=0;a<MAX_SIZE;a++)
	{
		VPtr varPtr;

		varPtr = (VPtr)malloc(MAX_SIZE*sizeof(variableTable));
     	initializeVPtr(fnPtr,varPtr,a);
		for(b=0;b<MAX_SIZE;b++)
		{

			RPtr recPtr;
			recPtr = (RPtr)malloc(MAX_SIZE*sizeof(recordTable));
			initializeRPtr(varPtr,recPtr,b);
			for(c=0;c<MAX_SIZE;c++)
			{
				(recPtr+c)->status=0;
				(recPtr+c)->offset = 0;
			}
		}
	}
	return symTabPtr;
}


FPtr insertFunctionTable(SPtr sp,int linenum,char fname[])
{
	FPtr fp1;
	fp1 = sp->fp; 
	int size = sp->size;
	int i=0;
	for(i=0;i<size;i++)
	{
		if(!strcmp((fp1+i)->functionName,fname))
		{
			printf("Function %s cannot be overloaded. Error at line number %d\n",fname,linenum);
			return ;
		}
	}
	int khash;	
	khash = hashing(fname,size);
	if((fp1+khash)->status == 0)
	{
		(fp1+khash)->status = 1;
		strcpy(((fp1+khash)->functionName),fname);
		(fp1+khash)->lineno = linenum;
		return (fp1+khash);
	}
	int j=0;
	int baseptr1=0,baseptr=0;
	do
	{
		j++;	
		baseptr1 = khash + pow(j,2);
		baseptr = baseptr1%size;
	}
	while((fp1+baseptr)->status != 0 && j!=size);
	
	if((fp1+baseptr)->status == 0)
	{  
		(fp1+baseptr)->lineno=linenum;
    	(fp1+baseptr)->status = 1;
		strcpy(((fp1+baseptr)->functionName),fname);
		return (fp1+baseptr);
	}
	else
	{
		return;
	}
}

VPtr insertVar(FPtr fglobal,FPtr fp,treeNode* tn,char type[],int offset)
{
	char hashablekhash[256];
	int size = fp->size;
	int i=0;
	VPtr v = fp->v;
	VPtr vg = fglobal->v;
	while(i<size)
	{
		if(strcmp(type,"UNKNOWN")==0&&strcmp((v+i)->lexName,tn->lexeCurNode)==0 && v!=vg)
		{
			return (v+i);
		}
		if(strcmp(type,"UNKNOWN")!=0&&strcmp((v+i)->lexName,tn->lexeCurNode)==0 && v!=vg)
		{
			printf("VARIABLE %s HAS ALREADY BEEN DECLARED. ERROR AT LINENO %d.\n",tn->lexeCurNode,tn->lineno);
			if(strcmp(type,"INT")==0)
				OFFSET -= 2;
			if(strcmp(type,"REAL")==0)
				OFFSET = OFFSET - 4;
			return NULL;
		}
		i++;
	}
	i=0;
	while(i<size){
		if(strcmp(type,"UNKNOWN")==0&&strcmp((vg+i)->lexName,tn->lexeCurNode)==0)
		{
			return (vg+i);
		}
		if(strcmp(type,"UNKNOWN")!=0&&strcmp((vg+i)->lexName,tn->lexeCurNode)==0){
			printf("VARIABLE %s is GLOBAL. CANNOT BE DECLARED AGAIN. ERROR AT LINENO %d\n",tn->lexeCurNode,tn->lineno);
			if(strcmp(type,"INT")==0)
				OFFSET -= 2;
			if(strcmp(type,"REAL")==0)
				OFFSET = OFFSET - 4;			
			return NULL;
		}	
		i++;
	}

	if(!strcmp(type,"UNKNOWN"))
	{
		printf("VARIABLE %s NOT DECLARED HERE. ERROR AT LINEON %d.\n",tn->lexeCurNode,tn->lineno);
		return NULL;
	}
	char hkey[256];
	strcpy(hkey,tn->lexeCurNode);
	int khash;
	khash = hashing(hkey,size);
	if((v+khash)->status == 0)
	{
		(v+khash)->status = 1;
		(v+khash)->lineno = tn->lineno;
		strcpy((v+khash)->lexName,tn->lexeCurNode);
		strcpy((v+khash)->type,type);
		(v+khash)->offset = offset;
		return (v+khash);
	}
	int baseptr=0,baseptr1=0;
	int openaddr=0;
	openaddr=openaddr+1;
	baseptr1 = khash + pow(openaddr,2);
	baseptr=baseptr1%size;	
	while((v+baseptr)->status != 0 && openaddr!=size);
	{
		openaddr++;	
		baseptr1 = khash + pow(openaddr,2);
		baseptr = baseptr1%size;
	}
	if((v+baseptr)->status == 0)
	{  
		strcpy((v+baseptr)->lexName,tn->lexeCurNode);
		strcpy((v+baseptr)->type,type);
    	(v+baseptr)->status = 1;
		(v+baseptr)->lineno = tn->lineno;
		(v+baseptr)->offset = offset;
		return (v+baseptr);
	}
	else
	{
		return NULL;
	}
}

void insertRecordTable(VPtr vp,treeNode *tn,int offset,char type[])
{
	RPtr r = vp->r;
	int size = vp->size;
	int i=0;
	for(i=0;i<size;i++)
	{
		if(!strcmp((r+i)->lexName,tn->lexeCurNode))
		{
			printf("RECORD %s HAS BEEN PREVIOUSLY DEFINED.\n",tn->lexeCurNode);
			return ;
		}
	}
	i=0;
	for(i=0;i<size;i++)
	{
		if((r+i)->status==0)
		{
			(r+i)->status = 1;
			(r+i)->lineno = tn->lineno;
			strcpy((r+i)->lexName,tn->lexeCurNode);
			strcpy((r+i)->type,type);
			(r+i)->offset = offset;
			return ;
		}
	}
	/*char hkey[20];
	strcpy(hkey,tn->lexeCurNode);
	int khash = hashing(hkey,size);
	if((r+khash)->status == 0)
	{
		(r+khash)->status = 1;
		(r+khash)->lineno = tn->lineno;
		strcpy((r+khash)->lexName,tn->lexeCurNode);
		strcpy((r+khash)->type,type);
		(r+khash)->offset = offset;
		return ;
	}
	int baseptr=0,baseptr1=0;
	int openaddr=0;
	openaddr=openaddr+1;
	baseptr1 = khash + pow(openaddr,2);
	baseptr=baseptr1%size;	
	while((r+baseptr)->status != 0 && openaddr!=size);
	{
		openaddr++;	
		baseptr1 = khash + pow(openaddr,2);
		baseptr = baseptr1%size;
	}
	if((r+baseptr)->status= 0)
	{  
		strcpy((r+khash)->lexName,tn->lexeCurNode);
		strcpy((r+khash)->type,type);
    	(r+baseptr)->status = 1;
    	(r+baseptr)->lineno = tn->lineno;
		(r+baseptr)->offset = offset;
		return ;
	}
	else
	{
		return ;
	}*/
}

void addrecordSymTab(treeNode* tp,VPtr vp)
{
	int condition = 0;
	char type[256];
	treeNode* tn;
	treeNode* temp = tp;
	temp= temp->child;
	while(temp!=NULL&&!strcmp("<fieldDefinition>",temp->NodeSymbol))
	{	
		if(!strcmp("TK_REAL",temp->child->next->NodeSymbol))
		{
			strcpy(type,"REAL");
			tn = temp->child->next->next;
			printf("%s\n",tn->lexeCurNode );
			insertRecordTable(vp,tn,OFFSET,type);
		}
		else if(!strcmp("TK_INT",temp->child->next->NodeSymbol))
		{
			
			strcpy(type,"INT");
			tn = temp->child->next->next;
			insertRecordTable(vp,tn,OFFSET,type);
		}
		temp=temp->next;
	}
	if(temp!=NULL)addrecordSymTab(temp,vp);
	else return;
}

void addingRecords1(treeNode* ast,FPtr fglobal)
{
	if(strcmp(ast->NodeSymbol,"<typeDefinitions>")==0)
	{
		char TYPE[256];
		treeNode* temp = ast->child->child->next;
		strcpy(TYPE,"RECORD");
		VPtr vp = insertVar(fglobal,fglobal,temp,TYPE,OFFSET);
		temp->SymPtr=vp;
		addrecordSymTab(temp->next,vp);
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		addingRecords1(child,fglobal);
		child=child->next;
	}
}

FPtr globalTable(SPtr s)
{
	FPtr f;
	char global[] = "global";
	f = insertFunctionTable(s,0,global);
	return f;
}



int inOutPar=0,flag=0;
void populateSymbolTable(FPtr f, FPtr fglobal,treeNode* ast,int *counter){
	int condition = 0;
	int position = 0;
	treeNode* tn;
	char TYPE[256];
	VPtr vp;	
	if(strcmp(ast->NodeSymbol,"<output_par>")==0){
		f->inparam=*counter;
		inOutPar=1;
		*counter=0;
		flag=1;
	}
	else if(strcmp(ast->NodeSymbol,"<stmts>")==0){
		if(flag==0)
		{
			*counter=0;
		}
		f->outparam=*counter;
		inOutPar=0;
		*counter=0;
	}
	else if(strcmp(ast->NodeSymbol,"TK_ID")==0)
	{
		(*counter)++;
		tn=ast;
		VPtr vp;
		if(ast->prev!=NULL&&strcmp(ast->prev->NodeSymbol,"TK_INT")==0){
			if(ast->next!=NULL&&strcmp(ast->next->NodeSymbol,"TK_GLOBAL")==0){
				strcpy(TYPE,"INT");
				vp=insertVar(fglobal,fglobal,tn,TYPE,OFFSET);
				ast->SymPtr=vp;
			}
			else{
				if(first)
						OFFSET+=2;
				first = 1;
				strcpy(TYPE,"INT");
				vp=insertVar(fglobal,f,tn,TYPE,OFFSET);
				ast->SymPtr=vp;
			}

		}
		else if(ast->prev!=NULL&&strcmp(ast->prev->NodeSymbol,"TK_REAL")==0){
			if(ast->next!=NULL&&strcmp(ast->next->NodeSymbol,"TK_GLOBAL")==0){
				strcpy(TYPE,"REAL");
				vp=insertVar(fglobal,fglobal,tn,TYPE,OFFSET);
				ast->SymPtr=vp;
			}
			else{
				if(first)
						OFFSET+=4;
				first = 1;
				strcpy(TYPE,"REAL");
				vp=insertVar(fglobal,f,tn,TYPE,OFFSET);
				ast->SymPtr=vp;
			}
		}
		else if(ast->prev!=NULL&&strcmp(ast->prev->NodeSymbol,"<dataType>")==0){
			treeNode *temp=ast->prev;
			temp=temp->child;
			temp=temp->child->next;
			strcpy(TYPE,temp->lexeCurNode);
			if(ast->next!=NULL&&strcmp(ast->next->NodeSymbol,"TK_GLOBAL")==0){
				vp=insertVar(fglobal,fglobal,tn,TYPE,OFFSET);
					ast->SymPtr=vp;	
			}
			else{
				int x=globaloff(fglobal,TYPE);
				if(first!=0)
				{
					OFFSET = OFFSET + x;
					if(x==0)
					{
						printf("%s record type is not defined.Error in line no %d\n",TYPE,ast->lineno );
					}
					else{
						vp=insertVar(fglobal,f,tn,TYPE,OFFSET);
						ast->SymPtr=vp;	
					}
				}
				else{
					
					if(x==0)
					{
						printf("%s record type is not defined.Error in line no %d\n",TYPE,ast->lineno );
					}
					else{
						vp=insertVar(fglobal,f,tn,TYPE,OFFSET);
						ast->SymPtr=vp;	
						first = 1;
					}
				}
			}
		}
		else
		{
			strcpy(TYPE,"UNKNOWN");
			vp=insertVar(fglobal,f,tn,TYPE,OFFSET);
			ast->SymPtr=vp;
		}

		if(ast->parent!=NULL&&ast->parent->parent!=NULL&&(!strcmp(ast->parent->parent->NodeSymbol,"<input_par>") || !strcmp(ast->parent->parent->NodeSymbol,"<remaining_list>"))&&inOutPar==0){
			vp->inparam=*counter;
		}
		if(ast->parent!=NULL&&ast->parent->parent!=NULL&&(!strcmp(ast->parent->parent->NodeSymbol,"<output_par>")|| !strcmp(ast->parent->parent->NodeSymbol,"<remaining_list>"))&&inOutPar==1){
			vp->outparam=*counter;
		}	
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		populateSymbolTable(f,fglobal,child,counter);
		child=child->next;
	}
}



void findingFunctions(treeNode* ast,SPtr s, FPtr fglobal){
	FPtr f;
	if(strcmp(ast->NodeSymbol,"<function>")==0 || strcmp(ast->NodeSymbol,"<mainFunction>")==0 )
	{
		OFFSET =0;
		first=0;
		f = insertFunctionTable(s,ast->child->lineno,ast->child->lexeCurNode);
		int c =0; flag=0;
		populateSymbolTable(f,fglobal,ast,&c);
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		findingFunctions(child,s,fglobal);
		child=child->next;
	}
}


void checkForDoubleDec(SPtr sp,FPtr fglobal){
	FPtr fp = sp->fp;
	VPtr vglobal = fglobal->v;
	int i=0;
	for(i=0;i<sp->size;i++){
		if((fp+i)!=fglobal){
			int j=0;
			VPtr vp = (fp+i)->v;
			for(j=0;j<(fp+i)->size;j++){
				int k=0;
				for(k=0;k<(fglobal)->size;k++){
					if(!strcmp((vp+j)->lexName,(vglobal+k)->lexName)&&strcmp((vp+j)->lexName,""))
					{
						printf("LINENO %d: Variable %s being a global variable cannot be declared more than once\n",(vp+j)->lineno,(vp+j)->lexName);
					}
				}
			}
		}
	}
}


void printSymbolTable(SPtr s)
{
	FPtr f = s->fp;
	int i=0,o=0;
	int size=s->size;
	while(i<size)
	{
		if((f+i)->status == 1)
		{	
			char domain[256];
			strcpy(domain,((f+i)->functionName));
			int j=0;
			while(j<size)
			{
				VPtr v = (f+i)->v;
				if(!strcmp(domain,"global") && (v+j)->status == 1)
				{	
					if(strcmp((v+j)->type,"RECORD"))
						printf("%20s%20s%20s%10d %d %d\n",(v+j)->lexName,domain,(v+j)->type,o,(v+j)->inparam,(v+j)->outparam);					
					RPtr rp = (v+j)->r;
					int k=0;
					while(k<size)
					{	
						char rec[256];
						strcpy(rec,(v+j)->lexName);
						strcat(rec,".");
						if((rp+k)->status == 1)
						{	
							char field[256];
							strcpy(field,(rp+k)->lexName);
							strcat(rec,field);
							printf("%20s%20s%20s%10d\n",rec,domain,(rp+k)->type,(rp+k)->offset);
						}
						k++;
					}	
				}
				else if(strcmp(domain,"global") && (v+j)->status == 1)
				{
					printf("%20s%20s%20s%10d %d %d\n",(v+j)->lexName,domain,(v+j)->type,(v+j)->offset,(v+j)->inparam,(v+j)->outparam);
				}
				j++;
			}

		}
		i++;
	}
}


void createSymbolTable(treeNode* ast,SPtr symPtr,FPtr fglobal){
	treeNode* temp = ast;
	addingRecords1(temp,fglobal);
	temp = ast;
	findingFunctions(temp,symPtr,fglobal);
	checkForDoubleDec(symPtr,fglobal);
	//printSymbolTable(symPtr);
}

int semanticChecks(treeNode* ast,SPtr symPtr,FPtr fglobal)
{	int flag;
	flag=0;
	//treeNode* temp = ast;
	checkifDeclared(ast,symPtr,fglobal,&flag);
	//temp = ast;
	checkInputParameters(ast,symPtr,fglobal,&flag);
	char fname[40];
	int req=0;
	//temp = ast;
	checkReturnValues(ast,symPtr,fglobal,&req,fname,&flag);
	//temp = ast;
	setIfAssigned(ast,symPtr,fglobal,&flag);
	//printf("11\n");
	//temp = ast;
	checkIfAssigned(ast,symPtr,fglobal,&flag);
	//printf("22\n");
	checkWhile(ast,&flag);
	//printf("33\n");
	typeChecking(ast,fglobal,&flag);
	//printf("44\n");
	booleanCheck(ast,fglobal,&flag);
	//printf("55\n");
	//recCheck(ast,fglobal);
	readCheck(ast,fglobal,&flag);
	checkConditional(ast,fglobal,&flag);
	//printf("aaaa\n");
	if(flag){
		return 0;
	}
	else{
		return 1;
	}
}

void createAssemblyCode(char* filename,treeNode* ast,SPtr symPtr,FPtr fglobal)
{
	printf("helllooo\n");
	FILE* fp1=fopen(filename,"w");
	NASM(ast,symPtr,fp1,fglobal);
	printf("wooooowooa\n");
	fclose(fp1);
}