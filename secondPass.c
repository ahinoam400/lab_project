#include "commonFunctions.h"

int secondPass(char *filename, code code_tail , code code_head , symbol symbol_head symbol symbol_tail){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, filename);
    FILE *assembly = fopen(strcat(fileNameCopy, ".am"), "r");
    if (assembly == NULL){
        printf("ERORR OPENING FILE\n");
        return -1;
    }
    char line[MAX_LINE_LEN];
    char *token;
    int errFlag = 0;
    int i=0;
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    while(fgets(line, MAX_LINE_LEN, assembly)){
        char *arr[MAX_LINE_LEN];
        split(line, arr);
        if(!isLegalSymName(arr[i]))
            continue;
        if(!(strcmp(arr[i], ".data"))||!(strcmp(arr[i], ".string"))||!(strcmp(arr[i], ".extern")))
            continue;
        if(!strcmp(arr[i], ".entry")){
            i++;
            if(!isNameInTable(arr[i], symbol_head)){
                errFlag = 1;
                printf("ERROR: NAME IS NOT IN TABLE");
                continue;
            }
            sym = head;
            while(sym != NULL){
                if(!strcmp(sym->symbol, arr[i])){
                    strcat(sym->attributes, ", entry");
                    break;
                }
                sym = sym->next;
            }
        }
    }
}
