/*
		BATCH 10
		Gargi Sharma 2013A7PS161P
		Ayushi Agarwal 2013A7PS065P

*/

#include "lexerDef.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFF_SIZE 1024
int state=0;
FILE* input;
extern int lineno;
char buff[MAX_BUFF_SIZE];

FILE* getStream(FILE *fp,int k){
	char* tr=fgets(buff,k,fp);
	if(tr==NULL)
		return NULL;
	return fp;
}

char keywords[][23] ={"list","with","parameters","end","while","int","real","type","global","parameter","list","output","endwhile","then","endif","read","write","return","call","record","endrecord","else","input","if"};
char keyt[][23]={"TK_LIST","TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_INT","TK_REAL","TK_TYPE","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_OUTPUT","TK_ENDWHILE","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_CALL","TK_RECORD","TK_ENDRECORD","TK_ELSE","TK_INPUT","TK_IF"};

int forward=0;	

tokenInfo getNextToken(FILE *fp,int k){
	tokenInfo ti;
	char lexeme[31];
	int minuscount=0,flag=0,lex=0,mainFlag=0;
	while(input!=NULL){
		char c = buff[forward];
		//printf("%c\n",c );
		fflush(stdout);
		if(c=='\0') {
			input = getStream(fp,k);
			forward = 0;
			continue;
		}
		switch (state)
		{
			case 0:
				//printf("%c\n", c);
				if(c=='\r'||c==' '||c=='\t')
				{
					forward++;
				}
				else if(c=='\n') {
					lineno++;
					forward++;
				}
				else if(c=='%')
				{
					state=1;
					strcpy(ti.token,"TK_COMMENT");
					strcpy(ti.lexeme,"%");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='[')
				{
					state=0;
					strcpy(ti.token,"TK_SQL");
					strcpy(ti.lexeme,"[");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c==']')
				{
					state=0;
					strcpy(ti.token,"TK_SQR");
					strcpy(ti.lexeme,"]");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c==';')
				{
					state=0;
					strcpy(ti.token,"TK_SEM");
					strcpy(ti.lexeme,";");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c==':')
				{
					state=0;
					strcpy(ti.token,"TK_COLON");
					strcpy(ti.lexeme,":");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='.')
				{
					state=0;
					strcpy(ti.token,"TK_DOT");
					strcpy(ti.lexeme,".");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='(')
				{
					state=0;
					strcpy(ti.token,"TK_OP");
					strcpy(ti.lexeme,"(");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c==')')
				{
					state=0;
					strcpy(ti.token,"TK_CL");
					strcpy(ti.lexeme,")");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='+')
				{
					state=0;
					strcpy(ti.token,"TK_PLUS");
					strcpy(ti.lexeme,"+");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='-')
				{
					state=0;
					strcpy(ti.token,"TK_MINUS");
					strcpy(ti.lexeme,"-");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='*')
				{
					state=0;
					strcpy(ti.token,"TK_MUL");
					strcpy(ti.lexeme,"*");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='/')
				{
					state=0;
					strcpy(ti.token,"TK_DIV");
					strcpy(ti.lexeme,"/");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='~')
				{
					state=0;
					strcpy(ti.token,"TK_NOT");
					strcpy(ti.lexeme,"~");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c==',')
				{
					state=0;
					strcpy(ti.token,"TK_COMMA");
					strcpy(ti.lexeme,",");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else if(c=='='){
					state=2;
					forward++;
				}
				else if(c=='>'){
					state=3;
					forward++;
				}
				else if(c=='<'){
					state =4;
					forward++;
				}
				else if(c=='&'){
					state =5;
					forward++;
				}
				else if(c=='@'){
					state=7;
					forward++;
				}
				else if(c=='_'){
					state=9;
					forward++;
					lexeme[lex++] = '_';
				}
				else if(c=='I'){
					state = 15;
					forward++;
					lexeme[lex++]=c;
				}
				else if(c=='#'){
					state =19;
					forward++;
					lexeme[lex++]=c;
				}
				else if(isdigit(c)){
					state =21;
					forward++;
					lexeme[lex++]=c;
				}
				else if( 'b'<=c && c<='d'){
					state= 24;
					forward++;
					lexeme[lex++]=c;
				}
				else if('a'==c ||(c > 'd' && c<='z')){
					state=26;
					forward++;
					lexeme[lex++]=c;
				}
				else{
					printf("ERROR_2: Character <%c> not recognized at line %d\n",c,lineno );
					forward++;
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 1: //if comment then ignore
				if(c=='\n')
				{	
				//	printf("fffff\n");
					lineno++;
					forward++;
					state =0;
				}
				else{
					forward++;
				}
				break;
			case 2:
				if(c=='='){
					state=0;
					strcpy(ti.token,"TK_EQ");
					strcpy(ti.lexeme,"==");
					ti.lineno=lineno;
					forward++;
					return ti;	
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in the lexeme =\n", lineno);
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 3:
				if(c=='='){
					state=0;
					strcpy(ti.token,"TK_GE");
					strcpy(ti.lexeme,">=");
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else{
					state=0;
					strcpy(ti.token,"TK_GT");
					strcpy(ti.lexeme,">");
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 4:
				if(c=='='){
					state=0;
					strcpy(ti.token,"TK_LE");
					strcpy(ti.lexeme,"<=");
					ti.lineno=lineno;
					forward++;
					return ti;	
				}
				else if(c =='-'){
					minuscount++;
					if(minuscount==3){
						state=0;
						strcpy(ti.token,"TK_ASSIGNOP");
						strcpy(ti.lexeme,"<---");
						forward++;
						ti.lineno=lineno;
						return ti;		
					}
					forward++;
				}
				else{
					if(minuscount==0){
						state=0;
						strcpy(ti.token,"TK_LT");
						strcpy(ti.lexeme,"<");
						ti.lineno=lineno;
						return ti;
					}
					else if(minuscount!=3 || minuscount!=0){
						state=0;
						printf("ERROR_3: Error in assignop lexeme at lineno %d\n",lineno);
						minuscount=0;
						lexeme[lex++]=c;
						lexeme[lex++]='\0';
						strcpy(ti.token,"TK_LEXERROR");
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
				}
				break;
			case 5:
				if(c=='&'){
					state = 6;
					forward++;
				}
				else{
					printf("ERROR_3: Error in & lexeme at lineno %d\n",lineno);
					state =0;
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 6:
				if(c=='&'){
					state=0;
					strcpy(ti.token,"TK_AND");
					strcpy(ti.lexeme,"&&&");
					forward++;
					ti.lineno=lineno;
					return ti;
				}
				else{
					printf("ERROR_3: Error in & lexeme at lineno %d\n",lineno);
					state =0;
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 7:
				if(c=='@'){
					state =8;
					forward++;
				}
				else{
					printf("ERROR_3: Error in & lexeme at lineno %d\n",lineno);
					state =0;
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 8:
				if(c=='@'){
					state=0;
					strcpy(ti.token,"TK_OR");
					strcpy(ti.lexeme,"@@@");
					forward++;
					ti.lineno=lineno;
					return ti;
				}
				else{
					printf("ERROR_3: Error in @@@ lexeme at lineno %d\n",lineno);
					state =0;
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;	
				}
				break;
			case 9:
				if(c=='m'){
					state=10;
					forward++;
					lexeme[lex] = 'm';
					lex++;
				}	
				else if(isalpha(c)){
					state = 13;
					lexeme[lex++]=c;
					forward++;
				}
				break;
			case 10:
				if(c=='a'){
					state =11;
					lexeme[lex++]=c;
					forward++;
				}
				else if(isalpha(c)){
					state = 13;
					lexeme[lex++]=c;
					forward++;
				}
				else if(isdigit(c)){
					state=14;
					lexeme[lex++]=c;
					forward++;
				}
				break;
			case 11:
				if(c=='i'){
					state =12;
					lexeme[lex++]=c;
					forward++;
				}
				else if(isalpha(c)){
					state = 13;
					lexeme[lex++]=c;
					forward++;
				}
				else if(isdigit(c)){
					state=14;
					lexeme[lex++]=c;
					forward++;
				}
				break;
			case 12:
				if(c=='n'){
					mainFlag=1;
					state = 13;
					lexeme[lex++]=c;
					forward++;	
				}
				else if(isalpha(c)){
					state = 13;
					lexeme[lex++]=c;
					forward++;
				}
				else if(isdigit(c)){
					state=14;
					lexeme[lex++]=c;
					forward++;
				}
				break;
			case 13:
				if(isalpha(c)){
						state=13;
						mainFlag=0;
						lexeme[lex++]=c;
						forward++;
				}
				else if(isdigit(c)){
					state=14;
					mainFlag=0;
					lexeme[lex++]=c;
					forward++;
				}
				else if(mainFlag==1){
					state=0;
					strcpy(ti.token,"TK_MAIN");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;	
				}
				else if(mainFlag==0){
					if(lex<=30){
						state=0;
						strcpy(ti.token,"TK_FUNID");
						lexeme[lex++]='\0';
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
					else{
						state =0;
						printf("ERROR_1: Error in line no %d, function id size greater than prescribed size \n",lineno );
						lexeme[lex++]=c;
						lexeme[lex++]='\0';
						strcpy(ti.token,"TK_LEXERROR");
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}	
				}
				break;
			case 14:
				if(isdigit(c)){
					state=14;
					mainFlag=0;
					lexeme[lex++]=c;
					forward++;
				}
				else {
					if(lex<=30){
						state=0;
						strcpy(ti.token,"TK_FUNID");
						lexeme[lex++]='\0';
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
					else{
						state =0;
						printf("ERROR_1: Error in line no %d, function id size greater than prescribed size \n",lineno );
						lexeme[lex++]=c;
						lexeme[lex++]='\0';
						strcpy(ti.token,"TK_LEXERROR");
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
				}
				break;
			case 15:
				if(c=='f'){
					lexeme[lex++]=c;
					state=0;
					strcpy(ti.token,"TK_IF");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					forward++;
					return ti;	
				}
				else if(c=='n'){
					lexeme[lex++]=c;
					state=16;
					forward++;
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme If\n",lineno );
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 16:
				if(c=='p'){
					lexeme[lex++]=c;
					state = 17;
					forward++;
				}
				else{
					lexeme[lex++]=c;
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme If\n",lineno );	
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 17:
				if(c=='u'){
					lexeme[lex++]=c;
					state = 18;
					forward++;
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme If\n",lineno );	
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 18:
				if(c=='t'){
					lexeme[lex++]=c;
					state = 0;
					strcpy(ti.token,"TK_INPUT");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme If\n",lineno );
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 19:
				if('a' <= c && c <='z'){
					state=20;
					forward++;
					lexeme[lex++] =c;
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme #record_id\n",lineno);
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 20:
				if('a' <= c && c <='z'){
					state=20;
					forward++;
					lexeme[lex++] =c;
				}
				else{
					state = 0;
					strcpy(ti.token,"TK_RECORDID");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 21:
				if(isdigit(c)){
					state=21;
					forward++;
					lexeme[lex++]=c;
				}
				else if(c=='.'){
					state=22;
					forward++;
					lexeme[lex++]=c;
				}
				else{
					state = 0;
					strcpy(ti.token,"TK_NUM");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 22:
				if(isdigit(c)){
					state=23;
					lexeme[lex++]=c;
					forward++;
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme num\n",lineno);
					lexeme[lex++]=c;
					strcpy(ti.token,"TK_LEXERROR");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 23:
				if(isdigit(c)){
					lexeme[lex++]=c;
					state = 0;
					strcpy(ti.token,"TK_RNUM");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					forward++;
					return ti;
				}
				else{
					state=0;
					printf("ERROR_3: Error at lineno %d in lexeme r_num\n",lineno);
					lexeme[lex++]=c;
					lexeme[lex++]='\0';
					strcpy(ti.token,"TK_LEXERROR");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 24:
				if('2'<=c && c<='7'){
					state =25;
					forward++;
					lexeme[lex++]=c;
				}
				else if('a'<=c&&c<='z'){
					state=26;
					forward++;
					lexeme[lex++]=c;
				}
				else{
					state = 0;
					strcpy(ti.token,"TK_FIELDID");
					lexeme[lex++]='\0';
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 25:
				if('b'<=c&&c<='d'){
					state = 25;
					forward++;
					lexeme[lex++]=c;
				}
				else if('2'<=c && c<='7'){
					state = 27;
					forward++;
					lexeme[lex++]=c;
				}
				else{
					if(lex<=20){
						state = 0;
						strcpy(ti.token,"TK_ID");
						lexeme[lex++]='\0';
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
					else{
						state = 0;
						printf("ERROR_1: Error in line no %d, id size greater than prescribed size \n", lineno);
						lexeme[lex++]=c;
						lexeme[lex++]='\0';
						strcpy(ti.token,"TK_LEXERROR");
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
				}
				break;
			case 26:
				if('a'<=c && c<='z'){
					state = 26;
					forward++;
					lexeme[lex++]=c;
				}
				else{
					lexeme[lex++]='\0';
					int uua;
					for(uua=0;uua<24;uua++){
						if(strcmp(lexeme,keywords[uua])==0){
							state = 0;
							strcpy(ti.token,keyt[uua]);
							strcpy(ti.lexeme,lexeme);
							ti.lineno=lineno;
							return ti;
						}
					}
					state = 0;
					strcpy(ti.token,"TK_FIELDID");
					strcpy(ti.lexeme,lexeme);
					ti.lineno=lineno;
					return ti;
				}
				break;
			case 27:
				 if('2'<=c && c<='7'){
					state = 27;
					forward++;
					lexeme[lex++]=c;
				}
				else{
					if(lex<=20){
						state = 0;
						strcpy(ti.token,"TK_ID");
						lexeme[lex++]='\0';
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						return ti;
					}
					else{
						state = 0;
						printf("ERROR_1: Error in line no %d, id size greater than prescribed size \n", lineno);
						//lexeme[lex++]=c;
						lexeme[lex++]='\0';
						//printf("%s 11\n",lexeme);
						strcpy(ti.token,"TK_LEXERROR");
						
						strcpy(ti.lexeme,lexeme);
						ti.lineno=lineno;
						//printf("hihihi %s\n",ti.lexeme );
						return ti;
					}
				}
				break;
		}
	}
}


tokenInfo* store(char* filename,int k,tokenInfo* tkin){
    input=fopen(filename,"r");
    input=getStream(input,k);
    tokenInfo temp;
    int cs=0;
    while(input!=NULL) // to be seen what '\0' is
    {
    	temp=getNextToken(input,k);
    	//if(temp.lineno!=-1)
        {
        //	printf("current lexeme name is %s\n",temp.token);
        //	printf("current token name is %s\n",temp.lexeme);
        //	printf("line no of the current token is %d\n",temp.lineno);
        	if(tkin==NULL){
				tkin = (tokenInfo*)malloc(sizeof(tokenInfo));
				tkin->lineno = temp.lineno;
				strcpy(tkin->token,temp.token);
				strcpy(tkin->lexeme,temp.lexeme);
				tkin->value = temp.value; 	
			}		
			else{
				tokenInfo *temp2 = tkin;		
				while(temp2->next !=NULL){
					temp2 = temp2->next;
				}
				tokenInfo * temp1 = (tokenInfo*)malloc(sizeof(tokenInfo));
				temp1->lineno = temp.lineno;
				strcpy(temp1->token,temp.token);
				strcpy(temp1->lexeme,temp.lexeme);
				temp1->value = temp.value; 
				temp2->next = temp1;			
				temp1->next=NULL;
				}

        }
    }
    return tkin;
}

tokenInfo* ABC(char* filename,int k){
	tokenInfo *tkin=NULL;
	printf("hello\n");
	tkin=store(filename,k,tkin);
	printf("bte\n");

	tokenInfo* temp=tkin;
	if(temp==NULL)printf("bahahhahahha\n");
	while(temp->next!=NULL && temp->next->next!=NULL){
		temp=temp->next;
	}
	if(strcmp(temp->next->token,temp->token)==0){
		temp->next =NULL;
	}
	temp=tkin;
	/*while(temp!=NULL){
		printf("%s\n",temp->token);
		temp = temp->next;
	}*/
	//fclose(input);
	return tkin;
}