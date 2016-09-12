#ifndef LEXICAL_H_
#define LEXICAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "Syntax.h"


struct Node{
	bool isAccepting;
	bool finalState1;
	bool finalState;
	char tokenName[20];
	int acceptingStateNum;
	int lineNum; //
	int stateNum;
	int transitionCtr;
	
	struct Transition{
		char charInput;
		Node *adjNode;
	}transition[150];
};

FILE *synoFile;
FILE *OutputFile;
FILE *fp;
char current,lookahead, storeCurrent;
char input;
char temp;
bool checker = true;
bool checker2 = false;
bool checker3 = false;
bool checker4 = true;
bool checker5 = true;
bool checker6 = true;
bool traverseAgain = true;
bool lookup = true;
int nodeCtr = 0;
Node *nodes[150];
Node *state0;
int count = 0;
int count2 = 0;
int count3 = 0;
int lineNumber =1 ;
char lexeme[20];
int lexemeNum=0;
char arrayLexeme[20];
void traverse(Node *currentNode);
void invalid();

char buffer[20];

Node *createNode (bool isAccepting){
	nodes[nodeCtr] = (Node*)malloc(sizeof(Node));
	nodes[nodeCtr]->isAccepting = isAccepting; 
	nodes[nodeCtr]->stateNum = nodeCtr;
	nodes[nodeCtr]->transitionCtr = nodeCtr;
	nodeCtr++;
	return nodes[nodeCtr-1];
	}

void *createTransition(Node *fromState, Node *toState, char inputChar){
	fromState->transition[fromState->transitionCtr].charInput = inputChar;
	fromState->transition[fromState->transitionCtr].adjNode = toState;
	fromState->transitionCtr++;
}
Node* createToken(bool finalState1,bool finalState, int acceptingStateNum){
	nodes[acceptingStateNum]->finalState1 = finalState1;
	nodes[acceptingStateNum]->finalState = finalState;
	nodes[acceptingStateNum]->acceptingStateNum = acceptingStateNum;
	//printf("%d\n",nodes[acceptingStateNum]->acceptingStateNum );
	return nodes[acceptingStateNum];
}

void printLexeme(){
	//printf("%d", lexemeNum);
	int lexemeCount = 0;
	while(lexemeNum>0){
		//printf("%s", arrayLexeme);
		lexemeCount++;
		lexemeNum--;
	}
for (int c=0;c<20;c++){
		arrayLexeme[c] = NULL;
		
	}
	lexemeNum = 0;
}
void getLexeme(char currentInput){
	arrayLexeme[lexemeNum] = currentInput;
	lexemeNum++;
}
void clearStringArray(){
	strcpy(arrayLexeme, "\0");
}
void stringLiteral(){
	if(current=='\"'){
		lookahead = fgetc(synoFile);
		while(lookahead!='\"'){
			//printf("%c", lookahead);
			getLexeme(lookahead);
			fprintf(OutputFile,"%c", lookahead);
			current = lookahead;
			lookahead = fgetc(synoFile);
			if(lookahead=='\"'){
				temp = lookahead;
			}
		}
		//add_to_tape(arrayLexeme,"STRING_LITERAL",itoa(lineNumber,buffer,10));
		//printf(" STRING_LITERAL\t%d\n", lineNumber);
		//fprintf(OutputFile,",STRING_LITERAL,%d\n", lineNumber);
		//printf("%s", arrayLexeme);
		printLexeme();
		//clearStringArray();
		if(lookahead=='\"'){
			//printf("%c", lookahead);
			getLexeme(lookahead);
			fprintf(OutputFile,"%c", lookahead);
			fprintf(OutputFile,"~STRING_LITERAL~%d\n", lineNumber);
			//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
			printLexeme();
			//printf(" delimBracket\t%d\n", lineNumber);
			//fprintf(OutputFile,",\"", lineNumber);
		}	
	}
}

void charLiteral(){
	int e=0;
		if(current=='\''){
			lookahead = fgetc(synoFile);
			while(e!=1){
				//printf("%c", lookahead);
				getLexeme(lookahead);
				fprintf(OutputFile,"%c", lookahead);
				current = lookahead;
				lookahead = fgetc(synoFile);
				if(lookahead=='\''){
					temp = lookahead;
				}
				e++;
			}
			//add_to_tape(arrayLexeme,"CHAR_LITERAL",itoa(lineNumber,buffer,10));
			printLexeme();
			//printf(" CHAR_LITERAL\t%d\n", lineNumber);
			fprintf(OutputFile,"%c", lookahead);
			fprintf(OutputFile,"~CHAR_LITERAL~%d\n", lineNumber);
			if(lookahead=='\''){
				//printf("%c", lookahead);
				getLexeme(lookahead);
			//fprintf(OutputFile,"%c", lookahead);
			//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
			printLexeme();
			//fprintf(OutputFile,",\',%d\n", lineNumber);
				//printf(" arrayLexeme\t%d\n", lineNumber);
			}
		}		
}
void identifiers(){
	while(((lookahead != ' ' )||(!(isalnum(lookahead))))&&((lookahead == '_')||(isalnum(lookahead)))){
			//printf("%c", current);
			getLexeme(current);
			fprintf(OutputFile,"%c", current);
		lexeme[lexemeNum] = current;
		lexemeNum++;
		lookahead = fgetc(synoFile);
		current = lookahead;
	}
			//add_to_tape(arrayLexeme,"id",itoa(lineNumber,buffer,10));
			printLexeme();
	//printf(" id\t%d\n", lineNumber);
	fprintf(OutputFile,"~id~%d\n", lineNumber);
	current = lookahead;
}
void intFloatLiteral(){
	checker5 = true; //para isang beses lang magkakaroon ng dot sa decimal
	while(((lookahead!=' ')||(!isdigit(lookahead)))&&((lookahead=='.')||(isdigit(lookahead)))){
		//printf("%c", lookahead);
			getLexeme(lookahead);
			fprintf(OutputFile,"%c", lookahead);
		lookahead = fgetc(synoFile);
		if((lookahead=='.')&&(checker5)){
			checker5 = false;
		}
	}
	if(checker5==false){
			//add_to_tape(arrayLexeme,"FLOAT_LITERAL",itoa(lineNumber,buffer,10));
			printLexeme();
			//printf(" FLOAT_LITERAL\t%d\n", lineNumber);
			fprintf(OutputFile,"~FLOAT_LITERAL~%d\n", lineNumber);
	}
	else {
			//add_to_tape(arrayLexeme,"INT_LITERAL",itoa(lineNumber,buffer,10));
			printLexeme();
		//printf(" INT_LITERAL\t%d\n", lineNumber);
		fprintf(OutputFile,"~INT_LITERAL~%d\n", lineNumber);
	}
	current = lookahead; //global variable ang current
}
void SingleLineComment(){
		lookahead = fgetc(synoFile);
		//printf("%c", current);
		fprintf(OutputFile,"%c", current);
		fprintf(OutputFile,"~COMMENT_OPERATOR~%d\n", lineNumber);
		//printf(" <comment_operator>\t%d\n", lineNumber);
		current = lookahead;
		while(lookahead!='\n'){
			lookahead = fgetc(synoFile);
			//printf("%c", current);
			fprintf(OutputFile,"%c", current);
			current = lookahead;
		}
		if(current=='\n'){
			//lineNumber++;
			//printf("\n****************** Line number: %d ******************\n", lineNumber);
		}
		checker6 = false;
		fprintf(OutputFile,"~SINGLE_COMMENT~%d\n", lineNumber);
		current = lookahead;
}
void MultiLine(){
	while(lookahead!='*'){
		lookahead = fgetc(synoFile);
		//printf("%c", current);
		fprintf(OutputFile,"%c", current);
		current = lookahead;
		if(lookahead=='\n'){
			//printf("<multiLine_comment>\t%d", lineNumber);
			fprintf(OutputFile,"~MULTI_COMMENT~%d\n", lineNumber);
			lineNumber++;
			MultiLine();
		}								
	}
}
void MultiLineComment(){
	lookahead = fgetc(synoFile);
	//printf("%c", current);
	fprintf(OutputFile,"%c", current);
	//printf(" <comment_operator>\t%d\n", lineNumber);
	fprintf(OutputFile,"~COMMENT_OPERATOR~%d\n", lineNumber);
	current = lookahead;
	
	MultiLine();
	//printf("<multiLine_comment>\t%d", lineNumber);
	fprintf(OutputFile,"~MULTI_COMMENT~%d\n", lineNumber);
	if(lookahead=='*'){
		lookahead = fgetc(synoFile);
		//printf("\n%c", current);
		fprintf(OutputFile,"%c", current);
		printLexeme();
		current = lookahead;
		if(lookahead=='/'){
			lookahead = fgetc(synoFile);
			printf("%c", current);
			fprintf(OutputFile,"%c", current);
		
			printLexeme();
			//printf(" <comment_operator>\t%d\n", lineNumber);
			fprintf(OutputFile,"~COMMENT_OPERATOR~%d\n", lineNumber);
			current = lookahead;
		}
		else{
			MultiLine();
		}
	}
	
	checker6 = false;
	if(current=='\n'){
		lineNumber++;
	}
	//printf("<multiLine_comment>\t%d\n", lineNumber);
	current = lookahead;
}
void invalid(){
	while((lookahead>='a' && lookahead <='z')||(isdigit(lookahead))){
				//printf("%c", lookahead);
				getLexeme(lookahead);
				fprintf(OutputFile,"%c", lookahead);
				lookahead = fgetc(synoFile);
				//current = lookahead;
				if(lookahead==' ' || lookahead=='\n'){
			//add_to_tape(arrayLexeme,"invalid",itoa(lineNumber,buffer,10));
			printLexeme();
				//printf(" <Invalid>\t%d\n",lineNumber);
				fprintf(OutputFile,"~INVALID~%d\n",lineNumber);
			}
			}
		}
void traverse(Node *currentNode){
	int a;
	count++;
	
	if(checker2){
		if(traverseAgain){
		lookahead = fgetc(synoFile);
		}
		if(lookahead != ' ' || lookahead != '\n'){
			current = lookahead;
		}
		else if(!(isalnum(lookahead))){
			current = lookahead;
		}
		if((lookahead>='a')&&(lookahead<='z')){
				identifiers();
		}
		if(isdigit(lookahead)){
				intFloatLiteral();		
		}
		if(current=='\n'){
			lineNumber++;
			//printf("\n****************** Line number: %d ******************\n", lineNumber);
		}

	}
	if(checker){ //once lang pwede mag enter dito kapag first reading
		current = input;
		lookahead = input;
		checker = false;// parang gate sya na ni lockk nya ang option na ito
		checker2 = true; // inopen nya ang another gate o checker 
	} 
	for(a = 0; a < currentNode->transitionCtr; a++){
		traverseAgain = true;
		if(currentNode->transition[a].charInput == current){
			
			//getLexeme(current);
			//printf("%c", current); //printing ng mga inputs based sa automaton
			getLexeme(current);
			if(current!=','){
			fprintf(OutputFile,"%c", current);
			}
			else{
				fprintf(OutputFile,",");
			}
			//lookahead = fgetc(synoFile);
			//current = lookahead;
			/*if((lookahead>='a' && lookahead <='z')||(isdigit(lookahead))){
			void invalid();
					}*/
			if((nodes[currentNode->stateNum+1]->isAccepting == true)||(currentNode->transition[a].adjNode->isAccepting == true)){
				lookup = false;
				if(nodes[currentNode->stateNum+1]->isAccepting == true){ //para sa final state ng keyword and reserved word
					//printf(" <word>\t%d\n",lineNumber);
					fprintf(OutputFile,"~%s~",arrayLexeme);
					fprintf(OutputFile,"%d\n",lineNumber);
					//printf(" <word>\t%d\n",lineNumber);
				//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
				printLexeme();
				}
				if((nodes[currentNode->stateNum+1]->isAccepting == false)&&(currentNode->transition[a].adjNode->isAccepting == true)){ //para sa other syntactic elements
					checker6 = true; //para irecognize kung accepted
				if(current=='-'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='*'){
						SingleLineComment();
					} 
					else if(current=='-'){ 
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						//printf(" ARITHM_OPERATOR\t%d\n",lineNumber);
						fprintf(OutputFile,"~--~%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
							//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
							printLexeme();
							fprintf(OutputFile,"~-~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='+'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='+'){
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						fprintf(OutputFile,"~++~%d\n",lineNumber);
						//printf(" <arith_operator>\t%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
							//printf(" <arith_operator>\t%d\n",lineNumber);
							fprintf(OutputFile,"~+~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='='){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='='){
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,"RELATIONAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
						fprintf(OutputFile,"~==~%d\n",lineNumber);
						//printf(" <relational_operator>\t%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,"ASSIGN_OPERATOR",itoa(lineNumber,buffer,10));
							//printf(" <assign_operator>\t%d\n",lineNumber);
							fprintf(OutputFile,"~=~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='!'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='='){
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,"RELATIONAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
						//printf(" <relational_operator>\t%d\n",lineNumber);
						fprintf(OutputFile,"~!=~%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,"LOGICAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
							//printf(" <logical_operator>\t%d\n",lineNumber);
							fprintf(OutputFile,"~!~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='&'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='&'){
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,"LOGICAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
						//printf(" <logical_operator>\t%d\n",lineNumber);
						fprintf(OutputFile,"~&&~%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,"INVALID",itoa(lineNumber,buffer,10));
						printLexeme();
							//printf(" <Invalid>\t%d\n",lineNumber);
							fprintf(OutputFile,"~INVALID~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='|'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='|'){
						//printf("%c", current);
						//getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,"LOGICAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
						//rintf(" <logical_operator>\t%d\n",lineNumber);
						fprintf(OutputFile,"~||~%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,"INVALID",itoa(lineNumber,buffer,10));
						printLexeme();
						fprintf(OutputFile,"~INVALID~%d\n",lineNumber);
							//printf(" <Invalid>\t%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='>'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='='){
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,"RELATIONAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
						//printf(" <relational_operator>\t%d\n",lineNumber);
						fprintf(OutputFile,"~>=~%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,"RELATIONAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
							//printf(" <relational_operator>\t%d\n",lineNumber);
							fprintf(OutputFile,"~>~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='<'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='='){
						//printf("%c", current);
						getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,"RELATIONAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
						//printf(" <relational_operator>\t%d\n",lineNumber);
						fprintf(OutputFile,"~<=~%d\n",lineNumber);
					}
					else {
							//printf("%c", storeCurrent);
						//add_to_tape(arrayLexeme,"RELATIONAL_OPERATOR",itoa(lineNumber,buffer,10));
						printLexeme();
							//printf(" <relational_operator>\t%d\n",lineNumber);
							fprintf(OutputFile,"~<~%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='/'){
					lookahead = fgetc(synoFile);
					current = lookahead;
					checker6 = false;
					if(current=='*'){
						MultiLineComment();
					}
					else {
							//printf("%c", storeCurrent);
							getLexeme(current);
						fprintf(OutputFile,"%c", current);
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						fprintf(OutputFile,"~/~%d\n",lineNumber);
							//printf(" <arithm_operator>\t%d\n",lineNumber);
							traverseAgain = false;
							traverse(state0);
							count--;
					}
				}
				if(current=='\"'){
					checker6 = false;
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						//fprintf(OutputFile,",\",%d\n",lineNumber);
					//printf(" <delimBracket>\t%d\n",lineNumber);
					stringLiteral();
				}
				if(current=='\''){
					checker6 = false;
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						//fprintf(OutputFile,",\',%d\n",lineNumber);
					//printf(" <delimBracket>\t%d\n",lineNumber);
					charLiteral();
				}
				if(current=='*'|| current=='%'||current=='^'){
					checker6 = false;
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						if(current=='*'){
					//printf(" <arithm_operator>\t%d\n",lineNumber);
					fprintf(OutputFile,"~*~%d\n",lineNumber);
									}
					if(current=='%'){
					//printf(" <arithm_operator>\t%d\n",lineNumber);
					fprintf(OutputFile,"~%~%d\n",lineNumber);
									}
						if(current=='^'){
					//printf(" <arithm_operator>\t%d\n",lineNumber);
					fprintf(OutputFile,"~^~%d\n",lineNumber);
									}
				}
				if(current==','){
					checker6 = false;
						//add_to_tape(arrayLexeme,"COMMA",itoa(lineNumber,buffer,10));
						printLexeme();
					//printf(" <comma_separator>\t%d\n",lineNumber);
					fprintf(OutputFile,"~COMMA~%d\n",lineNumber);
				}
				if(checker6){
						//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
						printLexeme();
						fprintf(OutputFile,"~%c~",current);
						fprintf(OutputFile,"%d\n",lineNumber);
						//fprintf(OutputFile,",<delimBracket>,%d\n",lineNumber);
					//printf(" <delimBracket>\t%d\n",lineNumber);
				}
				checker6 = false;
			}
		}
		if((currentNode->transition[a].adjNode->isAccepting==true)){
			traverse(state0);
			count--;
		} //pupunta lang sya dito every read ng character kapag nagkaroon na ng final state
		traverse(currentNode->transition[a].adjNode); //pupunta lang sya dito every read ng character.. example kapag keyword and reserved word
	}
	}
	if(isalnum(current)){
		while(current!=' ' && current!='\n' && isalnum(current)){
			printf("%c", current);
			getLexeme(current);
			fprintf(OutputFile,"%c", current);
			lookahead = fgetc(synoFile);
			current = lookahead;
			traverseAgain = false;
		}
		//add_to_tape(arrayLexeme,arrayLexeme,itoa(lineNumber,buffer,10));
		printLexeme();
	//	printf(" <Invalid>\t%d\n",lineNumber);
	fprintf(OutputFile,"~INVALID~%d\n",lineNumber);
	}
	if (count!=2000){ // numbers of character ng syno file
		traverse(state0); //pupunta lang sya dito every read ng character kapag wala sa automaton ang input
	}
}

void lexical_analysis(char *input_file){
	synoFile = fopen(input_file,"r");
	char strFilename[80]=".txt";
	char OutputFilename[80]="samsymtb";
	
	fp = fopen(strFilename,"r");
	
	strcat(OutputFilename,strFilename);
	OutputFile=fopen(OutputFilename,"w");
	fclose(OutputFile);
	
	OutputFile=fopen(OutputFilename,"a");
	//lineNumber++;
	//printf("****************** Line number: %d ******************\n", "line");
	state0 = createNode(false);
	Node *state1 = createNode(false);
	Node *state2 = createNode(false);
	Node *state3 = createNode(false);
	Node *state4 = createNode(false);
	Node *state5 = createNode(false);
	Node *state6 = createNode(false);
	Node *state7 = createNode(true);
	Node *state8 = createNode(false);
	Node *state9 = createNode(false);
	Node *state10 = createNode(false);
	Node *state11 = createNode(true);
	Node *state12 = createNode(false);
	Node *state13 = createNode(false);
	Node *state14 = createNode(false);
	Node *state15 = createNode(false);
	Node *state16 = createNode(false);
	Node *state17 = createNode(false);
	Node *state18 = createNode(true);
	Node *state19 = createNode(true);
	Node *state20 = createNode(false);
	Node *state21 = createNode(false);
	Node *state22 = createNode(true);
	Node *state23 = createNode(false);
	Node *state24 = createNode(false);
	Node *state25 = createNode(false);
	Node *state26 = createNode(true);
	Node *state27 = createNode(false);
	Node *state28 = createNode(false);
	Node *state29 = createNode(false);
	Node *state30 = createNode(false);
	Node *state31 = createNode(false);
	Node *state31A = createNode(false);
	Node *state32 = createNode(true);
	Node *state33 = createNode(false);
	Node *state34 = createNode(false);
	Node *state35 = createNode(true);
	Node *state36 = createNode(false);
	Node *state37 = createNode(false);
	Node *state38 = createNode(false);
	Node *state39 = createNode(false);
	Node *state40 = createNode(false);
	Node *state41 = createNode(true);
	Node *state42 = createNode(false);
	Node *state43 = createNode(false);
	Node *state44 = createNode(true);
	Node *state45 = createNode(false);
	Node *state46 = createNode(false);
	Node *state47 = createNode(false);
	Node *state48 = createNode(false);
	Node *state49 = createNode(false);
	Node *state50 = createNode(false);
	Node *state51 = createNode(false);
	Node *state52 = createNode(true);
	Node *state53 = createNode(false);
	Node *state54 = createNode(false);
	Node *state55 = createNode(false);
	Node *state56 = createNode(false);
	Node *state57 = createNode(false);
	Node *state58 = createNode(false);
	Node *state59 = createNode(false);
	Node *state60 = createNode(true);
	Node *state61 = createNode(false);
	Node *state62 = createNode(false);
	Node *state63 = createNode(false);
	Node *state64 = createNode(false);
	Node *state65 = createNode(false);
	Node *state66 = createNode(false);
	Node *state67 = createNode(false);
	Node *state68 = createNode(true);
	Node *state69 = createNode(false);
	Node *state70 = createNode(false);
	Node *state71 = createNode(true);
	Node *state72 = createNode(false);
	Node *state73 = createNode(false);
	Node *state74 = createNode(false);
	Node *state75 = createNode(true);
	Node *state76 = createNode(false);
	Node *state77 = createNode(false);
	Node *state78 = createNode(false);
	Node *state79 = createNode(false);
	Node *state80 = createNode(true);
	Node *state81 = createNode(false);
	Node *state82 = createNode(false);
	Node *state83 = createNode(false);
	Node *state84 = createNode(false);
	Node *state85 = createNode(true);
	Node *state86 = createNode(true);
	Node *state87 = createNode(true);
	Node *state88 = createNode(true);
	Node *state89 = createNode(true);
	Node *state90 = createNode(true);
	Node *state91 = createNode(true);
	Node *state92 = createNode(true);
	Node *state93 = createNode(true);
	Node *state94 = createNode(true);
	Node *state95 = createNode(true);
	Node *state96 = createNode(true);
	Node *state97 = createNode(true);
	Node *state98 = createNode(true);
	Node *state99 = createNode(true);
	Node *state100 = createNode(true);
	Node *state101 = createNode(true);
	Node *state102 = createNode(false);
	Node *state103 = createNode(true);
	Node *state104 = createNode(false);
	Node *state105 = createNode(true);
	Node *state106 = createNode(true);
	Node *state107 = createNode(true);
	Node *state108 = createNode(true);
	Node *state109 = createNode(true);
	
	Node *state110 = createNode(false);
	Node *state111 = createNode(false);
	Node *state112 = createNode(false);
	Node *state113 = createNode(true);
	Node *state114 = createNode(false);
	Node *state115 = createNode(false);
	Node *state116 = createNode(false);
	Node *state117 = createNode(true);
	
	//keyword reserved word
	/*state7 = createToken(true,true, 7);
	state11 = createToken(true,true, 11);
	state18 = createToken(true,false, 18);
	state21 = createToken(true,false, 21);
	state25 = createToken(true,true, 25);
	state30 = createToken(true,true, 30);
	state33 = createToken(true,true, 33);
	state39 = createToken(true,false, 39);
	state47 = createToken(true,true, 47);
	state55 = createToken(true,true, 55);
	state62 = createToken(true,false, 62);
	state65 = createToken(true,true, 65);
	state69 = createToken(true,false, 69);
	state74 = createToken(true,true, 74);
	state79 = createToken(true,true, 79);
	
	//logical
	state103 = createToken(false,false, 103);
	state105 = createToken(false,false, 105);
	state106 = createToken(false,false, 106);
	state108 = createToken(false,false, 108);*/
	
	
	
	createTransition(state0, state1, 'B');
	createTransition(state1, state2, 'O');
	createTransition(state2, state3, 'O');
	createTransition(state3, state4, 'L');
	createTransition(state4, state5, 'E');
	createTransition(state5, state6, 'A');
	createTransition(state6, state7, 'N');
	createTransition(state0, state8, 'C');
	createTransition(state8, state9, 'H');
	createTransition(state9, state10, 'A');
	createTransition(state10, state11, 'R');
	createTransition(state0, state12, 'D');
	createTransition(state12, state13, 'I');
	createTransition(state13, state14, 'S');
	createTransition(state14, state15, 'P');
	createTransition(state15, state16, 'L');
	createTransition(state16, state17, 'A');
	createTransition(state17, state18, 'Y');
	createTransition(state13, state19, 'V');
	createTransition(state0, state20, 'F');
	createTransition(state20, state21, 'I');
	createTransition(state21, state22, 'X');
	createTransition(state20, state23, 'L');
	createTransition(state23, state24, 'O');
	createTransition(state24, state25, 'A');
	createTransition(state25, state26, 'T');
	createTransition(state0, state27, 'G');
	createTransition(state27, state28, 'I');
	createTransition(state28, state29, 'V');
	createTransition(state29, state31A, 'E');
	createTransition(state31A, state32, 'N');
	createTransition(state0, state33, 'I');
	createTransition(state33, state34, 'N');
	createTransition(state34, state35, 'T');
	createTransition(state0, state36, 'M');
	createTransition(state36, state37, 'E');
	createTransition(state37, state38, 'T');
	createTransition(state38, state39, 'H');
	createTransition(state39, state40, 'O');
	createTransition(state40, state41, 'D');
	createTransition(state36, state42, 'A');
	createTransition(state42, state43, 'I');
	createTransition(state43, state44, 'N');
	createTransition(state0, state45, 'O');
	createTransition(state45, state46, 'R');
	createTransition(state46, state47, ' ');
	createTransition(state47, state48, 'G');
	createTransition(state48, state49, 'I');
	createTransition(state49, state50, 'V');
	createTransition(state50, state51, 'E');
	createTransition(state51, state52, 'N');
	createTransition(state45, state53, 'T');
	createTransition(state53, state54, 'H');
	createTransition(state54, state55, 'E');
	createTransition(state55, state56, 'R');
	createTransition(state56, state57, 'W');
	createTransition(state57, state58, 'I');
	createTransition(state58, state59, 'S');
	createTransition(state59, state60, 'E');
	createTransition(state0, state62, 'R');
	createTransition(state62, state63, 'E');
	createTransition(state63, state64, 'S');
	createTransition(state64, state65, 'P');
	createTransition(state65, state66, 'O');
	createTransition(state66, state67, 'N');
	createTransition(state67, state68, 'D');
	//createTransition(state68, state69, ' ');
	createTransition(state0, state70, 'T');
	createTransition(state70, state71, 'O');
	createTransition(state0, state72, 'S');
	createTransition(state72, state73, 'C');
	createTransition(state73, state74, 'A');
	createTransition(state74, state75, 'N');
	createTransition(state72, state76, 'T');
	createTransition(state76, state77, 'R');
	createTransition(state77, state78, 'I');
	createTransition(state78, state79, 'N');
	createTransition(state79, state80, 'G');
	createTransition(state0, state81, 'W');
	createTransition(state81, state82, 'H');
	createTransition(state82, state83, 'I');
	createTransition(state83, state84, 'L');
	createTransition(state84, state85, 'E');
	createTransition(state0, state86, '+');
	createTransition(state0, state87, '-');
	createTransition(state0, state88, '*');
	createTransition(state0, state89, '/');
	createTransition(state0, state90, '%');
	createTransition(state0, state91, '^');
	createTransition(state0, state92, '=');
	createTransition(state0, state93, '!');
	createTransition(state0, state94, '>');
	createTransition(state0, state95, '<');
	createTransition(state0, state96, '[');
	createTransition(state0, state97, ']');
	createTransition(state0, state98, '(');
	createTransition(state0, state99, ')');
	createTransition(state0, state100, '{');
	createTransition(state0, state101, '}');
	//createTransition(state0, state102, '|');
	createTransition(state0, state103, '|');
	//createTransition(state0, state104, '&');
	createTransition(state0, state105, '&');
	createTransition(state0, state106, ';');
	createTransition(state0, state107, '\"');
	createTransition(state0, state108, '\'');
	createTransition(state0, state109, ',');
	createTransition(state20, state110, 'A');
	createTransition(state110, state111, 'L');
	createTransition(state111, state112, 'S');
	createTransition(state112, state113, 'E');
	createTransition(state0, state114, 'T');
	createTransition(state114, state115, 'R');
	createTransition(state115, state116, 'U');
	createTransition(state116, state117, 'E');
	
	traverse(state0);
	//getch();
	//parser();
//	printf("Lexical Analysis Completed!");
}

#endif
