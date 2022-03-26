#include "commonFunctions.h"
#include "fileMaker.h"
#include <stdio.h>
void entryFile(char *fileName, struct images *images){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, fileName);
    FILE *entryF = fopen(strcat(fileNameCopy, ".ent"), "w");
    if (entryF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    char str[MAX_LINE_LEN];
    symbol *sym = images->symbol_head->next;
    while (sym != NULL){
        if (sym->attributes[8]!= '\0'){
            fprintf(entryF, "%s,%04d,%04d\n", sym->symbol, sym->baseAddress, sym->offset);
        }
        sym = sym->next;
    }
}

void objectFile(struct images *images, char *fileName){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, fileName);
    FILE *objectF = fopen(strcat(fileNameCopy, ".ob"), "w");
    if (objectF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1, 0));
    fprintf(objectF, "%d %d\n", images->ICF-BASE_ADDRESS, images->DCF);
    fprint_code(images->code_head, objectF);
    fprint_data(images->data_head, images->ICF, objectF);
}

int decToHex(int decNumber){
	int temp;
	char hexNumber;
	temp = decNumber % 16;
	if( temp < 10)
	    temp =temp + 48; 
    else
	    temp = temp + 55;
	hexNumber = temp;
	return hexNumber;
}