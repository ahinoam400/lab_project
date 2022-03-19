#include "commonFunctions.h"
#include "constant.h"
#include "structs.h"
int secondPass(char *filename){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL)
    {
        printf("ERORR OPENING FILE\n");
        return -1;
    }
    char line[MAX_LINE_LEN];
    char *token;
    int errFlag = 0;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    while(fgets(line, MAX_LINE_LEN, assembly)){
        token = strtok(line, " ");
        if(!isLegalSymName(token))
            continue;
        if(!(strcmp(token, ".data"))||!(strcmp(token, ".string"))||!(strcmp(token, ".extern")))
            continue;
        if(!strcmp(token, ".entry")){
            token = strtok(NULL, " ");
            if(!isNameInTable(token)){
                errFlag = 1;
                printf("ERROR: NAME IS NOT IN TABLE");
                continue;
            }
            sym = head;
            while(sym != NULL){
                if(!strcmp(sym->symbol, token)){
                    strcat(sym->attributes, ", entry");
                    break;
                }
                sym = sym->next;
            }
        }
    }
}
