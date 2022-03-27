#include "commonFunctions.h"
#include "fileMaker.h"
#include <stdio.h>
int entryFile(const char *fileName, struct images *images){
    char fileNameCopy[MAX_LINE_LEN];
    symbol *sym = images->symbol_head->next;
    FILE *entryF;
    strcpy(fileNameCopy, fileName);
    entryF = fopen(strcat(fileNameCopy, ".ent"), "w");
    if (entryF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    while (sym != NULL){
        if (sym->isEntry){
            fprintf(entryF, "%s,%04d,%04d\n", sym->symbol, sym->baseAddress, sym->offset);
        }
        sym = sym->next;
    }
    return 0;
}

int objectFile(const char *fileName, struct images *images){
    char fileNameCopy[MAX_LINE_LEN];
    FILE *objectF;
    strcpy(fileNameCopy, fileName);
    objectF = fopen(strcat(fileNameCopy, ".ob"), "w");
    if (objectF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    fprintf(objectF, "%d %d\n", images->ICF-BASE_ADDRESS, images->DCF);
    fprint_code(images->code_head, objectF);
    fprint_data(images->data_head, images->ICF, objectF);
    fclose(objectF);
    return 0;
}

int externalFile(const char *fileName, struct images *images){
    char fileNameCopy[MAX_LINE_LEN];
    external_words *node = images->ext_head->next;
    FILE *externalF;
    strcpy(fileNameCopy, fileName);
    externalF = fopen(strcat(fileNameCopy, ".ext"), "w");
    if (externalF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    while(node){
        fprintf(externalF, "%s BASE %04d\n", node->ext_word.symbol, node->ext_word.base_address);
        fprintf(externalF, "%s OFFSET %04d\n", node->ext_word.symbol, node->ext_word.offset);   
        node = node->next;
    }
    fclose(externalF);
    return 0;
}
