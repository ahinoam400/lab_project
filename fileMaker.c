#include "commonFunctions.h"

void entryFile(symbol *head, char *fileName, struct images *images){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, fileName);
    FILE *entryF = fopen(strcat(fileNameCopy, ".ent"), "a");
    if (entryF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1));
    char str[MAX_LINE_LEN];
    symbol *sym = (symbol*)malloc(sizeof(symbol));
    sym = images->symbol_head;
    while (sym != NULL){
        if (sym->attributes[8]!= '\0'){
            strcat(str, sym->symbol);
            strcat(str, ",");
            strcat(str, atoi(sym->baseAddress));
            strcat(str, ",");
            strcat(str, atoi(sym->offset));
            strcat(str, "\n");
            fwrite(str, 1, sizeof(str), entryF);
            str[0]= '\0';
        }
        sym = sym->next;
    }
}

void objectFile(struct images *images, char *fileName, int ICF, int DCF){
    char fileNameCopy[MAX_LINE_LEN];
    strcpy(fileNameCopy, fileName);
    FILE *objectF = fopen(strcat(fileNameCopy, ".ob"), "a");
    if (objectF == NULL)
        return (printAndReturn("ERROR OPENING FILE\n", -1));
    char str[MAX_LINE_LEN];
    code *c = (code*)malloc(sizeof(code));
    c = images->code_head;
    char temp;
    int optemp;
    char title[MAX_LINE_LEN];
    strcat(title, ICF);
    strcat(title, " ");
    strcat(title, DCF);
    fwrite(title, 1, sizeof(str), objectF);
    while(c != NULL){
        if(c->code_line->command != NULL){
            strcat(str, "A");
            if(c->code_line->command->absolute == 1) strcat(str, "4");
            else if(c->code_line->command->relocatable == 1) strcat(str, "2");
            else strcat(str, "1");
            temp = decToHex(c->code_line->command->opcode>>12);
            strcat(str, "-B");
            strcat(str, temp);
            optemp = temp;
            temp = optemp - decToHex(c->code_line->command->opcode>>8);
            strcat(str, "-C");
            strcat(str, temp);
            optemp += temp;
            temp = optemp - decToHex(c->code_line->command->opcode>>4);
            strcat(str, "-D");
            strcat(str, temp);
            optemp += temp;
            temp = optemp - decToHex(c->code_line->command->opcode);
            strcat(str, "-E");
            strcat(str, temp);
            fwrite(str, 1, sizeof(str), objectF);
            str[0] = '\0';
        }
        c = c->next;
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