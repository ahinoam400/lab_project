#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "constant.h"
#include "structs.h"

symbol *head = NULL, *tail = NULL;
code *head_code = NULL, *tail_code = NULL;
long int decimalToBinary(int n);
int isCommand(char commandName[MAX_LINE_LEN]);
void addSymbol(char symbolName[MAX_LINE_LEN], int IC, char attribute[MAX_LINE_LEN]);
int isLegalSymName(char symbolName[MAX_LINE_LEN]);
int isNameInTable(char symbolName[MAX_LINE_LEN]);
int isLegalNumber(char *number);
int isRegister(char *str);
int findAddressingMode(char *operand, int src_or_dest);
int addDataNode();
int split(char* str, char *arr[]);
int printAndReturn(char *str, int num);