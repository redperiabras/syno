#ifndef STACKS_H_
#define STACKS_H_

//#include<stdio.h>
//#include<string.h>

#define MAX 100
#define bool int
#define true 1
#define false 0

static int stack_top = 0;

static char stack[MAX][MAX];

char *get_top(){
	return stack[stack_top-1];
}

void push(const char *input){
    if(stack_top == MAX-1){
    	printf("PDA STACK IS FULL");
	}else{
		printf("\nPUSHED: %s", input);
		strcpy(stack[stack_top], input);
		stack_top++;
	}
}

char *pop(){
	
	if(stack_top <= 0){
		printf("PDA STACK IS EMPTY");
		return NULL;
	}else{
		stack_top--;
//		strcpy(popped,stack[stack_top]);
//		printf("\nexecuted");
//		strcpy(stack[stack_top],"\0");
		return stack[stack_top];
	}
}

int terminal_identifier(char *input){
	/*
		returns 0 if true
				1 if false
	*/
	if(
		(
		strcmp(input, "INT") == 0 ||
		strcmp(input, "MAIN") == 0 ||
		strcmp(input, "(") == 0 ||
		strcmp(input, ")") == 0 ||
		strcmp(input, "{") == 0 ||
		strcmp(input, "}") == 0 ||
		strcmp(input, "FLOAT") == 0 ||
		strcmp(input, "STRING") == 0 ||
		strcmp(input, "CHAR") == 0 ||
		strcmp(input, "BOOLEAN") == 0 ||
		strcmp(input, "id") == 0 ||
		strcmp(input, ";") == 0 ||
		strcmp(input, ",") == 0 ||
		strcmp(input, "SCAN") == 0 ||
		strcmp(input, "DISPLAY") == 0 ||
		strcmp(input, "STRING_LITERAL") == 0 ||
		strcmp(input, "OUTPUT_EXT_EXP") == 0 ||
		strcmp(input, "++") == 0 ||
		strcmp(input, "--") == 0  ||
		strcmp(input, "!") == 0 ||
		strcmp(input, "+") == 0 ||
		strcmp(input, "-") == 0 ||
		strcmp(input, "*") == 0 ||
		strcmp(input, "/") == 0 ||
		strcmp(input, "%") == 0 ||
		strcmp(input, "DIV") == 0 ||
		strcmp(input, "&&") == 0 ||
		strcmp(input, "||") == 0 ||
		strcmp(input, ">") == 0 ||
		strcmp(input, ">=") == 0 ||
		strcmp(input, "<") == 0 ||
		strcmp(input, "<=") == 0 ||
		strcmp(input, "==") == 0 ||
		strcmp(input, "!=") == 0 ||
		strcmp(input, "INT_LITERAL") == 0  ||
		strcmp(input, "FLOAT_LITERAL") == 0 ||
		strcmp(input, "CHAR_LITERAL") == 0 ||
		strcmp(input, "TRUE") == 0 ||
		strcmp(input, "FALSE") == 0 ||
		strcmp(input, "=") == 0 ||
		strcmp(input, "GIVEN") == 0 ||
		strcmp(input, "OR_GIVEN") == 0 ||
		strcmp(input, "OTHERWISE") == 0 ||
		strcmp(input, "METHOD") ==0 ||
		strcmp(input, "RESPOND")==0||
		strcmp(input, "COMMA")==0 ||
		strcmp(input, "$")==0 ||
		strcmp(input, "WHILE") == 0) == true
	)
		return 0;
	else
		return 1;
}

#endif
