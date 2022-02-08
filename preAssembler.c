#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
int MAX = 80;
int preAssembler(char* assemblyFileName);
char *removeExtension(char* myStr);

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
    after = removeExtension(assemblyFileName);
    strcat(after, ".am");
    afteAssembler = fopen(after, "w");
    while(fgets(str, 80, assembly)){
        
    }
}

//remove the extension from a file name
char *removeExtension(char* myStr) {
    char *retStr;
    char *lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return retStr;
}
