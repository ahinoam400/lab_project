#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
int MAX = 80;
int preAssembler(char* assemblyFileName);

int main(){
    preAssembler("assemblyExample.as");
}

int preAssembler(char* assemblyFileName){
    FILE *assembly = fopen(assemblyFileName, "r");
    FILE *afteAssembler;
    char *after;
    char str[80];
    if(assembly == NULL){
        printf("Error opening file\n");
        return(-1);
    }
    strcpy(after, assemblyFileName);
    strcat(after, ".am");
}