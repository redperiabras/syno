#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "lexical.h"

int main(int argc, char *argv[]){
	if(argc == 2){
		if(strcmp(argv[0],"synoc")==0){
			if(!(synoFile = fopen(argv[1],"r"))){
				printf("File not found...");
				return -1;
			}else{
				char *ext = strrchr(argv[1], '.');
				if (!ext) {
				    printf("Invalid File!");
					return -1;
				} else {
					if(strcmp(ext+1, "syno")!=0){
						printf("Invalid File!");
						return -1;
					}
					else{
						lexical_analysis(argv[1]);
					}
				}
			}
		}else{
			printf("%s is not defined as a valid command", argv[0]);
		}
	}else{
		printf("Invalid Argument");
	}
	return 0;
}
