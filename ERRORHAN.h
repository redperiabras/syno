#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

//#include<stdlib.h>
//#include<stdio.h>
//#include<conio.h>
//#include<stdlib.h>
//#include<string.h>

struct Node{
	char line_num[3];
	char input[50];
	Node *next;
} *head, *current, *new_error;

bool first = true;

void add_error(char *input_line_num, char *input_input){
	new_error = (Node *)malloc(sizeof(Node));
	strcpy(new_error->input, input_input);
	strcpy(new_error->line_num, input_line_num);
	
	if(first){
		head = new_error;
		current = head;
		first = false;
	}else{
		current->next = new_error;
		current = current->next;
	}
}

void print_errors(){
	current = head;
	printf("\n\nReported Errors:");
	while(current != NULL){
		printf("\n\tLine %s: Unexpected %s", current->line_num, current->input);
		current = current->next;
	}
}
#endif
