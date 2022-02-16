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
    FILE *macroSpreadFile;
    char *macroSpreadName;
    macroNode *head = NULL, *node = NULL, *tail = NULL;
    int macroFlag = 0, flag = 0, index = 0;
    char *token;
    char line[MAX], lineCopy[MAX];
    if(assembly == NULL ){
        printf("Error opening file\n");
        return;
    }
    macroSpreadName = removeExtension(fileName);
    strcat(macroSpreadName, ".am");
    macroSpreadFile = fopen(macroSpreadName, "w");
    head = (macroNode*)malloc(sizeof(macroNode));
    tail = head;
    while(fgets(line, MAX, assembly)){
        strcpy(lineCopy, line);
        token = strtok(lineCopy, " \n   ");
        if(macroFlag==1){
            if(strcmp(token,"endm") == 0){
                tail = tail->next;
                macroFlag = 0;
                continue;
            }
            strcpy(tail->macro[index++], line);
            continue;
        }
        node = head;
        do{
            /* This loop checks if the first field in the line is a macro from the list */
            if(node != NULL && token != NULL){
                if(strcmp(node->name,token) == 0){
                    strcat(node->macro[index], "\n");
                    fwrite(node->macro, strlen(node->macro), 1, macroSpreadFile);//Does not work
                    flag = 1;
                }
            }
            node = node->next;
        }
        while(!macroFlag && node != NULL);
        
        if(flag == 1){
            flag = 0;
            continue;
        }
        if(token != NULL){
            if(strcmp(token,"macro") == 0){
                macroFlag = 1;
                token = strtok(NULL, " \n");
                tail->next = (macroNode*)malloc(sizeof(macroNode));
                tail = tail->next;
                strcpy(tail->name, token);
                index = 0;
                continue;
            }
        }
     //   strcat(line, "\n");
        fwrite(line, strlen(line), 1, macroSpreadFile);
    }
    
}


