#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
int MAX = 80;
int preAssembler(char* assemblyFileName);
char *removeExtension(char* myStr);
void macroSpread(char* fileName);
typedef struct macroNode{
    char name[MAX];
    char macro[MAX][MAX];
    macroList next;
}

int main(){
    preAssembler("assemblyExample.as");
}

int preAssembler(char* assemblyFileName){
    FILE *assembly = fopen(assemblyFileName, "r");
    FILE *afterAssembler;
    char *after;
    char str[MAX];
    if(assembly == NULL){
        printf("Error opening file\n");
        return(-1);
    }
    after = removeExtension(assemblyFileName);
    strcat(after, ".am");
    afterAssembler = fopen(after, "w");
    while(fgets(str, MAX, assembly)){
        
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

void macroSpread(char* fileName){
    FILE *assembly = fopen(fileName, "r");
    FILE *macroSpread = fopen("macroSpread", "a");
    macroNode *head = NULL, *node = head, *next, *temp;
    int macroFlag = 0, flag = 0, index = 0;;
    char *token;
    char line[MAX];
    while(fgets(line, MAX, assembly)!= EOF){
        if(macroFlag==1){
            if(line == "endm"){
                macroFlag = 0;
                continue;
            }
            next.macro[i++][0] = line;
        }
        while(node.next != NULL){
            if(node.name == line){
                fwrite(node.macro, sizeof(node.macro), 1, macroSpread);
                flag = 1;
                break;
            }
            node = node.next;
        }
        node = head;
        if(flag == 1){
            flag = 0;
            continue;
        }
        token = strtok(line, " ");
        if(token == "macro"){
            macroFlag = 1;
            token = strtok(NULL, " ");
            if(head == NULL){
                head.name = token;
                next = head;
                continue;
            }
            next.name = token;
            continue;
        }
        fwrite(line, sizeof(line), 1, macroSpread);
    }
    
}

