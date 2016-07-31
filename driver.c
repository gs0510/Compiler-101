/*
		BATCH 10
		Gargi Sharma 2013A7PS161P
		Ayushi Agarwal 2013A7PS065P

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "lexerDef.h"
#include "symbolTableDef.h"
#include "parserDef.h"
//#include "semanticAnalyzer.c"

int lineno;

void printCode(char* testcase)
{
	FILE* fp=fopen(testcase,"r");
	char temp[512];
	while(fgets(temp,sizeof(temp),fp)!=NULL)
	{
		int i=0;
		while(temp[i]!='\0'&&temp[i]!='%')
		{
			printf("%c", temp[i]);
			i++;
		}
	}
	fclose(fp);
}

int main(int argc,char* argv[]){
	/*
	printf("FIRST and FOLLOW set automated\n");

    printf("Both lexical and syntax analysis modules implemented\n");

    printf("Modules work  with all the testcases\n");

    printf("Parse tree constructed successfully\n");

    printf("Error recovery has been implemented\n");
    */
	int uugs;
	char tuu[512];
	strcpy(tuu,argv[1]);
	int k1;
	//tree* ast;
	
	while(1)
	{
		printf("Enter buffer size\n");
		scanf("%d",&k1);
		if(k1!=1){
			break;
		}
	}
	popGram("grammar.txt");
	popFirsts();
	popFollows();
	createParseTable(ru);
	populatesyncset();
	while(1){
		printf("Enter your choice (press 0 to exit)\n");
		scanf("%d",&uugs);
		if(uugs==0) return;
		if(uugs==1){	
			lineno=1;
			tokenInfo * temp=ABC(tuu,k1);
			while(temp!=NULL){
				printf("%s\t%s\t%d\n",temp->token,temp->lexeme,temp->lineno);
				temp = temp->next;
			}
		}
		else if(uugs==2){
			lineno=1;
			tokenInfo * temp = ABC(tuu,k1);
			int demp=0;
			tree*t =parseInputSourceCode(temp,&demp);
			if(demp){
				printParseTree(t);
			}
			else{
				printf("Parse tree cannot be made. Correct the syntax of your program\n");
			}
		}
		else if(uugs==3){
			lineno=1;
			tokenInfo * temp = ABC(tuu,k1);
			int correctParseTree=0;
			tree* ast=parseInputSourceCode(temp,&correctParseTree);
			if(correctParseTree)
			{
				createAST(ast);
				printParseTree(ast);
			}
			else
			{
				printf("Abstract Syntax tree cannot be made. Correct the syntax of your program\n");
			}
			
		}
		else if (uugs==4){
			int ptnum=0,astnum=0;
			int psize=0,astsize=0;
			lineno=1;
			tokenInfo * temp = ABC(tuu,k1);
			int correctParseTree=0;
			tree* ast=parseInputSourceCode(temp,&correctParseTree);
			ptnum=countParseTree(ast);
			if(correctParseTree)
			{
				createAST(ast);
				astnum=countParseTree(ast);
				psize=ptnum*sizeof(treeNode);
				astsize=astnum*sizeof(treeNode);
				printf("Parse tree		Number of nodes = %d    Allocated Memory =  %d Bytess\n",ptnum,psize );
				printf(" AST            Number of nodes = %d    Allocated Memory =  %d Bytes\n",astnum,astsize );
				printf("Compression Percentage: %0.2f\n", ((psize-astsize)*1.0/psize)*100);
			}
			else
			{
				printf("Abstract Syntax tree cannot be made. Correct the syntax of your program\n");
			}
		}
		else if(uugs==5){
			tokenInfo * temp = ABC(tuu,k1);
			int correctParseTree=0;
			tree* ast=parseInputSourceCode(temp,&correctParseTree);
			if(correctParseTree)
			{
				createAST(ast);
				//astnum=printParseTree(ast);
				SPtr symPtr=initializeSymbolTable();
				FPtr fglobal=globalTable(symPtr);
				createSymbolTable(ast->root,symPtr,fglobal);
				printSymbolTable(symPtr);
			}
		}
		else if(uugs==6){
			lineno=1;
			tokenInfo * temp = ABC(tuu,k1);
			int correctParseTree=0;
			tree* ast=parseInputSourceCode(temp,&correctParseTree);
			if(correctParseTree)
			{
				createAST(ast);
				//astnum=printParseTree(ast);
				SPtr symPtr=initializeSymbolTable();
				FPtr fglobal=globalTable(symPtr);
				createSymbolTable(ast->root,symPtr,fglobal);
				if(semanticChecks(ast->root,symPtr,fglobal)){
					printf("Compiled successfully. No errors in the code.\n");
				}
			}
			//printf("kjacsna\n");
		}
		else if(uugs==7){
			//printf("naskansknkasdk\n");
			lineno=1;
			//printf("nack\n");
			strcpy(tuu,argv[1]);
			tokenInfo * temp = ABC(tuu,k1);
			int correctParseTree=0;
			tree* ast=parseInputSourceCode(temp,&correctParseTree);
			//printf("nack111\n");
			if(correctParseTree)
			{
				createAST(ast);
				//astnum=printParseTree(ast);
				//printf("nack333\n");
				SPtr symPtr=initializeSymbolTable();
				FPtr fglobal=globalTable(symPtr);
				printf("nack555\n");
				createSymbolTable(ast->root,symPtr,fglobal);
				int semCorrect=semanticChecks(ast->root,symPtr,fglobal);
				if(semCorrect){
					printf("jhjhjttft\n");
					createAssemblyCode(argv[2],ast->root,symPtr,fglobal);
					printf("Assembly Code Created in the file %s.\n",argv[2] );
				}
				else{
					printf("ERRORS exist in your code. Please correct them before generating assembly code.\n");
				}
			}
		}	
		else{
			printf("Incorrect choice. Please enter again.\n");
		}
	}
	return 0;
}