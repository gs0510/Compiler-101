/*
		BATCH 10
		Gargi Sharma 2013A7PS161P
		Ayushi Agarwal 2013A7PS065P

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "stack.c"
#include "parserDef.h"

//int correctParseTree=0;

char* term[]={"eps","TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RECORDID","TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_INT","TK_REAL","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT","TK_OUTPUT","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD","TK_ENDRECORD","TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_NE","TK_GE","TK_TYPE","$","TK_COMMA"};
char* nterm[]={"<program>","<mainFunction>","<otherFunctions>","<function>","<input_par>","<output_par>","<parameter_list>","<dataType>","<primitiveDatatype>","<constructedDatatype>","<remaining_list>","<stmts>","<typeDefinitions>","<typeDefinition>","<fieldDefinitions>","<fieldDefinition>","<moreFields>","<declarations>","<declaration>","<global_or_not>","<otherStmts>","<stmt>","<assignmentStmt>","<SingleOrRecId>","<isRecord>","<funCallStmt>","<outputParameters>","<inputParameters>","<iterativeStmt>","<conditionalStmt>","<continue_else>","<ioStmt>","<allVar>","<arithmeticExpression>","<arithmeticExpression2>","<arithmeticExpression1>","<arithmeticExpression3>","<factorExpression>","<operator1>","<operator2>","<booleanExpression>","<var>","<logicalOp>","<relationalOp>","<returnStmt>","<optionalReturn>","<idList>","<more_ids>"};

int errorflag=0;


void insertChild(tree* t,treeNode* tn);
void insertSibling(tree* t,treeNode* tn);

int getRow(char* temp){
	int i;
	for(i=0;i<NUM_NONTERMINALS;i++){
		if(strcmp(temp,nterm[i])==0) return i;
	}
	return -1;
}

int getColumn(char* temp){
	int i;
	for(i=0;i<NUM_TERMINALS;i++){
		if(strcmp(temp,term[i])==0) return i;
	}
	return -1;
}

Fhead* searchFirst(char *name){
	int i=0;
	for(i=0;i<(TOT_RULES+NUM_TERMINALS);i++){
		if(strcmp((h+i)->nonTerminal,name)==0){
			return (h+i);
		}	
	}
	//if(i==TOT_RULES){
	//	return NULL;
	//}
}

Fhead* searchFollow(char *name){
	int i=0;
	for(i=0;i<TOT_RULES;i++){
		if(strcmp((fo+i)->nonTerminal,name)==0){
			return (fo+i);
		}	
	}
}

void printParseTable(){
	int i,j;
	for(i=0;i<NUM_NONTERMINALS;i++){
		for(j=0;j<NUM_TERMINALS;j++){
			printf("%d ",table[i][j]);
		}
		printf("\n");
	}
}

void createParseTable(rule* ru)
{
	memset(table,-1,sizeof(table));
	int i=0;
	for(i=0;i<TOT_RULES;i++){
		char temp[30];
		//printf("%d rule no\n", i);
		strcpy(temp,(ru+i)->lNode->name);
		int rowNo = getRow(temp);
		//printf("\n%s  %d\n ",temp,rowNo );
		struct node* fn = (ru+i)->front;
		//printf("%s %s\n", temp,fn->name);
		Fhead* ftemp = 	searchFirst(fn->name);
		//printf("%s knnk\n",fn->name);
		Fnode* f = ftemp->head;		
		
		while(f!=NULL){
			//printf("%d rule no\n", i);
			int colNo = getColumn(f->terminal);
			//printf(" %s %d colNo \n",f->terminal,colNo );
			table[rowNo][colNo] =i;
			f=f->next;
		}
		if(strcmp(fn->name,"eps")==0)
		{
			Fhead* ftemp = searchFollow((ru+i)->lNode->name);
			Fnode* f = ftemp->head;
			while(f!=NULL){
				int colNo = getColumn(f->terminal);
				//printf(" %s %d colNo ",f->terminal,colNo );
				table[rowNo][colNo] =i;
				f=f->next;
			}	
		}
		if(ftemp->eflag==1){
			Fhead* ftemp = searchFollow((ru+i)->front->name);
			Fnode* f = ftemp->head;
			while(f!=NULL){
				int colNo = getColumn(f->terminal);
				//printf(" %s %d colNo ",f->terminal,colNo );
				table[rowNo][colNo] =i;
				f=f->next;
			}	
		}
	}
	//printParseTable();
}

void popGram(char* filename){
	ru = (rule*)malloc(TOT_RULES*sizeof(rule));
	ru->lNode = ru->front = ru->rear = NULL;
	FILE* f = fopen(filename,"r");
	char s[1024];
	int i=0;
	while (fgets(s,sizeof(s), f))
	{
    	s[strlen(s)-1]='\0';
    	//printf("\n%s\n", s);
    	(ru+i)->rnum=i+1;
    	popRule((ru+i),s);
    	//print(ru+i);
    	i++;
	}
	fclose(f);
}


void popRule(rule *ru,char str[]){
	int count=0;
    struct node* newNode;
    char * strToken = strtok(str," ");
    while(strToken){
    	newNode = (struct node*)malloc(sizeof(struct node));
    	if(count==0){
    		strcpy(newNode->name,strToken);
            if(strcmp(strToken,"eps")==0){
    			ru->epsilon = 1;
    		}
    		else ru->epsilon = 0;
            newNode->isTerm=0;
            newNode->next=NULL;
            newNode->prev=NULL;
            ru->lNode=newNode;
    	}
    	if(count!=0 && count!=1)
        {
          	strcpy(newNode->name,strToken);
            if(strcmp(strToken,"eps")==0){
    			ru->epsilon = 1;
    		}
    		else ru->epsilon = 0;

            if(newNode->name[0]=='<')
            newNode->isTerm=0;
            else
            newNode->isTerm=1;
            newNode->next=NULL;
            if(count==2)
            {
                ru->rear=newNode;
                newNode->prev=NULL;
                ru->front=ru->rear;
                ru->lNode->next=newNode;
            }
            else
            {
                ru->rear->next=newNode;
                newNode->prev = ru->rear;
                ru->rear=newNode;
            }
        }
        strToken = strtok(NULL, " ");
        count++;
    }
}



Fnode *search(Fnode *list,char* val)
{
	Fnode *ptr;
	for(ptr=list;ptr!=NULL && strcmp(ptr->terminal,val)!=0;ptr=ptr->next);
	return(ptr);
}

Fnode *findlast(Fnode *list)
{
	Fnode *ptr;
	for(ptr=list;ptr->next!=NULL;ptr=ptr->next);
	return(ptr);
}


Fnode* union1(Fhead* h1,Fhead* h2)
{
	if(h1->head==NULL&&h2->head==NULL)
		return NULL;
	Fnode *temp,*ptr1,*ptr2,*list3=NULL;
	int i,val;
	for(ptr1=h1->head;ptr1!=NULL;ptr1=ptr1->next)
	{
		temp=(Fnode* )malloc (sizeof(Fnode));
		strcpy(temp->terminal,ptr1->terminal);
		//printf("%s uu\n",temp->terminal );
		temp->next=NULL;
		if(list3==NULL)
		list3=temp;
		else
		{
			ptr2=findlast(list3);
			ptr2->next=temp;
		}
	}
		for(ptr1=h2->head;ptr1!=NULL;ptr1=ptr1->next)
		{
			if(strcmp(ptr1->terminal,"eps")!=0)
			{
				ptr2=search(h1->head,ptr1->terminal);
				if(ptr2==NULL)
				{
					temp=(Fnode* )malloc (sizeof(Fnode));
					strcpy(temp->terminal,ptr1->terminal);
					//printf("%s ua\n",temp->terminal );
					temp->next=NULL;
					if(list3==NULL)
						list3=temp;
					else
					{					
						ptr2=findlast(list3);
						ptr2->next=temp;
					}
				}
			}
		}
	//printf("ggg\n");
	return(list3);
}
	
Fhead* automateFirst(int i,Fhead* h){
    //printf("bcsjds\n");
    if(h[i].isCal==1)
        return h+i;
    else{
    	strcpy((h+i)->nonTerminal,nterm[i]);
    	int jk;
    	for(jk=0;jk<TOT_RULES;jk++){
    		if(strcmp((ru+jk)->lNode->name,nterm[i])==0)
    		{
    			struct node* newNode = (ru+jk)->front;
    			while(newNode!=NULL){
		            if(newNode->isTerm==1){
		                if(strcmp(newNode->name,"eps")==0){
		                    h[i].eflag=1;
		                }
		                Fnode *temp = (Fnode*)malloc(sizeof(Fnode));
		                strcpy(temp->terminal,newNode->name);
		                temp->next=NULL;
		                Fhead* temp2=(Fhead*)malloc(sizeof(Fhead));
		                temp2->head=temp;
		                //printf("%s\n",temp->terminal );
		                h[i].head=union1(temp2,(h+i));
		                newNode=newNode->next;
		                //printf("ddd\n");
		                break;
		            }
		            else
		            {
		                int j;
		                //printf("hiiii\n");
		                for(j=0;j<NUM_NONTERMINALS;j++)
		                {

		                    if(strcmp(nterm[j],newNode->name)==0)
		                    {
		                        //printf("%s\n", nterm[j]);
		                        h[i].head=union1((h+i),automateFirst(j,h));
		                        //printf("%s\n", nterm[j]);
		                        break;
		                    }
		                }
		                if(h[j].eflag==1)
		                {
		                    newNode=newNode->next;
		                }
		                else
		                {
		                    break;
		                }
		            }   
    			}
    		}
    	}
    h[i].isCal=1;
    return h+i;
    }
}

void printF(Fhead *fh){
	Fnode* newNode = fh->head;
	printf("%s front %d\n",fh->nonTerminal,fh->eflag );
	while(newNode!=NULL){
		printf("%s ", newNode->terminal);
		newNode=newNode->next;
	}
	printf("\n");
}


void popFirsts(){
    Fnode* n;
    h =(Fhead*)malloc((NUM_NONTERMINALS+NUM_TERMINALS)*sizeof(Fhead));
    int i=0;
    for(i=0;i<NUM_NONTERMINALS+NUM_TERMINALS;i++)
    {
    	h[i].isCal=0;
    	h[i].head=NULL;
    }
    for(i=0;i<NUM_NONTERMINALS;i++)
    {
        automateFirst(i,h);
    	//printF(h+i);
    }
    for(i=NUM_NONTERMINALS;i<(NUM_NONTERMINALS+NUM_TERMINALS);i++){
    	Fnode *n= (Fnode*)malloc(sizeof(Fnode));
    	n->next =NULL;
    	strcpy(n->terminal,term[i-NUM_NONTERMINALS]);
    	h[i].head =n;
    	strcpy(h[i].nonTerminal,term[i-NUM_NONTERMINALS]);
    	h[i].eflag =0;
    	//printF(h+i);
    }
}



void populatesyncset()
{
	//printf("hihi\n");
    char str[500];
    FILE* f1=fopen("sync-set.txt","r");
    Fnode* n;
    int i=0,j=0;
    ss=malloc(TOT_RULES*sizeof(Fhead));
    while (fgets(str, sizeof(str), f1))
    {
        str[strlen(str)-1]='\0';
        int count=0;
        char* token = strtok(str, " ");
        while(token)
        {
            if(count!=1){
            n=malloc(sizeof(Fnode));
            n->next=NULL;}
            if(count==0)
            {
                strcpy((ss+i)->nonTerminal,token);
          	}
            if(count==2)
            {
                strcpy(n->terminal,token);
                (ss+i)->rear=n;
                (ss+i)->rear->next=NULL;
                (ss+i)->head=(ss+i)->rear;
            }

            if(count!=0 && count!=1)
            {
                    strcpy(n->terminal,token);
                    (ss+i)->rear->next=n;
                    (ss+i)->rear=n;
                    (ss+i)->rear->next=NULL;
                    j++;
            }
            token = strtok(NULL, " ");
            count++;
        }
       //printF((ss+i));
        i++;
    }
    fclose(f1);
}
int bitv[NUM_NONTERMINALS];
Fhead* automateFollow(int i,Fhead* hf){
	if(hf[i].isCal==1)
		return (hf+i);
	else{
		bitv[i]=1;
		strcpy((hf+i)->nonTerminal,nterm[i]);
		//printf("%s\n",nterm[i] );
    	int jk;
    	for(jk=0;jk<TOT_RULES;jk++){
			struct node *newN = (ru+jk)->front;
			//printf("%s\n",newN->name );
			while(newN!=NULL){
				if(strcmp(newN->name,hf[i].nonTerminal)==0){
					//printf("%d---\n",jk );
					newN=newN->next;
					while(newN!=NULL){
						int uu;
						for(uu=0;uu<NUM_TERMINALS+NUM_NONTERMINALS;uu++)
						{
							if(strcmp(newN->name,h[uu].nonTerminal)==0)
							{
								hf[i].head=union1(hf+i,h+uu);
								break;
							}
						}
						if(h[uu].eflag==1)
						{
							newN=newN->next;
						}
						else
							break;
					}
					if(newN==NULL)
					{
						int uu;
						for(uu=0;uu<NUM_NONTERMINALS;uu++)
						{
							if(strcmp((ru+jk)->lNode->name,nterm[uu])==0)
							{
								if(bitv[uu]!=1)
								{
									hf[i].head=union1(hf+i,automateFollow(uu,hf));
								}
								break;
							}
						}
					}
					break;
				}
				else
				{
					newN=newN->next;
				}
			}
		}
		if(hf[i].head!=NULL)
			hf[i].isCal=1;
		bitv[i]=0;
		return(hf+i);
	}
}
void popFollows(){
    Fnode* n;
    fo =(Fhead*)malloc((NUM_NONTERMINALS)*sizeof(Fhead));
    int i=0;
    for(i=0;i<NUM_NONTERMINALS;i++)
    {
    	fo[i].isCal=0;
    	fo[i].head=NULL;
    	bitv[i]=0;
    }
    n = (Fnode*)malloc(sizeof(Fnode));
    strcpy(n->terminal,"$");
    n->next=NULL;
    fo[0].isCal=1;
    fo[0].head = n;
    strcpy(fo[0].nonTerminal,"<program>");
    for(i=1;i<NUM_NONTERMINALS;i++)
    {
        automateFollow(i,fo);
    	//printF(fo+i);
    }
    
}

void print(rule *ru){
	struct node* newNode = ru->lNode;
	printf("%s front\n",ru->front->name );
	while(newNode!=NULL){
		printf("%s %d", newNode->name,newNode->isTerm);
		newNode=newNode->next;
	}
	printf("\n");
}
tree* createParseTree()
{
	tree* t= (tree*)malloc(sizeof(tree));
	t->root =NULL;
	t->front = NULL;
	t->rear =NULL;
	t->height =0;
	return t;
}

tree* parseInputSourceCode(tokenInfo *ti,int* correctParseTree)
{
	//printf("gfgfgfg\n");
	tree *t = createParseTree();
	struct stack* st=createStack();
	//printf("fdgd\n");
	push(st,"$");
	//printf("hhhh\n");
	push(st,(ru+0)->lNode->name);
	treeNode * tn = (treeNode*)malloc(sizeof(treeNode));
	tn->SymPtr=NULL;
	tn->lineno=0;
	strcpy(tn->isLeafNode,"No");
	strcpy(tn->parentNodeSymbol,"ROOT");
	tn->isTerm = 0;
	strcpy(tn->NodeSymbol,((ru+0)->lNode->name));
	//printf("hihi\n");
	insertChild(t,tn);
	tokenInfo* temp =ti;
	while(temp!=NULL&&(st->top!=NULL))
	{
		//printf("%s\n",temp->token);
		if(strcmp(temp->token,"TK_COMMENT")==0)
		{
			temp=temp->next;
			continue;
		}
		while((st->top)!=NULL){
			//printf("yeee\n");
			if(strcmp(st->top->string,"eps")==0)
			{
				treeNode* t3 = (treeNode*)malloc(sizeof(treeNode));
				strcpy(t3->lexeCurNode,"");
				strcpy(t3->NodeSymbol,"EPS");
				strcpy(t3->isLeafNode,"YES");
				t3->child=NULL;
				t3->SymPtr=NULL;
				insertChild(t,t3); 
				strcpy(t3->parentNodeSymbol,t3->parent->NodeSymbol);
				//printf("%s\n",t->front->parent->NodeSymbol );
				//printf("%s\n",t->front->parent->parent->NodeSymbol );
				t->front=t->front->parent;
				while(t->front->parent!=NULL && t->front->parent->next==NULL)
					{
						t->front = t->front->parent;
					}
					t->front = t->front->parent->next;
					pop(st);
					continue;
				}
			if(strcmp(st->top->string,temp->token)==0 && st->top->string[0]!='<'){
				pop(st);
				treeNode* t3 = (treeNode*)malloc(sizeof(treeNode));
				t3->SymPtr=NULL;
				strcpy(t3->lexeCurNode,temp->lexeme);
				t3->lineno = temp->lineno;
				strcpy(t3->token,temp->token);
				if(strcmp(t3->token,"TK_RNUM")==0 || strcmp(t3->token,"TK_NUM")==0){
					t3->valuIfNumber = atof(temp->lexeme);
				}
				strcpy(t3->isLeafNode,"YES");
				t3->isTerm =1;
				t3->child=NULL;
				strcpy(t3->NodeSymbol,temp->token);
				insertChild(t,t3);
				strcpy(t3->parentNodeSymbol,t3->parent->NodeSymbol);
				while(t->front->parent!=NULL && t->front->parent->next==NULL)
				{
					t->front = t->front->parent;
				}
				//printf("break\n");
				temp=temp->next;
				if(t->front->parent==NULL)
					break;
				t->front = t->front->parent->next;
				break;
			}
			else if(strcmp(st->top->string,temp->token)!=0 && st->top->string[0]!='<'){
				if(strcmp(temp->token,"TK_LEXERROR")==0)
				{
					errorflag=1;
					temp=temp->next;
					break;
				}
				errorflag=1;
				printf("ERROR_5: Expected token is %s. Token %s for lexeme %s doesn't match in line %d\n",st->top->string,temp->token,temp->lexeme,temp->lineno);
				pop(st);
			}
			else{
				//printf("%sjjj\n",st->top->string );
				int row = getRow(st->top->string);
				if(strcmp(temp->token,"TK_LEXERROR")==0)
				{
					errorflag=1;
					/*while(temp!=NULL){
						Fnode* t2 = (fo+row)->head;
						while(t2!=NULL){
							if(strcmp(temp->token,t2->terminal)==0||strcmp(temp->token,"TK_SEM")==0) break;
							t2 = t2->next;
						}
						if(t2==NULL){
							temp = temp->next;
						}
						else{
							break;
						}
					}
					pop(st);
					if(temp==NULL) break;*/
					temp=temp->next;
					break;
				}
				else
				{
					int col = getColumn(temp->token);
					//printf("%d %d\n",row,col );
					//printf("%d\n",table[row][col] );
					if(table[row][col]==-1)
					{
						errorflag=1;
						printf("ERROR_5: Expected token is of type %s. Token %s for lexeme %s doesn't match in line %d.\n",st->top->string,temp->token,temp->lexeme,temp->lineno);
						while(temp!=NULL){
							Fnode* t2= union1((ss+row),(fo+row));
							while(t2!=NULL){
								if(strcmp(temp->token,t2->terminal)==0) break;
								t2 = t2->next;
							}
							if(t2==NULL){
								temp = temp->next;
							}
							else{
								break;
							}
						}
						pop(st);
						if(temp==NULL) break;
					}
					else
					{
						pop(st);
						struct node *t1 =(ru+(table[row][col]))->rear;
						while(t1!=(ru+(table[row][col]))->front){
							push(st,t1->name);
							t1 = t1->prev;
							//printf("jhhff\n");
						}
						push(st,t1->name);
						struct node* uu = (ru+(table[row][col]))->front;
		            	treeNode* t4  = (treeNode*)malloc(sizeof(treeNode));
		            	t4->SymPtr=NULL;
		            	strcpy(t4->NodeSymbol,uu->name);
		            	strcpy(t4->isLeafNode,"NO");
		            	t4->isTerm =uu->isTerm;
		            	insertChild(t,t4);
		            	strcpy(t4->parentNodeSymbol,t4->parent->NodeSymbol);
		            	while(uu->next!=NULL){
		            		uu=uu->next;
		            		treeNode* t3 = (treeNode*) malloc(sizeof(treeNode));
		            		t3->SymPtr=NULL;
		            		t3->isTerm =uu->isTerm;
		            		strcpy(t3->isLeafNode,"NO");
		            		strcpy(t3->NodeSymbol,uu->name);
		            		insertSibling(t,t3);
		            		strcpy(t3->parentNodeSymbol,t3->parent->NodeSymbol);
		            	}
						//printf("outss\n");
					}

				}
				
			}
		}
	}
	if(!errorflag &&temp==NULL&&st->top!=NULL&&strcmp(st->top->string,"$")==0)
	{
		//printf("Compiled successfully\n");
		*correctParseTree=1;
	}
	else{
		while(st->top!=NULL&&strcmp(st->top->string,"$")!=0)
		{	if(st->top->string[0]!='<')
				printf("ERROR_4: %s is missing in tokens\n", st->top->string);
			pop(st);
		}
		if(temp!=NULL)
			printf("tokens left in input stream are:\n");
		while(temp!=NULL){
			printf("%s \n",temp->token);
			temp=temp->next;
		}
	}
	return t;
	}



//int uugs=0;
void DFS1(treeNode* t,int *count)
{
	//uugs++;
	treeNode* temp=t;
	treeNode* pt=temp->child;
	while(pt!=NULL)
	{
		(*count)++;
		DFS1(pt,count);
		pt=pt->next;
	}
}

int countParseTree(tree* t)
{
	int count=0;
	treeNode* tres=t->root;
	DFS1(tres,&count);
	return count;
}


void DFS(treeNode* t,int *count)
{
	//uugs++;
	treeNode* temp=t;
	printf("%s\t\t\t%d\t%s\t\t%.2f\t%s\t\t\t%s\t%s\n",temp->lexeCurNode, temp->lineno, temp-> token, temp->valuIfNumber, temp->parentNodeSymbol, temp->isLeafNode, temp->NodeSymbol );
	treeNode* pt=temp->child;
	while(pt!=NULL)
	{
		(*count)++;
		DFS(pt,count);
		pt=pt->next;
	}
}
int printParseTree(tree* t)
{
	int count=0;
	treeNode* tres=t->root;
	printf("Lexeme_current_node\tlineno\t\ttoken\tvalueIfNumber\tparentNodeSymbol\tisLeafNode\tNodeSymbol\n");
	DFS(tres,&count);
	return count;
}


void insertChild(tree* t,treeNode* tn){
    if(t->front==NULL)
    {
    	//if(tn->SymPtr==NULL&&strcmp(tn->lexeCurNode,"b3")==0)printf("haha %d\n",tn->lineno);
    	tn->next=NULL;
    	tn->prev=NULL;
        t->rear=tn;
        tn->parent=NULL;
        t->root=t->rear;
        t->height=0;
        t->front=t->rear;
    }
    else
    {
    	//if(tn->SymPtr==NULL&&strcmp(tn->lexeCurNode,"b3")==0)printf("haha %d\n",tn->lineno);
    	tn->next=NULL;
    	tn->prev=NULL;
        t->front->child=tn;
        tn->parent=t->front;
        t->height=t->height+1;
        t->rear=tn;
        t->front=t->rear;
        //printf("%sdd\n",t->front->NodeSymbol );
        //printf("%sddd\n",t->front->parent->NodeSymbol );
    }
}

void insertSibling(tree* t,treeNode* tn){
	//if(tn->SymPtr==NULL&&strcmp(tn->lexeCurNode,"b3")==0)printf("haha %d\n",tn->lineno);
	t->rear->next=tn;
    tn->parent=t->front->parent;
    tn->prev=t->rear;
    t->rear=tn;
    tn->next=NULL;
}

tree* initializeAST()
{
	tree* t= (tree*)malloc(sizeof(tree));
	t->root =NULL;
	t->front = NULL;
	t->rear =NULL;
	t->height =0;
	return t;
}
void handleEpsandPunc(treeNode* pt)
{
	int count=0;
	//printf("HIIIII\n");
	treeNode* temp=pt;
	treeNode* temp1;
	while(1)
	{
		while(temp->child!=NULL)
		{
			//printf("inside\n");
			temp=temp->child;
		}
		//printf("%s\n", temp->NodeSymbol);
		if(!strcmp(temp->NodeSymbol,"eps") ||!strcmp(temp->NodeSymbol,"EPS") || !strcmp(temp->NodeSymbol,"TK_SEM") || !strcmp(temp->NodeSymbol,"TK_COMMA") || !strcmp(temp->NodeSymbol,"TK_CL") || !strcmp(temp->NodeSymbol,"TK_OP") || !strcmp(temp->NodeSymbol,"TK_SQL") || !strcmp(temp->NodeSymbol,"TK_SQR") || !strcmp(temp->NodeSymbol,"TK_COLON") || !strcmp(temp->NodeSymbol,"TK_DOT") || !strcmp(temp->NodeSymbol,"TK_PARAMETERS") || !strcmp(temp->NodeSymbol,"TK_PARAMETER")|| !strcmp(temp->NodeSymbol,"TK_WITH"))
		{
			if(temp->prev==NULL && temp->next==NULL){
				while(temp->prev==NULL &&temp->next==NULL){
					//if(count<20)printf("%s\n",temp->parentNodeSymbol);	
					temp = temp->parent;
					free(temp->child);
					temp->child = NULL;
					//if(count<20)printf("%s\n",temp->parentNodeSymbol);
					//if(temp->child ==NULL)printf("heloooo\n");
				}
				//	break;
				continue;	
			}
			else{
				//printf("%s\n", );
				if(temp->prev==NULL)
				{
					//printf("Helooo\n");
					temp->parent->child=temp->next;
					temp->next->prev=NULL;
				}
				else
				{
					//printf("Helooo\n");
					//if(strcmp(temp->parentNodeSymbol,"<ioStmt>") && strcmp(temp->NodeSymbol,"TK_SEM")==0) printf("%s\n",temp->prev->NodeSymbol );
					temp->prev->next=temp->next;
					if(temp->next!=NULL)
					{
						if(strcmp(temp->NodeSymbol,"TK_SQR")==0){
							//printf("%s 123\n",temp->prev->NodeSymbol );
						}
						temp->next->prev=temp->prev;
					}
				}
				if(temp->next==NULL)
				{
					while(temp->parent->next==NULL)
						temp=temp->parent;
					temp = temp->parent->next;
				}
				else
				{
					//printf("next hua\n");
					temp=temp->next;
				}
			}
		}
		else
		{	
			if(temp->next==NULL)
			{
				while(temp!= NULL && temp->parent!=NULL&&temp->parent->next==NULL)
					{
						temp=temp->parent;
						//printf("%s\n",temp->NodeSymbol );
					}
				if(temp==NULL||temp->parent==NULL)break;
				temp = temp->parent->next;
			}
			else
			{
				temp=temp->next;
			}
		}
	}
}
void remov(treeNode* tn)
{
	treeNode* temp=tn,* curr;
	while(1)
	{
		while(temp->child!=NULL)
		{
			temp=temp->child;
		}
		if(strcmp(temp->isLeafNode,"NO")==0&&temp->child==NULL)
		{
			if(temp->next!=NULL)
			{
				curr=temp->next;
			}
			else
			{
				curr=temp;
				while(curr->parent->next==NULL)
				{
					curr=curr->parent;
				}
				curr=curr->parent->next;
			}
			if(temp->prev==NULL)
			{
				temp->parent->child=temp->next;
				temp->next->prev=NULL;
				free(temp);
			}
			else
			{
				temp->prev->next=temp->next;
				if(temp->next!=NULL)
					temp->next->prev=temp->prev;
				free(temp);
			}
			temp=curr;
		}
		else
		{
			if(temp->next!=NULL)
			{
				temp=temp->next;
			}
			else
			{
				while(temp!= NULL && temp->parent!=NULL&&temp->parent->next==NULL)
					{
						temp=temp->parent;
						//printf("%s\n",temp->NodeSymbol );
					}
				if(temp==NULL||temp->parent==NULL)break;
				temp = temp->parent->next;
			}
		}
	}
	
	

}
void collapse(treeNode* tn)
{
	treeNode* temp=tn->child;
	if(temp==NULL)
	{
		if(tn->prev==NULL&&tn->next==NULL)
		{
			if(tn->parent!=NULL&&tn->parent->prev==NULL)
			{
				if(tn->parent->parent!=NULL)
				{
					tn->parent->parent->child=tn;
					tn->next=tn->parent->next;
					if(tn->next!=NULL)
						tn->next->prev=tn;
					tn->parent=tn->parent->parent;
					strcpy(tn->parentNodeSymbol,tn->parent->NodeSymbol);
				}
			}
			else if(tn->parent!=NULL)
			{
				tn->parent->prev->next=tn;
				tn->prev=tn->parent->prev;
				tn->next=tn->parent->next;
				if(tn->next!=NULL)
				{
					tn->next->prev=tn;
				}
				tn->parent=tn->prev->parent;
				strcpy(tn->parentNodeSymbol,tn->parent->NodeSymbol);
			}
		}
	}
	while(temp!=NULL)
	{
		collapse(temp);
		temp=temp->next;
	}

}


void createAST(tree* pt)
{
	tree* t=initializeAST();
	t->root=pt->root;
	t->front=pt->root;
	pt->front=pt->root;
	pt->rear=pt->root;
	handleEpsandPunc(t->front);
	remov(t->front);
	collapse(t->front);
	
}
/*int main(int argc,char* argv[]){
	popGram("grammar.txt");
	popFirsts();
	populateFollows();
	printf("----------------------------------------------------\n");
	createParseTable(ru);
	printf("----------------------------------------------------\n");
	tokenInfo *ti = ABC(argv[1],100);
	tree *t = createParseTree();
	populatesyncset();
	parseInputSourceCode(ti,t);
	printf("hoooooo\n");
	printf("hoooooo\n");
	//popParseTree(t,ti);	
	printParseTree(t);
	return 0;
}*/