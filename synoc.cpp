#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "lexical.h"

int main(int argc, char *argv[]){
	if(argc == 2){
		if(strcmp(argv[0],"synoc")==0){
			lexical_analysis(argv[1]);
		}else{
			printf("%s is not defined as a valid command", argv[0]);
		}
	}else{
		printf("Invalid Argument");
	}
	return 0;
}
