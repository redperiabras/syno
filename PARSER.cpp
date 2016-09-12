#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "csvmanag.h"
#include "stack.h"
#include "errorhan.h"
#include "parsetre.h"

struct derivations{
	char input[50];
	CsvRow *derives;
	derivations *next;
} *curr_derivation, *head_derivation, *new_derivation;

struct symbols{
	char symbol[50];
	derivations *start;
	struct symbols *next;
} *curr_symbol, *head_symbol, *new_symbol;

void init_parse_table(){
	int i =  0;
    //                                   file, delimiter, first_line_is_header?
    CsvParser *csvparser = CsvParser_new("parsetbl.csv", ",", 1);
    CsvRow *header;
    CsvRow *row;

    header = CsvParser_getHeader(csvparser);
    if (header == NULL) {
        printf("%s\n", CsvParser_getErrorMessage(csvparser));
    }
    char **headerFields = CsvParser_getFields(header);
    
    bool head = true;
    bool start_derivation = true;
    
    //INITIAL SYMBOL TABLE ROW
    row = CsvParser_getRow(csvparser);
    
    while(row != NULL){
    	
        char **rowFields = CsvParser_getFields(row);
        
        for (i = 0 ; i < CsvParser_getNumFields(row) ; i++) {
//        	printf("FIELD: %s\n", rowFields[i]);
			if(i==0){
				//Symbols
				/*
					i is zero, pointing to the symbols column
					row->numOfFields is 1, pointing to the table's 2nd row which will be the initial input of the parser
				*/
				new_symbol = (symbols *)malloc(sizeof(symbols));
				strcpy(new_symbol->symbol, rowFields[i]);

				if(head){
					head_symbol = new_symbol;
					curr_symbol = head_symbol;
					head = false;
				}else{
					curr_symbol->next = new_symbol;
					curr_symbol = new_symbol;
				}

				//for every row
				start_derivation = true;
			}
			else{
				if(strcmp(rowFields[i]," ")==1){ //field is not null
					CsvParser *derivation_parser = CsvParser_new_from_string(rowFields[i], " ", 0);
				
					new_derivation = (derivations *)malloc(sizeof(derivations));
					strcpy(new_derivation->input,headerFields[i]);
					new_derivation->derives = CsvParser_getRow(derivation_parser);
					
					if(start_derivation){
						curr_symbol->start = new_derivation;
						curr_derivation = new_derivation;
						start_derivation = false;
					}
					else{
						curr_derivation->next = new_derivation;
						curr_derivation = new_derivation;
					}
				
//					int j;
//					char **values = CsvParser_getFields(curr_derivation->derives);
//					for (j = 0 ; j < CsvParser_getNumFields(curr_derivation->derives) ; j++) {
//						printf("%s\n",values[j]);
//					}

					CsvParser_destroy(derivation_parser);
				}			
			}
        }
//        printf("\n");
        CsvParser_destroy_row(row);
        //NEXT ROW
        row = CsvParser_getRow(csvparser);
//        getch();
    }
    CsvParser_destroy(csvparser);
}

void unload_parse_table(){
	
	//start from head symbol
	curr_symbol = head_symbol;
	int ctr = 0;
    while(curr_symbol != NULL){
    	ctr++;
    	curr_derivation = curr_symbol->start;
    	
    	printf("\n\nCurr Symbol: %d %s", ctr, curr_symbol->symbol);
    	int der_ctr = 0;
    	do{
    		der_ctr++;
			printf("\nCurr Derivation: %d %s", der_ctr, curr_derivation->input);
    		
    		char **derivation_array = CsvParser_getFields(curr_derivation->derives);
	    	printf("\nStart of Derivation: %s",derivation_array[0]);
							
    		printf("\nUnloading Derivation");
	    	CsvParser_destroy_row(curr_derivation->derives);
	    	printf("\nUnloaded Derivation");
	    	
			printf("\nMoving to next derivation");
	    	curr_derivation = curr_derivation->next;
	    	printf("\nMoved to next derivation");
		}while(curr_derivation != NULL);
		
		printf("\nMoving to next Symbol");
		curr_symbol = curr_symbol->next;
		printf("\nMoved to next Symbol");
	}
	printf("\n\nUnloading Complete");
}

int main() {
	
    init_parse_table();
    
    //initialize lexical output file
    CsvParser *lexical_parser = CsvParser_new("samsymtb.txt", "~", 0);
    CsvRow *symboltbl_row;
    
    //initialize stack
    push("$");
    push("SYNO");
    
    //initialize symbol table input
    symboltbl_row = CsvParser_getRow(lexical_parser);
    
    char token[50];
    char **symboltbl_rowFields;
    
    
    bool root = true;
    bool EOF_reached = false;
    bool found_derivation = false;
    
    do{
    	printf("\n\nSTACK'S TOP: %s", get_top());
    	
    	if(symboltbl_row != NULL){
    		symboltbl_rowFields = CsvParser_getFields(symboltbl_row);
    		strcpy(token,symboltbl_rowFields[1]);
		}else{
			EOF_reached = true;
			strcpy(token,"$");
		}
    	
    	printf("\nCURRENT TOKEN: %s\nACTION--------", token);
    	
    	if(strcmp(token, "COMMENT_OPERATOR") == 0 || strcmp(token, "SINGLE_COMMENT") == 0 || strcmp(token, "MULTI_COMMENT") == 0){
    		//if current token is comment ignore and move to next token
			CsvParser_destroy_row(symboltbl_row);
			symboltbl_row = CsvParser_getRow(lexical_parser);
		}
		else{
			if(symboltbl_row != NULL){
				if((terminal_identifier(get_top())==1)){ //top of stack is symbol
					//symbol table has input token
					//start scanning table from the head symbol
					curr_symbol = head_symbol;	
					found_derivation = false;
							
			    	do{ //scan non-terminals or symbols		
			    		if(strcmp(get_top(),curr_symbol->symbol)==0){ //symbol found
			    			curr_derivation = curr_symbol->start; //initialize derivation list with the current symbol's start derivation link
			    			do{ //scan derivations
								if(strcmp(curr_derivation->input,symboltbl_rowFields[1])==0){ //derivation for this input found
			    					char popped_symbol[50]; //pop the top of the stack
			    					strcpy(popped_symbol, pop());
			    					printf("\nPOPPED: %s", popped_symbol);
			    					add_line(popped_symbol);
			    			
									char **derivation_array = CsvParser_getFields(curr_derivation->derives);
			    					int j;
									for (j = CsvParser_getNumFields(curr_derivation->derives) - 1 ; j >=0 ; j--) { //push from the last to first item in array of terminals
										if(strcmp(derivation_array[j],"''")==0) //if it derives empty break this loop
											break;
										else{ //push derivations
											push(derivation_array[j]);
										}
									}
									
									for (j = 0 ; j < CsvParser_getNumFields(curr_derivation->derives) ; j++) { //push from the first to last item in array of terminals for the parse tree representation
										concat_data(derivation_array[j]);
										if(strcmp(derivation_array[j],"''")==0) //if it derives empty break this loop
											break;
									}
										
									found_derivation = true;
									break;
								}
											
								//move to the next derivation
								curr_derivation = curr_derivation->next;
										
								if((!found_derivation) && (curr_derivation==NULL)){
									//no found derivation
									add_error(symboltbl_rowFields[2],symboltbl_rowFields[0]);
												
									//destroy current token input
									CsvParser_destroy_row(symboltbl_row);
									//move to next token input 
									symboltbl_row = CsvParser_getRow(lexical_parser);
								}
							}while(curr_derivation != NULL);
							break;
						}
									
						//move to the next symbol
						curr_symbol = curr_symbol->next;
					}while(curr_symbol != NULL);
				}
				else{ //top of stack is terminal
					if (strcmp(get_top(), symboltbl_rowFields[1]) == 0){	
						char popped_symbol[50]; //pop the top of the stack
			    		strcpy(popped_symbol, pop());
		//	    		curr_treenode = addNode
			    		printf("\nPOPPED: %s", popped_symbol);
						add_line(popped_symbol);
						concat_data(symboltbl_rowFields[0]);
					}else{
						//ERROR
						add_error(symboltbl_rowFields[2],symboltbl_rowFields[0]);
					}
						
					//destroy current token input
					CsvParser_destroy_row(symboltbl_row);
					//move to next token input 
					symboltbl_row = CsvParser_getRow(lexical_parser);
				}
			}else{
				//symbol table's inputs are already used up
				if((terminal_identifier(get_top())==1)){ //top of stack is symbol
	    			//start scanning table from the head symbol
					curr_symbol = head_symbol;
					found_derivation = false;
					while(curr_symbol != NULL){ //scan non-terminals or symbols
		    			if(strcmp(get_top(),curr_symbol->symbol)==0){ //non-terminal found
			    			curr_derivation = curr_symbol->start; //initialize derivation list with the current symbol's start derivation link
		    				while(curr_derivation != NULL){ //scan derivations
		    					if(strcmp(curr_derivation->input,"$")==0){ //derivation for this input found
									char popped_symbol[50]; //pop the top of the stack
		    						strcpy(popped_symbol, pop());
		    						printf("\nPOPPED: %s", popped_symbol);
		    						printf("\nTOP: %d", stack_top);
									add_line(popped_symbol);
		    						
									char **derivation_array = CsvParser_getFields(curr_derivation->derives);
									for (int j = CsvParser_getNumFields(curr_derivation->derives) - 1 ; j >=0 ; j--) { //push from the last to first item in array of terminals
										if(strcmp(derivation_array[j],"''")==0) //if it derives empty break this loop
											break;
										else{ //push derivations
											push(derivation_array[j]);
										}
									}
									
									for (int j = 0 ; j < CsvParser_getNumFields(curr_derivation->derives) ; j++) { //push from the first to last item in array of terminals for the parse tree
										concat_data(derivation_array[j]);
										if(strcmp(derivation_array[j],"''")==0) //if it derives empty break this loop
											break;
									}
									
									found_derivation = true;
									break;
								}
								//move to the next derivation
								curr_derivation = curr_derivation->next;
							}
							//break "while(curr_symbol != NULL)" because symbol was already found
							break;
						}
						//move to the next symbol
						curr_symbol = curr_symbol->next;
					}//while
		    	}
				else{ //top of stack is terminal
		    		if (strcmp(get_top(), "$") == 0){	
						char popped_symbol[50]; //pop the top of the stack
		    			strcpy(popped_symbol, pop());
		    			printf("\nPOPPED: %s", popped_symbol);
		    			printf("\nTOP: %d", stack_top);
						add_line(popped_symbol);
						concat_data("$");
					}else{
						//ERROR
						found_derivation = false;
					}
				}
			}
		}
		if(!found_derivation && EOF_reached)
			break;
//		getch();
	}while(stack_top!=0);
	
	
	if(stack_top == 0){
		printf("\n\nParsing Complete!");
		print_tree();
//		construct_tree();
	}else{
		print_errors();
	}
	
//	getch();
	//destroy symbol table analyzer
    CsvParser_destroy(lexical_parser);
    
    //destroy parse table
//    unload_parse_table();
    getch();
    return 0;
}
