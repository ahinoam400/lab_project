#include "commonFunctions.h"

void entryFile(symbol *head, char *fileName){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, fileName);
    FILE *entryF = fopen(strcat(fileNameCopy, ".ent"), "a");
    if (entryF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1));
    char str[MAX_LINE_LEN];
    symbol *sym = (symbol*)malloc(sizeof(symbol));
    while (sym != NULL){
        if (sym->attributes[8]!= '\0'){
            strcat(str, sym->symbol);
            strcat(str, atoi(sym->baseAddress));
            strcat(str, atoi(sym->offset));
            strcat(str, "\n");
            fwrite(str, 1, sizeof(str), entryF);
            str[0]= '\0';
        }
        sym = sym->next;
    }
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