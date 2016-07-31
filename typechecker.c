#include "symbolTableDef.h"
#include "parserDef.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *getfieldtype(char recordid[],char identifier[],FPtr fglobal)
{
	//printf("entered\n");
	VPtr vp = fglobal->v;
	RPtr rp;
	int i=0,j=0;
	char *type;
	type = (char*)malloc(20*sizeof(char));
	strcpy(type,"UNKNOWN");
	for(i=0;i<(fglobal->size);i++)
	{	
		//printf("%s\n",(vp+i)->tInfo.lexemeName);
		if(!strcmp(recordid,(vp+i)->lexName))
		{
			//printf("entered\n");
			//printf("ingettype = %s\n",(vp+i)->tInfo.lexemeName);
			rp = (vp+i)->r;
			for(j=0;j<(vp->size);j++)
			{
				if(!strcmp((rp+j)->lexName,identifier))
				{
					//printf("actualtype = %s\n",(rp+j)->tInfo.lexemeName);
					return (rp+j)->type;
				}			
			}
			
		}
	}
	return type;
}

void typeChecker(treeNode* ast, FPtr fglobal,char ty[],int *flag){

	if(strcmp(ast->NodeSymbol,"TK_RNUM")==0){
		if(strcmp(ty,"REAL")){
			*flag=1;
			printf("Type mismatch. The type on left hand side is %s. Error at line no %d.\n",ty,ast->lineno );
		}
		//printf("11\n");
	}
	if(strcmp(ast->NodeSymbol,"TK_NUM")==0){
		if(strcmp(ty,"INT")){
			*flag=1;
			printf("Type mismatch. The type on left hand side is %s. Error at line no %d.\n",ty,ast->lineno );
		}
		//printf("22\n");
	}
	if(strcmp(ast->NodeSymbol,"<singleOrRecId>")==0){
		//printf("%s %s tytyyt\n",ast->NodeSymbol,ty );
		if(ast->child->SymPtr!=NULL&&strcmp(ty,getfieldtype(ast->child->SymPtr->type,ast->child->next->lexeCurNode,fglobal))!=0){
			*flag=1;
			printf("Type mismatch. The type on left hand side is %s. Error at line no %d.\n",ty,ast->child->lineno );
		}
		//printf("%s lexee 33\n",ast->lexeCurNode );
	}
	if(strcmp(ast->NodeSymbol,"TK_ID")==0){
		//printf("44\n");
		//printf("%s lexee\n",ast->lexeCurNode );

		if(ast->next==NULL)// && strcmp(ast->next->NodeSymbol,"TK_FIELDID")!=0)
		{
			if(ast->SymPtr!=NULL&&strcmp(ty,ast->SymPtr->type)!=0){
				*flag=1;
				printf("Type mismatch. The type on left hand side is %s. Error at line no %d.\n",ty,ast->lineno );
			}
		}
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		typeChecker(child,fglobal,ty,flag);
		child=child->next;
	}
}



void typeCheck2(treeNode* ast,FPtr fglobal,char* ty,int linenum,int *flag){
	
	if(strcmp(ast->NodeSymbol,"TK_MUL")==0||strcmp(ast->NodeSymbol,"TK_DIV")==0){
		if(strcmp(ast->next->NodeSymbol,"TK_NUM")&&strcmp(ast->next->NodeSymbol,"TK_RNUM")){
			printf("111111\n");
			*flag=1;
			printf("RECORD cant be multiplied or divided by %s.Error at line no %d\n",ast->next->NodeSymbol,linenum );
		}
	}	
	if(strcmp(ast->NodeSymbol,"TK_NUM")==0||strcmp(ast->NodeSymbol,"TK_RNUM")==0){
		if(ast->prev!=NULL&&(strcmp(ast->prev->NodeSymbol,"TK_MUL")&&strcmp(ast->prev->NodeSymbol,"TK_DIV"))){
			*flag=1;
			printf("Type does not match .Error at line no %d\n",linenum );
		}
	}
	if(strcmp(ast->NodeSymbol,"<singleOrRecId>")==0){
		if(ast->child->SymPtr!=NULL&&strcmp(ty,getfieldtype(ast->child->SymPtr->type,ast->child->next->lexeCurNode,fglobal))!=0){
			*flag=1;
			printf("Type mismatch. The type on left hand side is %s. Error at line no %d.\n",ty,ast->child->lineno );
		}
	}
	if(strcmp(ast->NodeSymbol,"TK_ID")==0){
		//printf("44\n");
		//printf("%s lexee\n",ast->lexeCurNode );

		if(ast->next==NULL)// && strcmp(ast->next->NodeSymbol,"TK_FIELDID")!=0)
		{
			if(ast->SymPtr!=NULL&&strcmp(ty,ast->SymPtr->type)!=0){
				*flag=1;
				printf("Type mismatch. The type on left hand side is %s. Error at line no %d.\n",ty,ast->lineno );
			}
		}
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		typeCheck2(child,fglobal,ty,linenum,flag);
		child=child->next;
	}	
}

void typeChecking(treeNode* ast,FPtr fglobal,int *flag){
	char ty[256];
	if(strcmp(ast->NodeSymbol,"<assignmentStmt>")==0){	
		//printf("hi\n");
		if(strcmp(ast->child->NodeSymbol,"TK_ID")==0){ 
		//	printf("%s 22\n",ast->child->lexeCurNode );
			strcpy(ty,ast->child->SymPtr->type);
		//	printf("%s 11\n", ty);
			if(strcmp(ast->child->SymPtr->type,"INT")&&strcmp(ast->child->SymPtr->type,"REAL")){
				typeCheck2(ast->child->next->next,fglobal,ty,ast->child->lineno,flag);	
			}
			else{
				typeChecker(ast->child->next->next,fglobal,ty,flag);
			}
		}
		else{
			strcpy(ty,getfieldtype(ast->child->child->SymPtr->type,ast->child->child->next->lexeCurNode,fglobal));
			typeChecker(ast->child->next->next,fglobal,ty,flag);
		//	printf("%s 11\n", ty);
		}
		
		//printf("hello\n");
	}

	treeNode* child=ast->child;
	while(child!=NULL)
	{
		typeChecking(child,fglobal,flag);
		child=child->next;
	}
}

