#include "commonFunctions.h"
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]);
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isRegister(char *str);
int findAddressingMode(char *operand, int src_or_dest);
