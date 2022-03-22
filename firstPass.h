#include "commonFunctions.h"
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN], symbol *head, symbol *tail);
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isRegister(char *str);
int addressingModeFirstPass(char *operand, int src_or_dest, symbol *sym_head, code *code_tail);