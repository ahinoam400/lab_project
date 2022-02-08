#include <stdio.h>
#include <stdlib.h>
int preAssembler(char* assemblyFile);

int main(){
    preAssembler("assembltExample.as");
}

int preAssembler(char* assemblyFile){
    fopen(assemblyFile, "r");
    fopen("aftePreAssembler.am", "w");
}