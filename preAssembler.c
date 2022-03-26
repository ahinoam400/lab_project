#include "preAssembler.h"
#include "constant.h"
typedef struct macroNode{ /*a struct that holds within it a list with all of the macros*/
    char name[80];
    char macro[80][80];
    struct macroNode *next;
}macroNode;

int macroSpread(char *fileName){
    FILE *assembly;
    FILE *macroSpreadFile;
    char fileNameCopy[MAX_LINE_LEN];
    macroNode *head = NULL, *node = NULL, *tail = NULL;
    int macroFlag = 0, flag = 0, index = 0, macroLength;
    char *token;
    char line[MAX_LINE_LEN], lineCopy[MAX_LINE_LEN];
    size_t len = strlen(fileName);
    strcpy(fileNameCopy, fileName);
    assembly = fopen(strcat(fileNameCopy, ".as"), "r");
    fileNameCopy[len] = '\0';
    macroSpreadFile = fopen(strcat(fileNameCopy, ".am"), "w");
    if(assembly == NULL ){
        printf("ERROR : OPENING FILE %s\n", fileName);
        return 0;
    }
    head = (macroNode*)malloc(sizeof(macroNode));
    tail = head;
    while(fgets(line, MAX_LINE_LEN, assembly)){
        strcpy(lineCopy, line);
        token = strtok(lineCopy, " \n   ");
        if(macroFlag==1){
            if(strcmp(token,"endm") == 0){/*end of the macro */
                macroFlag = 0;
                continue;
            }
            /*copies the macro contents to the linked list */
            strcpy(tail->macro[index++], line);
            macroLength++;
            continue;
        }
        node = head;
        do{
            if(node != NULL && token != NULL){
                if(strcmp(node->name,token) == 0){/*checks if token is a macro from the list */
                    for(index = 0; index<macroLength; index++){
                        /*copies the macro contents to macroSpreadFile */
                        fwrite(node->macro[index], strlen(node->macro[index]), 1, macroSpreadFile);
                    }
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
            if(strcmp(token,"macro") == 0){/*add a new macro to the linked list*/
                macroFlag = 1, macroLength = 0, index = 0;
                token = strtok(NULL, " \n");
                tail->next = (macroNode*)malloc(sizeof(macroNode));
                tail = tail->next;
                strcpy(tail->name, token);
                continue;
            }
        }
        fwrite(line, strlen(line), 1, macroSpreadFile);
    }
    fclose(macroSpreadFile);
    fclose(assembly);
    return 1;
}