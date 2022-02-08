#include <stdio.h>
#include <stdlib.h> 
int MAX = 80;
int preAssembler(char* assemblyFile);

int main(){
    preAssembler("assemblyExample.as");
}

int preAssembler(char* assemblyFile){
    FILE *assembly = fopen(assemblyFile, "r");
    FILE *afteAssembler;
    char str[80];
    if(assembly == NULL){
        printf("Error opening file\n");
        return(-1);
    }
}