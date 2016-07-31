#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTableDef.h"
#include "parserDef.h"
int bufcount=0;
int LabelNo=0;
void printingRecords(FILE* f,char type[],char id[],FPtr fglobal)
{
	FPtr fp=fglobal;
	int count=0;
	int j,k;
	VPtr vp;
	vp=fp->v;
	for(j=0;j<fp->size;j++)
	{
		if( !strcmp((vp+j)->lexName,type)  ) //last good config--- strcmp((vp+j)->t,"INT") && strcmp((vp+j)->t,"REAL")
		{//assuming unknown type are not stored in the symbol table
			//fprintf(f, "%-10s%10s\n","STRUC",(vp+j)->lexName);
			RPtr rp=(vp+j)->r;
			for (k = 0; k < vp->size; k++)
			{
				//printf("%s\n",(rp+k)->t );
				if(!strcmp((rp+k)->type,"INT")){
					fprintf(f,"%s.%s%10s\n",id,(rp+k)->lexName,"resd		1");
					count++;
				}
				else if(!strcmp((rp+k)->type,"REAL")){
				fprintf(f,"%s%10s\n",(rp+k)->lexName,"resd		2");
				count++;
				}	
			}
			(vp+j)->num_fields=count;
			//fprintf(f, "%-10s\n","ENDSTRUC");
		}
	}	
	fseek(f,0,SEEK_END);
	return ;
}


void variablesHandle(SPtr sptr,FILE* f,FPtr fglobal)
{
	FPtr fp;
	fp=sptr->fp;
	int i,j,k;
	VPtr vp;
	for(i=0;i<fp->size;i++)
	{
		
		if(!strcmp((fp+i)->functionName,"_main"))
		{//accessing only the variables in the main function
			//now do the data declarations for each datatype
			vp=(fp+i)->v;
			for(j=0;j<fp->size;j++)
			{
				
				if(!strcmp((vp+j)->type,"INT"))
				{
					fprintf(f,"%-10s%10s\n",(vp+j)->lexName,"resd		1");
				}
				else if(!strcmp((vp+j)->type,"REAL"))
				{
					fprintf(f,"%-25s%s\n",(vp+j)->lexName,"resd		2");
				}
				else if((vp+j)->status==1)
				{
					//fprintf(f,"%-10s%10s%10d\n",(vp+j)->lexName,"resd",findoffset(fglobal,(vp+j)->type)/2);
					//fprintf(f, ".buffer%d 	resd  %d\n", bufcount,(vp+j)->num_fields);
					//bufcount++;
					printingRecords(f,(vp+j)->type,(vp+j)->lexName,fglobal);
					
				}		
			}
		}
	}
	VPtr vglobal= fglobal->v;
	for(i=0;i<(fglobal->size);i++){
		if(!strcmp((vglobal+i)->type,"INT"))
		{
			fprintf(f,"%-10s%10s\n",(vglobal+i)->lexName,"resd		1");
		}
		else if(!strcmp((vglobal+i)->type,"REAL"))
		{
			fprintf(f,"%-25s%s\n",(vglobal+i)->lexName,"resd		2");
		}
		else if((vp+j)->status==1){
			printingRecords(f,(vp+j)->type,(vp+j)->lexName,fglobal);		
		}
	}

	//printf("return check\n");
	fseek(f,0,SEEK_END);
	return ;
}
int addtoFields(char type[],FPtr flgobal,char lex[],char op[],int bufnum,FILE* f,int*flag)
{
	VPtr vg=flgobal->v;
	int j,k,temp;
	for(j=0;j<vg->size;j++)
	{
		if(!strcmp((vg+j)->lexName,type))
		{
			RPtr rp=(vg+j)->r;
			temp=0;
			for(k=0;k<vg->size;k++)
			{
				if(!strcmp((rp+k)->type,"INT"))
				{
					if(*flag==1)
					{
						fprintf(f, "push eax\n mov eax,[%s.%s]\n",lex,(rp+k)->lexName );
						fprintf(f,"mov [bo0%d+%d],eax\n",bufnum,4*temp);
						fprintf(f, "pop eax\n");
					}
					else
					{
						fprintf(f, "push eax\npush ebx\nmov eax,[%s.%s]\n mov ebx,[bo0%d+%d]\n",lex,(rp+k)->lexName,bufnum,4*temp );
						if(strcmp(op,"-")==0)
							fprintf(f, "sub ebx,eax\n");
						else
							fprintf(f, "add ebx,eax\n");
						fprintf(f,"mov [bo0%d+%d],ebx\n",bufnum,4*temp);
						fprintf(f, "pop ebx\npop eax\n");
					}
					temp++;
				}
			}
		}
	}
	return temp;
}
void storeFields(FILE* f,char type[],FPtr flgobal,char lex[])
{
	VPtr vg=flgobal->v;
	int j,k;
	for(j=0;j<vg->size;j++)
	{
		if(!strcmp((vg+j)->lexName,type))
		{
			RPtr rp=(vg+j)->r;
			int temp=0;
			for(k=0;k<vg->size;k++)
			{
				if(!strcmp((rp+k)->type,"INT"))
				{
					fprintf(f, "push eax\n mov eax,[bo1+%d]\n",4*temp );
					fprintf(f,"mov [%s.%s],eax\n",lex,(rp+k)->lexName);
					fprintf(f, "pop eax\n" );
					temp++;
				}
			}
		}
	}
}
void genAE(treeNode *ast,FILE* fp,int regVal,int* rec,FPtr fglobal,int* con)
{
	char VARIABLE[50];
	char reg[25];
	char reg1[25];
	char buf1[25];
	char buf0[25];
	if(regVal==1){
		strcpy(reg,"eax");
		strcpy(reg1,"ebx");
		strcpy(buf1,"bo1");
		strcpy(buf0,"bo0");
	}
	else{
		strcpy(reg,"ebx");
		strcpy(reg1,"eax");
		strcpy(buf1,"bo0");
		strcpy(buf0,"bo1");
	}
	if((!strcmp(ast->NodeSymbol,"TK_ID") || !strcmp(ast->NodeSymbol,"TK_NUM") || !strcmp(ast->NodeSymbol,"TK_RNUM")) )
	{
		if(!strcmp(ast->NodeSymbol,"TK_ID"))
		{
			if(!strcmp(ast->SymPtr->type,"INT")||!strcmp(ast->SymPtr->type,"REAL")){
				strcpy(VARIABLE,"[");
				strcat(VARIABLE,(ast->lexeCurNode));
				strcat(VARIABLE,"]");
				fprintf(fp, "\tadd %s,%s\n",reg,VARIABLE );
			}
			else
			{
				*rec=1;
				int as=1;
				char str[3];
				strcpy(str,"+");
				int p=addtoFields(ast->SymPtr->type,fglobal,ast->lexeCurNode,str,regVal,fp,&as);
				*con=p;
				printf("doneee\n");
			}
		}
		else
		{
			strcpy(VARIABLE,(ast->lexeCurNode));	
			fprintf(fp, "\tadd %s,%s\n",reg,VARIABLE );
		}
		
		fseek(fp,0,SEEK_END);
	}
	if(!strcmp((ast->NodeSymbol),"TK_MINUS"))
	{
		if(!strcmp(ast->next->NodeSymbol,"TK_ID"))
		{
			if(!strcmp(ast->next->SymPtr->type,"INT")||!strcmp(ast->next->SymPtr->type,"REAL")){
				strcpy(VARIABLE,"[");
				strcat(VARIABLE,(ast->next->lexeCurNode));
				strcat(VARIABLE,"]");
				fprintf(fp, "\tsub %s,%s\n",reg,VARIABLE );
			}
			else
			{
				*rec=1;
				int as=0;
				char str[3];
				strcpy(str,"-");
				int p=addtoFields(ast->next->SymPtr->type,fglobal,ast->next->lexeCurNode,str,regVal,fp,&as);
				*con=p;
			}
		}
		else if((!strcmp(ast->next->NodeSymbol,"TK_NUM")) || (!strcmp(ast->next->NodeSymbol,"TK_RNUM")) ){
			strcpy(VARIABLE,(ast->next->lexeCurNode));
			fprintf(fp, "\tsub %s,%s\n",reg,VARIABLE );
		}
		else if(!strcmp(ast->next->NodeSymbol,"<arithmeticExpression1>")||!strcmp(ast->next->NodeSymbol,"<arithmeticExpression2>"))
		{
			genAE(ast->next,fp,!regVal,rec,fglobal,con);
			if(*rec==1)
			{
				int i;
				for(i=0;i<*con;i++)
				{
					fprintf(fp, "push ecx\n mov ecx,[%s+%d]\n",buf1,(4*i) );
					fprintf(fp, "sub ecx,[%s+%d]\n",buf0,(4*i) );
					fprintf(fp, "mov [%s+%d],ecx\n pop ecx\n",buf1,(4*i));
				}
			}
			else
				fprintf(fp, "\tsub %s,%s\n",reg,reg1);

		}
		fseek(fp,0,SEEK_END);
	}
	if(!strcmp((ast->NodeSymbol),"TK_PLUS"))
	{
		if(!strcmp(ast->next->NodeSymbol,"TK_ID")){
			printf("in here\n");
			if(!strcmp(ast->next->SymPtr->type,"INT")||!strcmp(ast->next->SymPtr->type,"REAL")){
				printf("reeeeee\n");
				strcpy(VARIABLE,"[");
				strcat(VARIABLE,(ast->next->lexeCurNode));
				strcat(VARIABLE,"]");
				fprintf(fp, "\tadd %s,%s\n",reg,VARIABLE );
			}
			else
			{
				*rec=1;
				int as=0;
				char str[3];
				strcpy(str,"+");
				fseek(fp,0,SEEK_END);
				printf("in\n");
				addtoFields(ast->next->SymPtr->type,fglobal,ast->next->lexeCurNode,str,regVal,fp,&as);
				printf("out\n");
			}
		}
		else if((!strcmp(ast->next->NodeSymbol,"TK_NUM")) || (!strcmp(ast->next->NodeSymbol,"TK_RNUM")) ){
			strcpy(VARIABLE,(ast->next->lexeCurNode));
			fprintf(fp, "\tadd %s,%s\n",reg,VARIABLE );
		}
		else if(!strcmp(ast->next->NodeSymbol,"<arithmeticExpression1>")||!strcmp(ast->next->NodeSymbol,"<arithmeticExpression2>"))
		{
			genAE(ast->next,fp,!regVal,rec,fglobal,con);
			if(*rec==1)
			{
				int i;
				for(i=0;i<*con;i++)
				{
					fprintf(fp, "push ecx\n mov ecx,[%s+%d]\n",buf1,(4*i) );
					fprintf(fp, "add ecx,[%s+%d]\n",buf0,(4*i) );
					fprintf(fp, "mov [%s+%d],ecx\n pop ecx\n",buf1,(4*i));
				}
			}
			else
				fprintf(fp, "\tadd %s,%s\n",reg,reg1);

		}
		fseek(fp,0,SEEK_END);
	}
	if(!strcmp((ast->NodeSymbol),"TK_MUL"))
	{
		if(!strcmp(ast->next->NodeSymbol,"TK_ID"))
		{
			strcpy(VARIABLE,"[");
			strcat(VARIABLE,(ast->next->lexeCurNode));
			strcat(VARIABLE,"]");
			fprintf(fp, "\timul %s,%s\n",reg,VARIABLE );
		}
		else if((!strcmp(ast->next->NodeSymbol,"TK_NUM")) || (!strcmp(ast->next->NodeSymbol,"TK_RNUM")) ){
			strcpy(VARIABLE,(ast->next->lexeCurNode));
			if(*rec==1)
			{
				int i;
				for(i=0;i<*con;i++)
				{
					fprintf(fp, "push ecx\n mov ecx,[%s+%d]\n",buf1,(4*i) );
					fprintf(fp, "imul ecx,%s\n",VARIABLE );
					fprintf(fp, "mov [%s+%d],ecx\n pop ecx\n",buf1,(4*i));
				}
			}
			else
				fprintf(fp, "\timul %s,%s\n",reg,VARIABLE );

		}
		else if(!strcmp(ast->next->NodeSymbol,"<factorExpression>"))
		{
			printf("nooooooooooo3333\n");
			genAE(ast->next,fp,!regVal,rec,fglobal,con);
			fprintf(fp, "\timul %s,%s\n",reg,reg1);

		}
		fseek(fp,0,SEEK_END);
	}
	if(!strcmp((ast->NodeSymbol),"TK_DIV"))
	{
		if(!strcmp(ast->next->NodeSymbol,"TK_ID"))
		{
			strcpy(VARIABLE,"[");
			strcat(VARIABLE,(ast->next->lexeCurNode));
			strcat(VARIABLE,"]");
			fprintf(fp, "\tpush ecx\n" );
			fprintf(fp, "\tmov ecx,%s\n",VARIABLE );
			if(regVal==1)
			{
				fprintf(fp, "\tcdq\n" );
				fprintf(fp, "\tidiv ecx\n");
				fprintf(fp,"\tpop ecx\n" );
			}
			else{
				fprintf(fp, "\tpush eax\n" );
				fprintf(fp, "\tmov eax,ebx\n" );
				fprintf(fp, "\tcdq\n" );
				fprintf(fp,"\tidiv ecx\n" );
				fprintf(fp, "\tmov ebx,eax\n" );
				fprintf(fp, "\tpop eax\n" );
				fprintf(fp, "\tpop ecx\n" );
			}
			//fprintf(fp, "\tdiv %s,%s\n",reg,VARIABLE );
		}
		else if((!strcmp(ast->next->NodeSymbol,"TK_NUM")) || (!strcmp(ast->next->NodeSymbol,"TK_RNUM")) ){
			strcpy(VARIABLE,(ast->next->lexeCurNode));
			fprintf(fp, "\tpush ecx\n" );
			fprintf(fp, "\tmov ecx,%s\n",VARIABLE );
			if(regVal==1)
			{
				fprintf(fp, "\tcdq\n" );
				fprintf(fp, "\tidiv ecx\n");
				fprintf(fp,"\tpop ecx\n" );
			}
			else{
				fprintf(fp, "\tpush eax\n" );
				fprintf(fp, "\tmov eax,ebx\n" );
				fprintf(fp, "\tcdq\n" );
				fprintf(fp,"\tidiv ecx\n" );
				fprintf(fp, "\tmov ebx,eax\n" );
				fprintf(fp, "\tpop eax\n" );
				fprintf(fp, "\tpop ecx\n" );
			}
		}
		else if(!strcmp(ast->next->NodeSymbol,"<factorExpression>"))
		{
			genAE(ast->next,fp,!regVal,rec,fglobal,con);
			if(regVal==1)
			{
				fprintf(fp, "\tidiv ebx\n");
			}
			else{
				fprintf(fp, "\tpush eax\n" );
				fprintf(fp, "\tpush ecx\n" );
				fprintf(fp, "\tmov ecx,ebx\n" );
				fprintf(fp, "\tmov ebx,eax\n" );
				fprintf(fp,"\tmov eax,ecx\n" );
				fprintf(fp, "\tcdq\n" );
				fprintf(fp, "\tidiv ebx\n" );
				fprintf(fp, "\tmov ebx,eax\n" );
				fprintf(fp, "\tpop ecx\n" );
				fprintf(fp, "\tpop eax\n" );
			}
		}
		fseek(fp,0,SEEK_END);
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		genAE(child,fp,regVal,rec,fglobal,con);
		if(!strcmp((child->NodeSymbol),"TK_MINUS")||!strcmp((child->NodeSymbol),"TK_PLUS")||!strcmp((child->NodeSymbol),"TK_MUL")||!strcmp((child->NodeSymbol),"TK_DIV"))
			child=child->next->next;
		else
			child=child->next;
	}

}

void relOpGen(char operator[],FILE *f,int regVal)
{
	//LabelNo++;
	char reg[22];

		if(regVal){
			strcpy(reg,"dl");
		}
		else{
			strcpy(reg,"cl");
		}
	
	if(!strcmp(operator,"TK_LT"))
	{
		fprintf(f, "\tsetl %s\n",reg);
	}
	else if(!strcmp(operator,"TK_LE"))
	{
		fprintf(f,"\tsetle %s\n",reg);
	}
	else if(!strcmp(operator,"TK_GT"))
	{
		fprintf(f, "\tsetg %s\n",reg);
	}	
	else if(!strcmp(operator,"TK_GE"))
	{
		fprintf(f, "\tsetge %s\n",reg);
	}
	else if(!strcmp(operator,"TK_EQ"))
	{
		fprintf(f, "\tsete %s\n",reg );
	}
	else if(!strcmp(operator,"TK_NE"))
	{
		fprintf(f, "\tsetne %s\n",reg);
	}
}
void genConditional1(treeNode* ast,FILE *fp,int regVal)
{	char reg1[5];
	char reg2[5];
	if(regVal)
	{
		strcpy(reg1,"dl");
		strcpy(reg2,"cl");
	}
	else
	{
		strcpy(reg1,"cl");
		strcpy(reg2,"dl");
	}
	if(!strcmp(ast->NodeSymbol,"TK_ID")&&ast->next!=NULL){
		fprintf(fp, "\tmov eax,[%s]\n", ast->lexeCurNode);
		if(!strcmp(ast->next->next->NodeSymbol,"TK_ID")){
			fprintf(fp, "\tmov ebx,[%s]\n",ast->next->next->lexeCurNode );
		}
		else{
			fprintf(fp, "\tmov ebx,%s\n",ast->next->next->lexeCurNode );
		}
		fprintf(fp, "\tcmp eax,ebx\n");
		relOpGen(ast->next->NodeSymbol,fp,regVal);
		return;
	}
	else if((!strcmp(ast->NodeSymbol,"TK_NUM")||!strcmp(ast->NodeSymbol,"TK_RNUM"))&&ast->next!=NULL){
		fprintf(fp, "\tmov eax,%s\n", ast->lexeCurNode);
		if(!strcmp(ast->next->next->NodeSymbol,"TK_ID")){
			fprintf(fp, "\tmov ebx,[%s]\n",ast->next->next->lexeCurNode );
		}
		else{
			fprintf(fp, "\tmov ebx,%s\n",ast->next->next->lexeCurNode );
		}
		fprintf(fp, "\tcmp eax,ebx\n");
		relOpGen(ast->next->NodeSymbol,fp,regVal);
		//fprintf(stderr, "%s\n", );
		return;
	}
	if(!strcmp(ast->NodeSymbol,"TK_AND"))
	{
		if(!strcmp(reg2,"cl")){
			fprintf(fp, "\tmovsx ecx,%s\n",reg2 );	
			fprintf(fp, "\tpush ecx\n" );
		}
		else{
			fprintf(fp, "\tmovsx edx,%s\n",reg2 );
			fprintf(fp, "\tpush edx\n" );
		}
		
		genConditional1(ast->next,fp,!regVal);
		fprintf(fp,"and %s,%s\n",reg1,reg2);
		if(!strcmp(reg2,"cl")){
			fprintf(fp, "\tpop ecx\n" );
		}
		else{
			fprintf(fp, "\tpop edx\n" );
		}	
	}
	if(!strcmp(ast->NodeSymbol,"TK_OR"))
	{
		if(!strcmp(reg2,"cl")){
			fprintf(fp, "\tmovsx ecx,%s\n",reg2 );	
			fprintf(fp, "\tpush ecx\n" );
		}
		else{
			fprintf(fp, "\tmovsx edx,%s\n",reg2 );
			fprintf(fp, "\tpush edx\n" );
		}
		
		genConditional1(ast->next,fp,!regVal);
		fprintf(fp,"or %s,%s\n",reg1,reg2);
		if(!strcmp(reg2,"cl")){
			fprintf(fp, "\tpop ecx\n" );
		}
		else{
			fprintf(fp, "\tpop edx\n" );
		}
	}
	if(!strcmp(ast->NodeSymbol,"TK_NOT")){
		genConditional1(ast->next,fp,!regVal);
		fprintf(fp, "\txor %s,1\n",reg2 );	
	}

	treeNode* child=ast->child;
	while(child!=NULL)
	{
		genConditional1(child,fp,regVal);
		if(!strcmp(child->NodeSymbol,"TK_AND")||!strcmp(child->NodeSymbol,"TK_OR")||!strcmp(child->NodeSymbol,"TK_NOT"))
			return;
		child=child->next;
	}	
}


void forAssign(treeNode* ast,SPtr sptr,FILE *fp,FPtr fglobal){
	fprintf(fp,"%s\n","mov	eax,0" );
	fprintf(fp,"%s\n","mov	ebx,0" );
			fseek(fp,0,SEEK_END);

	printf("%s\n","mov	eax,0" );

	char recfield[256]; //accomodates id and recordid
	//if(ast->prev==NULL)printf("kuchh\n");
	if(!strcmp(ast->prev->NodeSymbol,"TK_ID")){
	//	printf("55\n");
		strcpy(recfield,ast->prev->lexeCurNode);
	}
	else{
		strcpy(recfield,ast->prev->child->lexeCurNode);
		strcat(recfield,".");
		strcat(recfield,ast->prev->child->next->lexeCurNode);
	}
	//printf("\tmov %s,eax\n", recfield);
	int rec=0;
	int con=0;
	genAE(ast->next,fp,1,&rec,fglobal,&con);
	//printf("lalallala\n");
	if(rec==0)
		fprintf(fp, "\tmov [%s],eax\n", recfield);
	else
	{
		storeFields(fp,ast->prev->SymPtr->type,fglobal,ast->prev->lexeCurNode);
	}
}
void readGen(treeNode* ast, FILE* fp)
{
	/*mov eax, 3		 ; 3 is recognized by the system as meaning "read"
mov ebx, 0		 ; read from standard input
mov ecx, variable        ; address to pass to
mov edx, 1		 ; input length (one byte)
int 0x80 */

	char LEXEME[100];
	if(!strcmp((ast->NodeSymbol),"TK_ID")){
		strcpy(LEXEME,(ast->lexeCurNode));
		fprintf(fp,"\tcall _stdin1\n\tmov [%s],eax\n\t",LEXEME);
	}
	else{
		ast=ast->child;
		strcpy(LEXEME,(ast->lexeCurNode));
		strcat(LEXEME,".");
		strcat(LEXEME,(ast->next->lexeCurNode));
		fprintf(fp,"\tcall _stdin1\n\tmov [%s],eax\n\t",LEXEME);
	}
	return ;
}

void writeGen(treeNode* ast, FILE* fp,FPtr fglobal)
{
	/*mov eax, 3		 ; 3 is recognized by the system as meaning "read"
mov ebx, 0		 ; read from standard input
mov ecx, variable        ; address to pass to
mov edx, 1		 ; input length (one byte)
int 0x80 */

	char LEXEME[100];
	if(!strcmp((ast->NodeSymbol),"TK_ID")){
		
		//printf("haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
		if(!strcmp(ast->SymPtr->type,"INT")||!strcmp(ast->SymPtr->type,"REAL")||!strcmp(ast->SymPtr->type,"TK_NUM")||!strcmp(ast->SymPtr->type,"TK_RNUM"))
		{
			//printf("haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			strcpy(LEXEME,(ast->lexeCurNode));
			fprintf(fp,"\tmov eax,[%s]\n\tcall _stdout1\n\t",LEXEME);
			fseek(fp,0,SEEK_END);
		}
		else{
			//printf("haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
			FPtr fp1=fglobal;
			int j,k;
			VPtr vp;
			vp=fp1->v;
			for(j=0;j<fp1->size;j++)
			{
				if( !strcmp((vp+j)->lexName,ast->SymPtr->type)  ){	
					//printf("%s %s 1111111\n", (vp+j)->lexName,ast->SymPtr->type);
					RPtr rp=(vp+j)->r;
					for (k = 0; k < ast->SymPtr->size; k++)
					{
						//printf("^^^^^^^^^^^^^^^^^^^^^^ %s\n",(rp+k)->type);	
						if(!strcmp((rp+k)->type,"INT")){
							//printf("--------------------------------------------------------------------\n");
							strcpy(LEXEME,(ast->lexeCurNode));
							strcat(LEXEME,".");
							strcat(LEXEME,(rp+k)->lexName);
							//printf("!!!!!!!!!!!!!!!!!!\n");
							fprintf(fp,"\tmov eax,[%s]\n\tcall _stdout1\n\t",LEXEME);
							//printf("!!!!!!!!!!!!!!!!!!\n");
						}
					}
				}		
			}
		}
	}
	else{
		ast=ast->child;
		strcpy(LEXEME,(ast->lexeCurNode));
		strcat(LEXEME,".");
		strcat(LEXEME,(ast->next->lexeCurNode));
		fprintf(fp,"\tmov eax,4\n\tmov ebx,1\n\tmov ecx,%s\n\tmov edx,2\nint 0x80\n",LEXEME);
	}
	return ;
}



void iterating(treeNode *ast,FILE *fp){
	if(!strcmp(ast->NodeSymbol,"TK_WHILE")){
		fprintf(fp, "\tLabel%d: ",LabelNo++ );
	}
	else if(!strcmp(ast->NodeSymbol,"<booleanExpression>")){
		genConditional1(ast,fp,1);
		fprintf(fp,"je Label%d\n",LabelNo );

	}

	treeNode* child=ast->child;
	while(child!=NULL)
	{
		iterating(child,fp);
		child=child->next;
	}
}


void printingStmts(treeNode* ast,SPtr sptr,FILE *fp,FPtr fglobal){
	printf("%d\n", ast->lineno);
	if(!strcmp(ast->NodeSymbol,"TK_ENDWHILE"))
	{	fprintf(fp, "\tJMP Label%d\n",LabelNo-1 );
		fprintf(fp, "\tLabel%d: ",LabelNo );
	}
	if(!strcmp(ast->NodeSymbol,"TK_ELSE"))
	{
		fprintf(fp, "\tJMP Label%d\n",++LabelNo );
		fprintf(fp, "\tLabel%d: ",LabelNo-1 );
	}
	if(!strcmp(ast->NodeSymbol,"TK_ENDIF"))
				fprintf(fp, "\tLabel%d:",LabelNo);		
	if(strcmp(ast->NodeSymbol,"TK_ASSIGNOP")==0)
	{
		//printf("koi mil gaya %s \n",ast->parent->NodeSymbol);
		forAssign(ast,sptr,fp,fglobal);
	}
	if(!strcmp(ast->NodeSymbol,"<ioStmt>"))
	{	
		if(!strcmp(ast->child->NodeSymbol,"TK_READ")){
			readGen(ast->child->next,fp);
		}
		else{
			writeGen(ast->child->next,fp,fglobal);
			//printf("chbjjjjjjjjjjjjjjjjjjjj\n");
		}
	}
	if(!strcmp(ast->NodeSymbol,"<iterativeStmt>")){
		iterating(ast,fp);
	}	
	if(!strcmp(ast->NodeSymbol,"<conditionalStmt>")){
		//printf("hhhee\n");
		int tem=1;
		genConditional1(ast->child->next,fp,1);
		fprintf(fp,"je Label%d\n",LabelNo );
	}
	treeNode* child=ast->child;
	while(child!=NULL)
	{
		printingStmts(child,sptr,fp,fglobal);
		child=child->next;
	}	
}


void printingText(treeNode* ast,SPtr sptr,FILE *fp,FPtr fglobal){
	//if(ast==NULL)
	printf("cbbshjsj %s11\n ",ast->NodeSymbol);
	if(!strcmp(ast->NodeSymbol,"<mainFunction>")){
		printf("nananan111\n");
		printingStmts(ast,sptr,fp,fglobal);
		return;
	}

	treeNode* child=ast->child;
	printf("kyonnnnnnnnnnnn\n");
	while(child!=NULL)
	{
		printf("%s --------- \n",child->NodeSymbol );
		printingText(child,sptr,fp,fglobal);
		child=child->next;
	}
	printf("1111111111111111111111111\n");
}


void NASM(treeNode* ast,SPtr sptr,FILE *fp,FPtr fglobal)
{
		if(fp!=NULL)
			printf("djdjdjdjjjjjjjjjj\n");
		fprintf(fp, "%%include 'auxillary.asm'\n" );
		fprintf(fp,"BITS 32\nGLOBAL _start\n");
		//printingRecords(sptr,fp,fglobal);
		fprintf(fp,"SECTION\t.bss\n");
		
		variablesHandle(sptr,fp,fglobal);
		
		fprintf(fp, "bo1 resd 255\n" );
		fprintf(fp, "bo0 resd 255\n" );
		fseek(fp,0,SEEK_END);
		fprintf(fp,"SECTION\t.text\n_start:\n");
		fseek(fp,0,SEEK_END);
		printingText(ast,sptr,fp,fglobal);
		printf("nananan\n");
		fseek(fp,0,SEEK_END);
		printf("not seeknig\n");
	    fprintf(fp, "\tmov eax,1\n\t mov ebx,0\n\t int 80h\n");
	    printf("not seeknig\n");
	    fseek(fp,0,SEEK_END);
	    printf("not seeknig\n");
}