#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
int MAX = 80;
int preAssembler(char* assemblyFileName);
char *removeExtension(char* myStr);
void macroSpread(char* fileName);
typedef struct macroNode{
    char name[80];
    char macro[80][80];
    struct macroNode *next;
}macroNode;

int main(){
    macroSpread("assemblyExample.as");
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

void macroSpread(char *fileName){
    FILE *assembly = fopen(fileName, "r");
    FILE *macroSpreadFile;//the macro spread file should have the same name as fileName but with a diffrent extension
    char *macroSpreadName;
    macroNode *head = NULL, *node = head, *next, *temp;
    int macroFlag = 0, flag = 0, index = 0;
    char *token;
    char line[MAX], lineCopy[MAX];
    macroSpreadName = removeExtension(fileName);
    strcat(macroSpreadName, ".am");
    macroSpreadFile = fopen(macroSpreadName, "a");
    if(assembly == NULL ){
        printf("Error opening file\n");
        return;
    }
    head = (macroNode*)malloc(sizeof(macroNode));
    
    while(fgets(line, MAX, assembly)!= 0){
        strcpy(lineCopy, line);
        token = strtok(lineCopy, " ");
        if(macroFlag==1){
            if(!strcmp(token,"endm") == 0){
                macroFlag = 0;
                continue;
            }
            strcpy(next->macro[index++], line);
        }
        while(node->next != NULL){
            if(!strcmp(node->name,token) == 0){
                strcat(node->macro[index], "\n");
                fwrite(node->macro, sizeof(node->macro), 1, macroSpreadFile);
                flag = 1;
                break;
            }
            node = node->next;
        }
        node = head;
        if(flag == 1){
            flag = 0;
            continue;
        }
        if(strcmp(token,"macro") == 0){
            macroFlag = 1;
            token = strtok(NULL, " ");
            if(head == NULL){
                strcpy(head->name, token);
                next = head;
                continue;
            }
            strcpy(next->name, token);
            continue;
        }
        strcat(line, "\n");
        fwrite(line, sizeof(line), 1, macroSpreadFile);
    }
    
}

