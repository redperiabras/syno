#ifndef PARSETREE_H_
#define PARSETREE_H_

//#include<stdlib.h>
//#include<string.h>
//#include<conio.h>
//#include<stdio.h>

int tree_level = 0;

struct TreeNode{
	char data[20];
	TreeNode *parent;
	TreeNode *child[20];
	int child_count = 0;
	int child_pointer = 0;
} *root_treenode, *curr_treenode, *parent_treenode;

void init_tree(char *input){
	root_treenode = (TreeNode *)malloc(sizeof(TreeNode));
	strcpy(root_treenode->data,input);
	curr_treenode = root_treenode;
}

void add_node(char *input){
	TreeNode *new_node = (TreeNode *)malloc(sizeof(TreeNode));
	strcpy(new_node->data,input);
	
	new_node->parent = parent_treenode; //define node's parent
	parent_treenode->child[parent_treenode->child_count] = new_node; //define node's childs
	parent_treenode->child_count++;
	
	curr_treenode = new_node; //move current pointer to the added child
}

void goto_parent(){
	if(strcmp(curr_treenode->data,root_treenode->data)==1){
		curr_treenode = curr_treenode->parent;
	}else{
		printf("Root Node already reached");
	}
}

//void construct_tree(){
//	FILE *file = fopen("syntree.txt","r");
//	char input[100];
//	bool root = true;
//	while(fgets(input, 100, file)!=NULL){
//		
//		CsvParser *derivation_parser = CsvParser_new_from_string(input, " ", 0);
//		
//		for (j = 0 ; j < CsvParser_getNumFields(temp) ; j++) {
//			if(j==0){
//				if(root){
//					init_tree(temp[0]);
//					root=false;
//				}else{
//					add_node(temp[j]);
//				}
//			}else{
//				add_node(temp[j]);
//			}	
//		}
//	}
//}
//characteristics ng node na parent
//	:pushed then popped
//characteristics ng node na child
//	:just pushed 
//

//
//
//void goto_parent(){
//	curr_treenode = curr_treenode->parent;
//}

struct parse_line{
	char derivation[100];
	parse_line *next;
} *curr_line, *head_line, *new_line;

bool parser_first = true;

void add_line(char *input){
	new_line = (parse_line *)malloc(sizeof(parse_line));
	strcpy(new_line->derivation, input);
//	strcat(new_line->derivation, " ");
	if(parser_first){
		head_line = new_line;
		curr_line = head_line;
		parser_first = false;
	}else{
		curr_line->next = new_line;
		curr_line = new_line;
	}
}

void concat_data(char *input){
	strcat(curr_line->derivation," ");
	strcat(curr_line->derivation,input);
}

void print_tree(){
	FILE *file = fopen("syntree.txt", "w");
	
	curr_line = head_line;
	printf("\nParse Tree:\n");
	while(curr_line!=NULL){
		
		printf("\t%s\n", curr_line->derivation);
		
		fprintf(file,"%s\n",curr_line->derivation);
		
		curr_line = curr_line->next;
		
	}
	
	fclose(file);
}
#endif

